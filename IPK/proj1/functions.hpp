/* * * * * * * * * *
 * AUTHOR: xmarti76
 * Functions pro IPK
 * 14. 03. 2017
 * * * * * * * * * */

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP 

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <sys/stat.h>	// is_file()
#include <vector>

#include <stdio.h>		//popen()

#include "server.hpp"
#include "client.hpp"
#include "error.hpp"

const std::string CRLF = "\x0D\x0A";

const std::string SERVER_HELP = {
	"Simple server for basic operations with files and folders.\n"
	"Author: Vaclav Martinka (xmarti76) FIT VUTBR 19. 03. 2017\n\n"
	"Use:\n"
	"\t[-p PORT]           Port where running server, implicit is 6677\n"
	"\t[-r ROOT_FOLDER]    Root folder of server\n"
};
const std::string CLIENT_HELP = {
	"Simple client for basic operations with files and folders.\n"
	"Author: Vaclav Martinka (xmarti76) FIT VUTBR 19. 03. 2017\n\n"
	"Use:\n"
	"\tlst SERVER[:port]/USER/[folder]                  Same as unix ls\n"
	"\trmd SERVER[:port]/USER/[folder]                  Remove empty directory\n"
	"\tmkd SERVER[:port]/USER/[folder]                  Make directory\n"
	"\tdel SERVER[:port]/USER/[folder]/FILE1            Delete file\n"
	"\tput SERVER[:port]/USER/[folder]/FILE1 FILE2      Put FILE2 to server as FILE1\n"
	"\tget SERVER[:port]/USER/[folder]/FILE1 [FILE2]    Get FILE1 from server to FILE2\n"
};


// For server
void read_args ( server::Options &options, unsigned int argc, char *argv[] );
// For client
void read_args ( client::Options &options, unsigned int argc, char *argv[] );

std::string get_time ();

bool call_ls ( std::string path, std::vector<char> &output );

void file_to_string ( std::fstream &file, std::string &output );
void file_to_vector ( std::fstream &file, std::vector<char> &output );

bool is_file   ( std::string file_name );
bool is_folder ( std::string file_name );

void to_html_spaces   ( std::string &path );
void from_html_spaces ( std::string &path );

int count_substring ( std::string input, std::string substring );

#endif