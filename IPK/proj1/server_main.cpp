/* * * * * * * * * *
 * AUTOR: xmarti76
 * server (ftrestd) pro IPK
 * 08. 03. 2017
 * Use http://beej.us/guide/bgnet/output/print/bgnet_A4.pdf
 * * * * * * * * * */

#include <iostream>
#include <string>

#include "functions.hpp"
#include "server.hpp"
#include "client.hpp"
#include "error.hpp"

using namespace std;

int main ( int argc, char *argv[] ) {

	server::Server server;
	// Nactu arumenty do options
	if ( argc > 1 ) {
		read_args( server.options, argc, argv );
	}
	else {
		server.options.set_port( 6677 );
		server.options.set_root_folder( "" );
	}

	server.run_server();
	
	// Server bezi a ceka na pozadavky
	while ( true ) {
// TODO dodelat ukonceni serveru
		struct sockaddr_storage client_addres;
		socklen_t addres_size = sizeof ( client_addres );
		
		int client_socket = accept( server.get_socket(), (struct sockaddr *) &client_addres, &addres_size );
		if ( client_socket == -1 ) { continue; }	// Chyba -> obslouzim dalsiho klienta

		int process_id = fork();
		if ( process_id < 0 ) {
			exit_error( "Problem with service of client!" );
		}
		else if ( process_id == 0 ) {	// Podarilo se vytvorit novy proces pro obsluhu
			server::Client_command command;
			server.listen_client( client_socket, command );
			server.do_command( client_socket, command );
			exit( 0 );
		}
		else { close( client_socket ); }	// Obslouzeno, cekam na dalsi
	}

	return 0;
}