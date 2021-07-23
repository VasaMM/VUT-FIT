/* * * * * * * * * * * * * * * * * * *
 * AUTOR: Vaclav Martinka (xmarti76)
 * Podpurne funkce
 * 21. 04. 2017 FIT VUT
 * * * * * * * * * * * * * * * * * * */

#include <cstdio>
#include <string>
#include <vector>
#include <regex>

#include "options.hpp"
#include "functions.hpp"
#include "error.hpp"

using namespace std;


void read_args ( Options &options, unsigned int argc, char *argv[] ) {

	if ( argc > 11 ) {
		exit_error ( "To much of arguments! Use '--help' for help.");
	}
	else if ( argc < 2 ) {
		exit_error ( "To little of arguments! Use '--help' for help.");
	}

	// Stavy pri cteni argumentu
	enum read_args_state {
		FIRST_TTL_READ, FIRST_TTL_SET, FIRST_TTL_UNSET,
		  MAX_TTL_READ,   MAX_TTL_SET,   MAX_TTL_UNSET,
		  TIMEOUT_READ,   TIMEOUT_SET,   TIMEOUT_UNSET,
		     PORT_READ,      PORT_SET,      PORT_UNSET
	};
	
	auto first_ttl = FIRST_TTL_UNSET;
	auto max_ttl   =   MAX_TTL_UNSET;
	auto timeout   =   TIMEOUT_UNSET;
	auto port      =      PORT_UNSET;
	
	for ( unsigned int i = 1 ; i < argc ; i++ ) {
		string arg (argv[i]);

		if ( arg == "--help" || arg == "-h" ) {
			cout << HELP;
			exit(0);
		}
		else if ( arg == "-f" ) {
			if ( first_ttl == FIRST_TTL_SET ) { exit_error ( "First TTL is already set!" ); }

			first_ttl = FIRST_TTL_READ;
		}
		else if ( arg == "-m" ) {
			if ( max_ttl == MAX_TTL_SET ) { exit_error( "Max TTL is already set!" ); }

			max_ttl = MAX_TTL_READ;
		}
		else if ( arg == "-t" ) {
			if ( timeout == TIMEOUT_SET ) { exit_error( "Timeout is already set!" ); }

			timeout = TIMEOUT_READ;
		}
		else if ( arg == "-p" ) {
			if ( port == PORT_SET ) { exit_error( "Port is already set!" ); }

			port = PORT_READ;
		}
		else if ( arg == "--ptr" ) {
			options.ptr( options.YES );
		}
		else if ( arg == "--no-ptr" ) {
			options.ptr( options.NO );
		}
		else if ( first_ttl == FIRST_TTL_READ ) {
			first_ttl = FIRST_TTL_SET;

			unsigned int tmp_int;
			if ( !str_to_uint( arg, tmp_int ) || tmp_int == 0 ) { exit_error( "The '" + arg + "' isn't valid number!" ); }
			if ( options.first_ttl() > options.max_ttl() )      { exit_error( "The first TTL (" + to_string( options.first_ttl() ) + ") must be lower than the max TTL (" + to_string( options.max_ttl() ) + ")" ); }
			
			options.first_ttl( tmp_int );
		}
		else if ( max_ttl == MAX_TTL_READ ) {
			max_ttl = MAX_TTL_SET;

			unsigned int tmp_int;
			if ( !str_to_uint( arg, tmp_int ) || tmp_int == 0 ) { exit_error( "The '" + arg + "' isn't valid number!" ); }
			if ( options.max_ttl() < options.first_ttl() )      { exit_error( "The max TTL (" + to_string( options.first_ttl() ) + ") must be higher than the first TTL (" + to_string( options.max_ttl() ) + ")" ); }
			
			options.max_ttl( tmp_int );
		}
		else if ( timeout == TIMEOUT_READ ) {
			timeout = TIMEOUT_SET;

			unsigned int tmp_int;
			if ( !str_to_uint( arg, tmp_int ) || tmp_int == 0 ) { exit_error( "The '" + arg + "' isn't valid number!" ); }
			
			options.timeout( tmp_int );
		}
		else if ( port == PORT_READ ) {
			port = PORT_SET;

			unsigned int tmp_int;
			if ( !str_to_uint( arg, tmp_int ) || tmp_int == 0 ) { exit_error( "The '" + arg + "' isn't valid number!" ); }
			
			options.port( tmp_int );
		}
		else {
			if ( !options.address().empty() )   { exit_error( "You already set the host adress! Use '--help' for help." ); }
			options.address( arg );
		}
	}

	if ( first_ttl == FIRST_TTL_READ ) { exit_error( "You use '-f' argument but no set the first TTL! Use '--help' for help." ); }
	if (   max_ttl == MAX_TTL_READ )   { exit_error( "You use '-m' argument but no set the max TTL! Use '--help' for help." ); }
	if (   timeout == TIMEOUT_READ )   { exit_error( "You use '-t' argument but no set the timeout! Use '--help' for help." ); }
	if (      port == PORT_READ )      { exit_error( "You use '-p' argument but no set the port! Use '--help' for help." ); }
	if ( options.address().empty() )   { exit_error( "You must set the host adress! Use '--help' for help." ); }
}


bool str_to_uint ( string &input, unsigned int &output ) {
	if ( input.find_first_not_of( "0123456789" ) != string::npos ) { return false; }

	output = stoi( input );
	return true;
}

type address_type ( string input ) {
	// Velmi primitivni rozdeleni bez jakekoli validace
	if ( input.find_first_not_of( "0123456789." ) != string::npos ) {
		if ( input.find_first_not_of( "0123456789abcdefABCDEF:" ) != string::npos ) {
			return HOSTNAME;
		}
		return IPV6;
	}
	else {
		return IPV4;
	} 
}
