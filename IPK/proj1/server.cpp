/* * * * * * * * * *
 * AUTOR: xmarti76
 * server (ftrestd) pro IPK
 * 08. 03. 2017
 * * * * * * * * * */

#include "functions.hpp"
#include "server.hpp"
#include "error.hpp"

using namespace server;


/* * * * * *
 * Server  *
 * * * * * */
void Server::run_server () {
	// Zjistim si info o serveru
	struct addrinfo server_info; 	// Vstupni nastaveni serveru
	struct addrinfo *server_ip_s;	// Seznam volnych adres
	struct addrinfo *next;         	// Docasny ukazatel

	memset ( &server_info, 0, sizeof server_info );	//Nuluji strukturu
	server_info.ai_family   = AF_UNSPEC;     // don't care IPv4 or IPv6
	server_info.ai_socktype = SOCK_STREAM;   // TCP stream sockets
	server_info.ai_flags    = AI_PASSIVE;    // fill in my IP for me

	if ( getaddrinfo ( NULL, std::to_string ( options.get_port() ).c_str(), &server_info, &server_ip_s ) != 0 )
		exit_error ( "Run the server was unsuccessful" );

	// Zkusim se pripojit na jednu z adres
	for ( next = server_ip_s ; next != NULL ; next = next->ai_next ) {

		// Vytvorim socket
		server_socket = socket ( next->ai_family, next->ai_socktype, next->ai_protocol );
		if ( server_socket == -1 ) continue;	// Nepovedlo se -> zkusim to na dalsi adrese

		// Pripojeni se povedlo -> muzu ukoncit for
		if ( bind ( server_socket, next->ai_addr, next->ai_addrlen ) == 0 ) {
			break;
		}
		// Pripojeni se nepovedlo -> uzavru socket a zkousim dal
		else {
			close ( server_socket );
		}		
	}

	// Nepodarilo se pripojit na zadnou adresu
	if (next == NULL) exit_error ( "Run the server was unsuccessful" );
	
	// Nepodarilo se naslouchat na portu
	if ( listen ( server_socket, MAX_CLIENTS ) < 0 ) exit_error ( "Run the server was unsuccessful" );

	freeaddrinfo(server_ip_s);		// Uklizim po sobe
}

int Server::get_socket () {
	return server_socket;
}

Server::server_errors Server::listen_client ( int client_socket, server::Client_command &command ) {
	static const int BUFFER_SIZE = 4096;
	std::vector<char> buffer;
	buffer.resize( BUFFER_SIZE );

	// Prijmu data
	recv( client_socket, &buffer[0], buffer.size(), 0 );	// TODO dodelat test
	std::string command_string( buffer.begin(), buffer.end()) ;

	int pos = command_string.find( ' ' );
	int command_type = client::command_string_to_enum( command_string.substr( 0, pos ) );
	
	if ( command_type == -1 ) {
		return OTHER_ERROR;
	}
	else {
		std::string tmp_line = command_string.substr( pos + 1, command_string.find( CRLF ) - pos - 1 );	// Odstranim prvni slovo (prikaz)
		            tmp_line = tmp_line.substr( 0, tmp_line.rfind( "HTTP" ) - 1);	// Odstranim HTTP priznak  || NOTE asi bych si mel ulozit verzi HTML...

		if      ( tmp_line.compare( tmp_line.rfind( "?type="), std::string::npos, "?type=file" )    == 0 ) { command.operation_type = server::FILE; }
		else if ( tmp_line.compare( tmp_line.rfind( "?type="), std::string::npos, "?type=folder" )  == 0 ) { command.operation_type = server::FOLDER; }
		else { exit_error( "Client send unknown type '" + tmp_line.substr( tmp_line.rfind( "?type="), std::string::npos ) + "'!" ); }

		tmp_line = tmp_line.substr( 0, tmp_line.rfind( "?type=" ) );	// Odstranim typ operace

		// Prevedu typ operace na enum
		if ( command.operation_type == server::FOLDER ) {
			switch ( command_type ) {
				case client::GET:    command.type = client::LST; break;
				case client::PUT:    command.type = client::MKD; break;
				case client::DELETE: command.type = client::RMD; break;
			}
		}
		else if ( command.operation_type == server::FILE ) {
			switch ( command_type ) {
				case client::GET:    command.type = client::GET; break;
				case client::PUT:    command.type = client::PUT; break;
				case client::DELETE: command.type = client::DELETE; break;
			}			
		}

		// Opravim cestu
		from_html_spaces( tmp_line );
		
		size_t new_end;
		if ( tmp_line[ tmp_line.length() - 1 ] == '/' ) {
			new_end = tmp_line.length() - 1;
		}
		else {
			new_end = tmp_line.length();
		}

		if (  tmp_line[0] == '.' && tmp_line[1] == '/' ) {
			tmp_line = tmp_line.substr( 2, new_end );
		}
		else if (  tmp_line[0] == '/' ) {
			tmp_line = tmp_line.substr( 1, new_end );
		}
		
		command.path = tmp_line;


		// NOTE Tady bych mel parsovat zbytek hlavicky... -> ale pro projekt to neni potreba

		// Prectu zbytek dat
		if ( command.type == client::PUT ) {
				// Nactu si delku obsahu
				tmp_line = command_string.substr( command_string.find( "Content-Length" ), std::string::npos );
				tmp_line = tmp_line.substr( 16 , tmp_line.find( CRLF ) - 16 );		// 16 = Content-Length
				
				// Po kontrole ji ulozim
				if ( tmp_line.find_first_not_of( "0123456789" ) != std::string::npos ) { return Server::OTHER_ERROR; }
				command.file_length = stol( tmp_line );
		
				// Zapisu data do file a prectu zbytek dat (je-li to potreba)
				tmp_line = command_string.substr( command_string.find( CRLF + CRLF ) + 4, std::string::npos );		// 4 = CR LF CR LF
				std::copy( tmp_line.begin(), tmp_line.end(), std::back_inserter( command.file ) );

				while ( command.file.size() < command.file_length ) {
					recv( client_socket, &buffer[0], buffer.size(), 0 );	// TODO dodelat test
					std::copy( buffer.begin(), buffer.end(), std::back_inserter( command.file ) );
				}
		}
	}

	return Server::OK;
}

void Server::do_command ( int client_socket, Client_command &command ) {
	std::vector<char> buffer;
	std::fstream file;

	// Kontrola na pristup do uzivatelske slozky
	// Jsem v rootu
	if ( command.path.empty() ) { answer_client ( client_socket, FORBIDDEN ); }
	
	// Opustil jsem uzivatelskou slozku!
	int count = count_substring( command.path, std::string( "/.." ) );
	if ( count && count_substring( command.path, std::string( "/" ) ) < 2 * count ) { answer_client ( client_socket, FORBIDDEN ); }

	// Opravim path podle rootu
	std::string path = command.path;
	if ( !options.get_root_folder().empty() ) {
		path = options.get_root_folder() + command.path;
	}
	
	switch ( command.type ) {
		case client::DELETE:
			if ( !is_file( path ) ) {
				if ( is_folder( path ) ) {
					answer_client ( client_socket, BAD_REQUEST );
					break;
				}
				else                             {
					answer_client ( client_socket, NOT_FOUND );
					break;
				}
			}

			if ( remove( path.c_str() ) != 0 ) {
				answer_client( client_socket, SERVER_ERROR );
				break;
			}

			answer_client( client_socket, OKOK );
			break;

		case client::GET:
			if ( !is_file( path ) ) {
				answer_client ( client_socket, NOT_FOUND );
				break;
			}
		
			file.open( path.c_str(), std::fstream::in | std::fstream::binary );
		
			if ( !file.is_open() ) {
				answer_client( client_socket, SERVER_ERROR );
				break;
			}
		
			answer_client( client_socket, OKOK, file );
			break;

		case client::PUT:
			file.open( path.c_str(), std::fstream::out | std::fstream::binary );
		
			if ( !file.is_open() ) {
				answer_client( client_socket, SERVER_ERROR );
				break;
			}

			file.write( &command.file[0], command.file_length );
			file.close();

			answer_client( client_socket, OKOK );
			break;

		case client::LST:
			if ( is_file( path ) ) {
				answer_client ( client_socket, BAD_REQUEST );
				break;
			}
			if ( !is_folder( path ) ) {
				answer_client ( client_socket, NOT_FOUND );
				break;
			}
			if ( !call_ls( path, buffer ) ) {
				answer_client( client_socket, SERVER_ERROR );
				break;
			}
			
			answer_client( client_socket, OKOK, buffer );
			break;

		case client::MKD:
			if ( !is_folder( path.substr( 0, path.find_last_of( '/') ) ) ) {
				answer_client ( client_socket, BAD_REQUEST );
				break;
			}
			if ( is_folder( path ) ) {
				answer_client ( client_socket, METHOD_NOT_ALLOWED );
				break;
			}
			if ( mkdir( path.c_str(), S_IRWXU ) != 0 ) {
				answer_client( client_socket, SERVER_ERROR );
				break;
			}

			answer_client( client_socket, OKOK );
			break;

		case client::RMD:
			if ( !is_folder( path ) ) {
				if ( is_file( path ) ) {
					answer_client ( client_socket, BAD_REQUEST );
					break;
				}
				else {
					answer_client ( client_socket, NOT_FOUND );
					break;
				}
			}

			if ( remove( path.c_str() ) != 0 ) {
				answer_client( client_socket, METHOD_NOT_ALLOWED );
				break;
			}

			answer_client( client_socket, OKOK );
			break;

		default:
			answer_client( client_socket, SERVER_ERROR );
	}
}

void Server::answer_client ( int client_socket, return_code code ) {
	std::vector<char> file;
	answer_client ( client_socket, code, file );
}
void Server::answer_client ( int client_socket, return_code code, std::fstream &file ) {
	if ( code == OKOK ) {
		std::vector<char> output;
		std::string answer ( "HTTP/1.1 200 OK" + CRLF );
		answer += "Date: " + get_time() + CRLF;
		answer += "Content-Type: application/json" + CRLF;
		

		file.seekg( 0, file.end );
		unsigned int file_size = file.tellg();
		file.seekg( 0, file.beg );

		answer += "Content-Length: " + std::to_string( file_size ) + CRLF;
		answer += "Content-Encoding: identity" + CRLF + CRLF;
		std::copy( answer.begin(), answer.end(), std::back_inserter( output ) );
		file_to_vector( file, output );
	
		send( client_socket, &output[0], output.size(), 0 );
	}
	else {
		std::vector<char> file;
		answer_client ( client_socket, code, file );
	}

}
void Server::answer_client ( int client_socket, return_code code, std::vector<char> &file ) {

	std::string answer ("HTTP/1.1 ");
	std::vector<char> output;

	switch ( code ) {
		case OKOK:               answer += "200 OK" + CRLF; break;
		case BAD_REQUEST:        answer += "400 Bad Request" + CRLF; break;
		case FORBIDDEN:          answer += "403 Forbidden" + CRLF; break;
		case NOT_FOUND:          answer += "404 Not Found" + CRLF; break;
		case METHOD_NOT_ALLOWED: answer += "405 Method Not Allowed" + CRLF; break;
		case SERVER_ERROR:
		default:           answer += "500 Internal Server Error" + CRLF; break;
	}

	// Pridam zbytek hlavicky
	answer += "Date: " + get_time() + CRLF;
	answer += "Content-Type: application/json" + CRLF;
	
	if ( code == OKOK ) {
		answer += "Content-Length: " + std::to_string( file.size() ) + CRLF;
		answer += "Content-Encoding: identity" + CRLF + CRLF;
		std::copy( answer.begin(), answer.end(), std::back_inserter( output ) );
		std::copy(file.begin(), file.end(), std::back_inserter( output ) );
	}
	else {
		std::copy( answer.begin(), answer.end(), std::back_inserter( output ) );
	}

	send( client_socket, &output[0], output.size(), 0 );
}
