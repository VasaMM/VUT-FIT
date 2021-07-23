/* * * * * * * * * * * * * * * * * * *
 * AUTOR: Vaclav Martinka (xmarti76)
 * Podpurne funkce
 * 17. 11. 2017 FIT VUT
 * * * * * * * * * * * * * * * * * * */

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP 

// #include <netinet/ip_icmp.h>
#include <string>
#include <iostream>

#include "options.hpp"

extern bool verbous;
extern bool quiet;
extern bool progress;


const std::string HELP = {
	"\n"
	"Simple POP3 client\n"
	"Author: Vaclav Martinka (xmarti76)\t\t20. 11. 2017\t\tFIT VUT\n\n"
	
	"Available options:\n"
	"  -h, --help        Display this help and exit.\n\n"

	"  [-p <port>]       Server port for communication, default value is 110.\n"
	"  [-T|-S]           '-T'  Enable ciphering whole communication (POP3S),\n"
	"                    it changes default port to 995.\n"
	"                    '-S'  Start unencrypted connection and after then uses\n"
	"                          the STLS command to switch on encrypted protocol.\n"
	"                    If you use '-T' or '-S' you must use '-c', '-C' or both\n"
	"  [-c <certfile>]   File with certificate.\n"
	"  [-C <certaddr>]   Direction with certificates.\n"
	"  [-n]              Download only news messages (messages, which isn't in output direction).\n"
	"  [-d]              Delete messages on server after downloading. If you download\n"
	"                    only news messages ('-n'), so only news messages will be downloaded.\n\n"
	"  [-v]              Verbous, progress reports will be displayed.\n"
	"  [-b]              Progress bar will be displayed.\n"
	"  [-q]              Quiet, nothing display (only errors on stderr).\n"
	
	"  -a <auth_file>    Authentication file with user name and password\n"
	"                    Format:\n"
	"                       username = <name>\n"
	"                       password = <pass>\n"
	"  -o <out_dir>      Output direction, where messages will be saved.\n"
	"  <server>          Address of POP3 server. Address name or IP address.\n\n\n"
};

void read_args ( Options &, unsigned int argc, char *argv[] );

// String to unsigned int
bool str_to_uint ( std::string &input, unsigned int &output );

enum type { IPV4, IPV6, HOSTNAME };
type address_type ( std::string input );

bool is_file   ( std::string file_name );
bool is_folder ( std::string file_name );

// Convert verbal month to number MMM -> MM
std::string MMM2mm ( std::string );

// Quiet
inline void out ( std::string data, std::string end = "\n" ) {
	if ( ! quiet ) {
		std::cout << data << end << std::flush;
	}
}

// Verbous
inline void say ( std::string data, std::string end = "\n" ) {
	if ( verbous ) {
		out( data, end );
	}
}

// Draw simple progress bar
void draw_progress ( float actual, float total );

// Replace all substrings to new string
void replace_all ( std::string &input, std::string from, std::string to );

#endif
