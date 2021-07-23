/* * * * * * * * * *
 * AUTOR: xmarti76
 * client pro IPK
 * 08. 03. 2017
 * * * * * * * * * */

#include "functions.hpp"
#include "client.hpp"
#include "error.hpp"

using namespace client;

int client::command_string_to_enum ( std::string command ) {

	for ( auto i = 0 ; i < CLIENT_COMMAND_COUNTER ; i++ ) {
		if ( client_command_string[ i ] == command )
			return i;
	}

	return -1;
}

/* * * * * * * * * *
 * Client options  *
 * * * * * * * * * */
void Options::prepare_file () {
	switch ( get_command() ) {
		case GET:
			file.open( get_local_path().c_str(), std::fstream::out | std::fstream::binary );
			if ( !file.is_open() ) exit_error( "Creating the '" + get_local_path() + "' was unsuccessful!" );

			break;

		case PUT:
			file.open( get_local_path().c_str(), std::fstream::in | std::fstream::binary );
			if ( !file.is_open() ) exit_error( "Opening the '" + get_local_path() + "' was unsuccessful!" );

			break;

		case DELETE:
		case LST:
		case MKD:
		case RMD:
		default:
			return;
	}

	// Zapisu velikost souboru
	file.seekg( 0, file.end );
	file_size = file.tellg();
	file.seekg( 0, file.beg );
}



/* * * * * *
 * Client  *
 * * * * * */

void Client::connect_server () {
	// Zjistim si info o serveru
	struct addrinfo client_info; 	// Vstupni nastaveni serveru
	struct addrinfo *server_ip_s;	// Seznam volnych adres
	struct addrinfo *next;         	// Docasny ukazatel

	memset( &client_info, 0, sizeof client_info );	//Nuluji strukturu
	client_info.ai_family   = AF_UNSPEC;     // don't care IPv4 or IPv6
	client_info.ai_socktype = SOCK_STREAM;   // TCP stream sockets

	if ( getaddrinfo( options.get_address().c_str(), std::to_string( options.get_port() ).c_str(), &client_info, &server_ip_s ) != 0 )
		exit_error( "Connecting the server was unsuccessful" );

	// Zkusim se pripojit na jednu z adres
	for ( next = server_ip_s ; next != NULL ; next = next->ai_next ) {

		// Vytvorim socket
		client_socket = socket( next->ai_family, next->ai_socktype, next->ai_protocol );
		if ( client_socket == -1 ) continue;	// Nepovedlo se -> zkusim to na dalsi adrese

		// Pripojeni se povedlo -> muzu ukoncit for
		if ( connect( client_socket, next->ai_addr, next->ai_addrlen ) != -1 ) {
			break;
		}
		// Pripojeni se nepovedlo -> uzavru socket a zkousim dal
		else {
			close( client_socket );
		}		
	}

	// Nepodarilo se pripojit na zadnou adresu
	if (next == NULL) exit_error( "Connecting the server was unsuccessful" );

	freeaddrinfo(server_ip_s);		// Uklizim po sobe
}

void Client::disconnect_server () {
	close( client_socket );
}

void Client::sent_command () {
	std::string tmp;
	std::string header;
	std::vector<char> output;
	std::vector<char> file_content;

	switch ( options.get_command() ) {
		case DELETE: header = "DELETE " + options.get_remote_path() + "?type=file HTTP/1.1" + CRLF; break;
		case GET:    header = "GET "    + options.get_remote_path() + "?type=file HTTP/1.1" + CRLF; break;
		case PUT:    header = "PUT "    + options.get_remote_path() + "?type=file HTTP/1.1" + CRLF; break;
		case LST:    header = "GET "    + options.get_remote_path() + "?type=folder HTTP/1.1" + CRLF; break;
		case MKD:    header = "PUT "    + options.get_remote_path() + "?type=folder HTTP/1.1" + CRLF; break;
		case RMD:    header = "DELETE " + options.get_remote_path() + "?type=folder HTTP/1.1" + CRLF; break;
		default: exit_error( "This is big problem..." ); 
	}

	// std::copy( header.begin(), header.end(), std::back_inserter( output ) );
	// Pridam zbytek hlavicky
	header += "Host: " + options.get_address() + CRLF;
	header += "Date: " + get_time() + CRLF;
	header += "Accept: application/json" + CRLF;
	header += "Accept-Encoding: identity" + CRLF;
	std::copy( header.begin(), header.end(), std::back_inserter( output ) );

	if ( options.get_command() == PUT ) {
		header += "Content-Length: " + std::to_string( options.get_file_size() ) + CRLF + CRLF;

		std::copy( header.begin(), header.end(), std::back_inserter( output ) );
		file_to_vector( *options.get_file(), output );
	}
	
	send( get_socket(), &output[0], output.size(), 0);
}

void Client::listen_answer () {
	static const int BUFFER_SIZE = 4096;
	std::vector<char> buffer;
	buffer.resize( BUFFER_SIZE );

	// Prijmu data
	recv( client_socket, &buffer[0], buffer.size(), 0 );	// TODO dodelat test
	std::string command_string( buffer.begin(), buffer.end()) ;

	//Zkontroluji HTML odpoved
	if ( command_string.compare( 0, 4, "HTTP" ) != 0 ) { exit_error( "Server didn't respond valid HTTP answer!" ); }

	// Odignoruji verzi HTML
	int pos = command_string.find( ' ' );
	command_string = command_string.substr( pos, std::string::npos );

	// Nactu kod odpovedi
	pos = command_string.find( ' ' );
	if ( command_string.substr( 0, pos ).find_first_not_of( "0123456789" ) != std::string::npos ) { exit_error( "Server didn't respond valid HTTP answer!" ); }
	int answer = stoi( command_string.substr( pos ) );

	switch ( answer ) {
		case server::OKOK:
			// Ocekavam nejakou odpoved -> musim ji nacist
			if ( options.get_command() == GET || options.get_command() == LST ) {
				std::string tmp_string;
				std::vector<char> answer_content;
				// Nactu si delku obsahu
				tmp_string = command_string.substr( command_string.find( "Content-Length" ), std::string::npos );
				tmp_string = tmp_string.substr( 16 , tmp_string.find( CRLF ) - 16 );		// 16 = Content-Length
				
				// Po kontrole ji ulozim
				if ( tmp_string.find_first_not_of( "0123456789" ) != std::string::npos ) { exit_error( "Server didn't respond valid HTTP answer!" ); }
				unsigned long file_length = stol( tmp_string );
		
				// Zapisu data do file a prectu zbytek dat (je-li to potreba)
				tmp_string = command_string.substr( command_string.find( CRLF + CRLF ) + 4, std::string::npos );		// 4 = CR LF CR LF
				std::copy( tmp_string.begin(), tmp_string.end(), std::back_inserter( answer_content ) );

				while ( answer_content.size() < file_length ) {
					recv( client_socket, &buffer[0], buffer.size(), 0 );	// TODO dodelat test
					std::copy( buffer.begin(), buffer.end(), std::back_inserter( answer_content ) );
				}

				if ( options.get_command() == GET ) {
					options.get_file()->write( &answer_content[0], file_length );
					options.close_file();
				}
				else {
					for (std::vector<char>::const_iterator i = answer_content.begin(); i != answer_content.begin() + file_length; ++i)
						std::cout << *i;
				}
			}

			break;
		
		case server::BAD_REQUEST:
			if ( options.get_command() == LST    || options.get_command() == RMD || options.get_command() == MKD ) { exit( "Not a directory." ); }
			if ( options.get_command() == DELETE || options.get_command() == GET || options.get_command() == PUT ) { exit( "Not a file." ); }
			// exit_error( "Server answer '400 Bad Request'." );
			exit( "Unknown error." );

		case server::NOT_FOUND:
			if ( options.get_command() == LST    || options.get_command() == RMD || options.get_command() == MKD ) { exit( "Directory not found." ); }
			if ( options.get_command() == DELETE || options.get_command() == GET || options.get_command() == PUT ) { exit( "File not found." ); }
			// exit_error( "Server answer '404 Not Found'." );
			exit( "Unknown error." );
		
		case server::METHOD_NOT_ALLOWED:
			if ( options.get_command() == RMD ) { exit( "Directory not empty." ); }
			if ( options.get_command() == MKD ) { exit( "Already exists." ); }
			// exit_error( "Server answer '405 Method Not Allowed'." );
			exit( "Unknown error." );

		case server::FORBIDDEN:
			// exit_error( "Server answer '403 Forbidden'." );
			exit ( "User Account Not Found" );

		case server::SERVER_ERROR:
			// exit_error( "Server answer '500 Internal Server Error'." );
			exit( "Unknown error." );
		default:
			// exit_error( std::string( "Server answer '" ) + std::to_string( answer ) + std::string( "'." ) );
			exit( "Unknown error." );
	}
}
