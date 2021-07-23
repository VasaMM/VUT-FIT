/* * * * * * * * * *
* AUTOR: xmarti76
* client pro IPK
* 08. 03. 2017
* * * * * * * * * */

#include <iostream>
#include <string>

#include "functions.hpp"
#include "client.hpp"

using namespace std;

int main ( int argc, char *argv[] ) {

	client::Client client;

	// Nactu arumenty do options
	read_args( client.options, argc, argv );

	client.connect_server();
	
	client.sent_command();

	client.listen_answer();

	client.disconnect_server();

	return 0;
}