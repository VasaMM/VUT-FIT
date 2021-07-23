/* * * * * * * * * *
 * AUTOR: xmarti76
 * server (ftrestd) pro IPK
 * 08. 03. 2017
 * * * * * * * * * */

#ifndef SERVER_HPP
#define SERVER_HPP 

#include <iostream>
#include <string>
#include <fstream>
#include <map>        		// map

#include <sys/socket.h>		// Sockety
#include <sys/types.h>
#include <netdb.h>     		// struct addrinfo
#include <cstring>     		// memset
#include <unistd.h>    		// close

#include "client.hpp"


namespace server {
	// Nody datovy typ "typ operace"
	enum operation_type_identifer { FOLDER, FILE };
	using operation = operation_type_identifer;

	// Navratove kody
	enum return_code { OKOK = 200, BAD_REQUEST = 400, FORBIDDEN = 403, NOT_FOUND = 404, METHOD_NOT_ALLOWED = 405, SERVER_ERROR = 500 };

	// Trida s nastavenim serveru
	class Options {
	private:
		std::string root_folder;
		int port = 6677;

	protected:

	public:
		void        set_root_folder ( std::string _root_folder ) { root_folder = _root_folder; }
		std::string get_root_folder () const                     { return root_folder; }

		void set_port ( int _port ) { port = _port; }
		int  get_port () const      { return port; }

		// Construktor and destruktor
		 Options (){}
		~Options (){}
	};


	class Client_command {
	public:
		client::client_command type;
		std::string path;
		operation operation_type;
		unsigned long file_length;
		std::vector<char> file;


		// Construktor and destruktor
		 Client_command (){}
		~Client_command (){}
	};

	class Server {
	private:
		const int MAX_CLIENTS = 20;
		int server_socket;

		void answer_client ( int client_socket, return_code code );
		void answer_client ( int client_socket, return_code code, std::fstream &file );
		void answer_client ( int client_socket, return_code code, std::vector<char> &file );

	protected:

	public:
		enum server_errors { OK, OTHER_ERROR };
		Options options;

		// Run and close server
		void run_server   ();
		void close_server ();

		int get_socket ();
		server_errors listen_client ( int client_socket, Client_command &command );
		void do_command ( int client_socket, Client_command &command );

		// Construktor and destruktor
		 Server (){}
		~Server () { close ( server_socket ); }
	};

}
#endif