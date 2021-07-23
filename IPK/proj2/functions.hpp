/* * * * * * * * * * * * * * * * * * *
 * AUTOR: Vaclav Martinka (xmarti76)
 * Podpurne funkce
 * 21. 04. 2017 FIT VUT
 * * * * * * * * * * * * * * * * * * */

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP 

#include <netinet/ip_icmp.h>

#include "options.hpp"

const std::string HELP = {
	"\n"
	"Simple traceroute for network diagnostic.\n"
	"Author: Vaclav Martinka (xmarti76)\t\t21. 04. 2017\t\tFIT VUT\n\n"
	
	"Available options:\n"
	"\t-h, --help          Display this help and exit\n\n"

	"\t[-f <first_ttl>]    First number of hop, default is 1\n"
	"\t[-m <max_ttl>]      Maximum number of hops, default is 30\n"
	"\t[-t <timeout>]      Timeout for one hop in seconds, default is 3 s\n"
	"\t[-p <port>]         Host port for communication, default value is from range 49 152 - 65 535\n"
	"\t[--ptr]             Turns on the address translation to hostname\n"
	"\t[--no-ptr]          Turns off the address translation to hostname\n\n"
	
	"\t<destination>       Destination for diagnostic. If you set it as hostname, all hops will be translated to their hostname\n\n\n"
};

void read_args ( Options &, unsigned int argc, char *argv[] );

// String to unsigned int
bool str_to_uint ( std::string &input, unsigned int &output );

enum type { IPV4, IPV6, HOSTNAME };
type address_type ( std::string input );

#endif