/* * * * * * * * * *
 * AUTHOR: xmarti76
 * client pro IPK
 * 14. 03. 2017
 * * * * * * * * * */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <fstream>

#include <sys/socket.h>		// Sockety
#include <sys/types.h>
#include <netdb.h>			// struct addrinfo
#include <unistd.h>			// close

#include "server.hpp"

namespace client {
	enum client_command                                {  DELETE,   GET,   PUT,   LST,   MKD,   RMD  , CLIENT_COMMAND_COUNTER };
	static const std::string client_command_string[] = { "DELETE", "GET", "PUT", "LST", "MKD", "RMD" };
	int command_string_to_enum ( std::string command );


	class Options {
	private:
		client_command command;

		std::string remote_path;
		std::string local_path;
		
		int port = 6677;
		std::string address;

		std::fstream file;
		unsigned int file_size = 0;
		
	protected:

	public:
		void           set_command ( client_command _command ) { command = _command; }
		client_command get_command () const                    { return command; }

		void        set_remote_path ( std::string _remote_path ) { remote_path = _remote_path; }
		std::string get_remote_path () const                     { return remote_path; }

		void        set_local_path ( std::string _local_path ) { local_path = _local_path; }
		std::string get_local_path () const                    { return local_path; }

		void set_port ( int _port ) {port = _port; }
		int  get_port () const      { return port; }

		void        set_address ( std::string _address ) { address = _address; }
		std::string get_address () const                 { return address; }

		void prepare_file ();
		std::fstream* get_file () { return &file; }
		unsigned int get_file_size () const { return file_size; }
		// std::fstream get_file () { return file; }
		void close_file () { get_file()->close(); }

		// Construktor and destruktor
		 Options () {}
		~Options () {}
		
	};

	class Client {
	private:
		int client_socket;

	protected:

	public:
		Options options;

		// TODO metody pro praci s file v options

		void connect_server    ();
		void disconnect_server ();

		void sent_command ();
		void listen_answer ();

		int get_socket () const { return client_socket; }

		 Client () {}
		~Client () {}
	};
}

#endif