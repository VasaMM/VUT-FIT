/* * * * * * * * * * * * * * * * * * *
 * AUTOR: Vaclav Martinka (xmarti76)
 * POP3 klient - síťové funkce
 * 20. 11. 2017 FIT VUT
 * * * * * * * * * * * * * * * * * * */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <regex>
#include <string>

#include "client.hpp"
#include "functions.hpp"
#include "options.hpp"
#include "error.hpp"

using namespace std;


void Client::connect_server () {
	say( "Pripojuji se k serveru " + options->server() + ":" + to_string( options->port() ) );

	// Pripojim se k serveru
	addrinfo flags = { 
		0,            // ai_flags
		AF_UNSPEC,    // ai_family
		SOCK_STREAM,  // ai_socktype
	};
	
	// Otestuji adresu a port
	addrinfo *res;
	if ( getaddrinfo( options->server().c_str(), nullptr, &flags, &res ) != 0) {
		exit_error( "Unable to connect '" + options->server() + "'. Maybe bad server address?" );
	}
	
	// Otestuji adresu a port
	if ( getaddrinfo( options->server().c_str(), to_string( options->port() ).c_str(), &flags, &res ) != 0) {
		exit_error( "Unable to connect '" + options->server() + "'. Maybe bad port?" );
	}

	struct addrinfo *next;
	for ( next = res ; next != NULL ; next = next->ai_next ) {
		// Vytvorim sockety
		if ( ( pop3_socket = socket( res->ai_family, res->ai_socktype, res->ai_protocol ) ) < 0 ) {
			freeaddrinfo( res );
			exit_error( "Creating socket was unsuccessful." );
		}

		// Nastavim timeout
		struct timeval timeout;
			timeout.tv_sec = 10;
			timeout.tv_usec = 0;

		if ( setsockopt( pop3_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof( timeout ) ) < 0 ||
		     setsockopt( pop3_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof( timeout ) ) < 0
		   ) {
			freeaddrinfo( res );
			exit_error( "Couldn't set timeout for socket." );
		}

		
		// Vyresim IPv6
		if ( is_IPv4( res ) ) {
			say( "IPv4" );
			// Pripojim se
			if ( connect( pop3_socket, res->ai_addr, res->ai_addrlen ) < 0 ) {
				close( pop3_socket );    // Pripojeni se nepovedlo, zkusim dalsi adresu
			}
			// Povedlo se
			else {
				freeaddrinfo( res );
				break;
			}
		}
		else {
			say( "IPv6" );
			// Pripojim se
			struct sockaddr_in6 serv_addr;
				memset( (char *) &serv_addr, 0, sizeof( serv_addr ) );
				serv_addr.sin6_flowinfo = 0;
				serv_addr.sin6_family = AF_INET6;
				serv_addr.sin6_port = htons( options->port() );

			struct hostent *server;
			server = gethostbyname2( options->server().c_str(), AF_INET6 );
			
			if ( server == NULL ) {
				exit_error( "Unable to connect '" + options->server() + "'. Maybe bad IPv6 server address?" );
			}

			memmove(
				(char *) &serv_addr.sin6_addr.s6_addr,
				(char *) server->h_addr, server->h_length
			);

			if ( connect( pop3_socket, ( struct sockaddr * ) &serv_addr, sizeof( serv_addr ) ) < 0 ) {
				close( pop3_socket );    // Pripojeni se nepovedlo, zkusim dalsi adresu
			}
			// Povedlo se
			else {
				// freehostent( server );	// Nevim proc, ale nefunguje...
				freeaddrinfo( res );
				break;
			}
		}
	}

	// Nepodarilo se pripojit na zadnou adresu
	if ( next == NULL ) {
		exit_error( "Unable to connect '" + options->server() + "'." );
	}


	// Navazi nesifrovane prpojeni
	if ( options->ciphering() != Options::POP3S ) {
		// Prijmu uvitaci zpravu
		memset( buffer, 0, BUFFER_SIZE );
		int answer = recv( pop3_socket, buffer, BUFFER_SIZE, 0);
		if ( answer < 0 ) {
			disconnect_server();
			exit_error( "Couldn't receive greeting message from server." );
		}
		else {
			buffer[ answer ] = '\0';
			say( "PRIJATO: " + string( buffer ) );
		}
	}
	
	// Jedna se o sifrovane spojeni	
	if ( options->ciphering() != Options::NONE ) {
		string answer;
		
		if ( options->ciphering() == Options::STLS ) {
			// Poslu zadost na sifrovane spojeni
			send_message( "STLS\r\n", answer );
			
			if ( ! is_it_OK( answer ) ) {
				disconnect_server();
				exit_error( "Couldn't switch to security connection." );
			}		
			
		}

		init_SSL();
		
		if ( options->ciphering() == Options::POP3S ) {
			say( "Cekam na odpoved od serveru" );
			receive_message( answer );
			if ( ! is_it_OK( answer ) ) {
				disconnect_server();
				exit_error( "Couldn't receive greeting message from server." );
			}
		}
	}
}


bool Client::disconnect_server () {
	say( "Odpojim se od serveru" );
	string answer;
	send_message( "QUIT\r\n", answer );

	switch ( options->ciphering() ) {
		case Options::POP3S:
		case Options::STLS:
			SSL_CTX_free( ctx );
			SSL_shutdown( ssl );
			SSL_free    ( ssl );
			ERR_free_strings();
			EVP_cleanup();
			break;
			
		case Options::NONE:
			close( pop3_socket );
	}

	say( is_it_OK( answer ) ? "Odpojeni uspesne" : "Odpojeni neuspesne" );
	return is_it_OK( answer );
}


void Client::send_message ( string message ) {
	int answer = 0;
	
	if ( use_SSL ) { answer = SSL_write( ssl, message.c_str(), message.length() ); }
	else           { answer = send( pop3_socket, message.c_str(), message.length(), 0 ); }
	
	if ( answer < 0 ) {
		say( "\tODESLANO: null" );
		disconnect_server();
		exit_error( "Couldn't send message to server." );
	}
	
	say( "\tODESLANO: " + message, "" );
}


void Client::send_message ( string message, string &output, int message_size ) {
	send_message( message );
	receive_message( output, message_size );
}


void Client::receive_message ( string &output, int message_size ) {
	output.clear();
	memset( buffer, 0, BUFFER_SIZE );

	int answer = 0;
	if   ( use_SSL ) { answer = SSL_read( ssl, buffer, BUFFER_SIZE ); }
	else             { answer = recv( pop3_socket, buffer, BUFFER_SIZE, 0 ); }

	// Escape zanku CRLF.. na CRLF.
	replace_all( output, "\r\n..", "\r\n." );

	if ( answer > 0 ) {
		buffer[ answer ] = '\0';
		output = buffer;
		// Escape zanku CRLF.. na CRLF.
		replace_all( output, "\r\n..", "\r\n." );

		say( "\tPRIJATO: " + output.substr(0, output.find( "\r\n" ) ) );
		
		// Ocekavam dlouhou zpravu a znam velikost obsahu
		if ( message_size > 0 ) {
			
			// Zkontroluji, zda ma vubec cenu cist dal
			if ( ! is_it_OK( output ) ) {
					output.clear();
					return;
				}
			
			// Zjistim velikost prvniho radku a odectu ho
			size_t first_line_size = output.find( "\n" );	
			output = output.substr( first_line_size + 1 );
			
			// Prictu ji k velikosti zpravy + 4b na zakonecni
			message_size = message_size - answer + first_line_size + 4;

			while ( true ) {
				if   ( use_SSL ) { answer = SSL_read( ssl, buffer, BUFFER_SIZE ); }
				else             { answer = recv( pop3_socket, buffer, BUFFER_SIZE, 0 ); }

				message_size -= answer;
				
				if ( message_size > 0 ) {
					buffer[ answer ] = '\0';
					output += buffer;
				}
				else {
					buffer[ answer - 5 ] = '\0';
					output += buffer;
					break;
				}
			}
		}
		// Ocekavam dlouho zpravu, ale neznam jeji delku
		else if ( message_size == 0 ) {
			// Zkontroluji, zda ma vubec cenu cist dal
			if ( ! is_it_OK( output ) ) {
				output.clear();
				return;
			}

			// Zkontroluji konec zpravy na CRLF.CRLF
			if (
				output[ output.size() - 5 ] == '\r' &&
				output[ output.size() - 4 ] == '\n' &&
				output[ output.size() - 3 ] == '.'  &&
				output[ output.size() - 2 ] == '\r' &&
				output[ output.size() - 1 ] == '\n'
			) { return; }

			while( true ) {
				buffer[ answer ] = '\0';
				output += buffer;
				// Zkontroluji, zda ma vubec cenu cist dal
				if ( ! is_it_OK( output ) ) {
					output.clear();
					return;
				}

				// Zkontroluji konec zpravy na CRLF.CRLF
				if (
					output[ output.size() - 5 ] == '\r' &&
					output[ output.size() - 4 ] == '\n' &&
					output[ output.size() - 3 ] == '.'  &&
					output[ output.size() - 2 ] == '\r' &&
					output[ output.size() - 1 ] == '\n'
				) { return; }

				if   ( use_SSL ) { answer = SSL_read( ssl, buffer, BUFFER_SIZE ); }
				else             { answer = recv( pop3_socket, buffer, BUFFER_SIZE, 0 ); }
			}
		}
		// Jinak ocekavam kratkou zpravu -> staci mi jedno cteni
	}
	// Nekde nastala chyba
	else {
		say( "\tPRIJATO: null" );
		disconnect_server();
		exit_error( "Couldn't receive message from server." );
	}
}


bool Client::log_in () {
	say( "Prihlasovani" );

	string answer;

	// Poslu jmeno
	send_message( "USER " + options->name() + "\r\n", answer );
	// Otestuji odpoved
	if ( is_it_OK( answer ) ) {
		// Poslu heslo
		send_message( "PASS " + options->password() + "\r\n", answer );
		// Povedlo se mi prihlasit
		say ( (is_it_OK( answer ) ? "Prihasovani - uspesne" : "Prihasovani - neuspesne" ) );
		return is_it_OK( answer );
	}
	
	return false;
}

bool Client::is_it_OK ( string &message ) {
	// Otestuji, zda zprava zacina +OK
	return (
		message[0] == '+' &&
		message[1] == 'O' &&
		message[2] == 'K'
	);
}

bool Client::list ( string &answer ) {
	send_message( "LIST\r\n", answer, 0 );
	return is_it_OK( answer );
}

// Vraci pocet zprav na serveru
// -1 v pripade chyby
int Client::statistics ( string &answer ) {
	send_message( "STAT\r\n", answer );

	unsigned int messages_on_server;

	// ulozim si pocet zprav
	if ( is_it_OK( answer ) ) {
		// +OK n size
		regex count ("^\\+OK (\\d+) \\d+");
		smatch match;
		if ( regex_search ( answer, match, count ) ) {
			string number = match[ 1 ];
			if ( str_to_uint( number, messages_on_server ) ) {
				return messages_on_server;
			}
		}
	}
	
	return -1;
}

bool Client::read_message ( string &answer, int index, int message_size ) {
	send_message( "RETR " + to_string( index ) + "\r\n", answer, message_size );
	if ( message_size >= 0 ) {
		return ( answer.size() != 0 );
	}
	else return is_it_OK( answer );
}

bool Client::delete_message ( string &answer, int index ) {
	send_message( "DELE " + to_string( index ) + "\r\n", answer );
	return is_it_OK( answer );
}


void Client::init_SSL() {
	say( "Navazuji sifrovane pripojeni" );
	int err = 0;

	// Inicializuji knihovnu
	// OpenSSL_add_all_algorithms();
	OpenSSL_add_ssl_algorithms();
	SSL_load_error_strings();
	ERR_load_BIO_strings();


	// Vytorim spojeni
	ctx = SSL_CTX_new( TLSv1_2_client_method() );
	if( ctx == NULL ) {
		ERR_free_strings();
		EVP_cleanup();
		exit_error( "Secure connection failed (cannot use certificate)." );
	}

	loadCertificates();

	// Navazu pripojeni
	ssl = SSL_new( ctx );
	if( ssl == NULL ) {
		ERR_free_strings();
		EVP_cleanup();
		exit_error( "Secure connection failed (cannot use SSL)." );
	}

	err = SSL_set_fd( ssl, pop3_socket );
	if ( err < 1 ) {
		ERR_free_strings();
		EVP_cleanup();
		SSL_CTX_free( ctx );
		exit_error( "Secure connection failed (server connect server)." );
	}
	
	if ( SSL_connect( ssl ) < 1 ) {
		SSL_CTX_free( ctx );
		SSL_shutdown( ssl );
		SSL_free    ( ssl );
		ERR_free_strings();
		EVP_cleanup();
		exit_error( "Secure connection failed (server close connection)." );
	}

	say( "Připojeno" );

	// Zkontroluji certifikat
	if( SSL_get_peer_certificate( ssl ) == NULL ||
	    SSL_get_verify_result( ssl ) != X509_V_OK
	  ) {
		SSL_CTX_free( ctx );
		SSL_shutdown( ssl );
		SSL_free    ( ssl );
		ERR_free_strings();
		EVP_cleanup();
		exit_error( "Secure connection failed (invalid SSL certifikate)." );
	}

	use_SSL = true;
	
	say( "Inicialiazce sifrovaneho spojeni dokoncena" );
}


void Client::loadCertificates() {
	bool load_default = true;

	// Mam zadany soubor s certifikatem
	if( options->certfile().size() ) {
		// Soubor neexistuje
		if ( ! is_file( options->certfile() ) ) {
			exit_error( "Couldn't open the certificate '" + options->certfile() + "'." );
		}

		if( ! SSL_CTX_load_verify_locations( ctx, const_cast<char *>( options->certfile().c_str() ), NULL ) ) {
			exit_error( "Couldn't load the certificate '" + options->certfile() + "'." );
		}

		load_default = false;
	}

	// Mam zadanou slozku s certifikaty - otestuji ji
	if( options->certaddr().size() ) {
		// Slozka neexistuje
		if ( ! is_folder( options->certaddr() ) ) {
			exit_error( "Couldn't open the directory '" + options->certaddr() + "'." );
		}

		if( ! SSL_CTX_load_verify_locations( ctx, NULL, const_cast<char *>( options->certaddr().c_str() ) ) ) {
			exit_error( "Couldn't load the certificate directory '" + options->certfile() + "'." );
		}

		load_default = false;
	}

	// Nemam zadane ani jedno - pouziji defaultni
	if ( load_default ) {
		SSL_CTX_set_default_verify_paths( ctx );
	}
}



