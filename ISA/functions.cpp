/* * * * * * * * * * * * * * * * * * *
 * AUTOR: Vaclav Martinka (xmarti76)
 * Podpurne funkce
 * 17. 11. 2017 FIT VUT
 * * * * * * * * * * * * * * * * * * */

// #include <cstdio>
#include <string>
#include <sys/stat.h> //is file
// #include <vector>
// #include <regex>

#include "options.hpp"
#include "functions.hpp"
#include "error.hpp"

using namespace std;

bool verbous  = false;
bool quiet    = false;
bool progress = false;

void read_args ( Options &options, unsigned int argc, char *argv[] ) {

	if ( argc > 14 ) {
		exit_error ( "To much of arguments! Use '--help' for help.");
	}

	// Stavy pri cteni argumentu
	enum read_args_state {
		PORT_READ,           PORT_SET,           PORT_UNSET, 
		CERTFILE_READ,       CERTFILE_SET,       CERTFILE_UNSET,
		CERTADDR_READ,       CERTADDR_SET,       CERTADDR_UNSET,
		AUTHENTICATION_READ, AUTHENTICATION_SET, AUTHENTICATION_UNSET,
		OUT_DIR_READ,        OUT_DIR_SET,        OUT_DIR_UNSET
	};
	
	auto port           = PORT_UNSET;
	auto certfile       = CERTFILE_UNSET;
	auto certaddr       = CERTADDR_UNSET;
	auto authentication = AUTHENTICATION_UNSET;
	auto out_dir        = OUT_DIR_UNSET;


	for ( unsigned int i = 1 ; i < argc ; i++ ) {
		string arg (argv[i]);

		if ( arg == "--help" || arg == "-h" ) {
			cout << HELP;
			exit(0);
		}


		else if ( port == PORT_READ ) {
			port = PORT_SET;

			unsigned int tmp_int;
			if ( !str_to_uint( arg, tmp_int ) || tmp_int == 0 ) { exit_error( "The '" + arg + "' isn't valid port!" ); }
			
			options.port( tmp_int );
		}
		else if ( certfile == CERTFILE_READ ) {
			certfile = CERTFILE_SET;

			options.certfile( arg );
		}
		else if ( certaddr == CERTADDR_READ ) {
			certaddr = CERTADDR_SET;

			options.certaddr( arg );
		}
		else if ( authentication == AUTHENTICATION_READ ) {
			authentication = AUTHENTICATION_SET;

			options.authentication( arg );
		}
		else if ( out_dir == OUT_DIR_READ ) {
			out_dir = OUT_DIR_SET;

			options.out_dir( arg );
		}


		else if ( arg == "-p" ) {
			if ( port == PORT_SET ) { exit_error ( "Port is already set!" ); }

			port = PORT_READ;
		}
		else if ( arg == "-c" ) {
			if ( certfile == CERTFILE_SET ) { exit_error( "Certification file is already set!" ); }

			certfile = CERTFILE_READ;
		}
		else if ( arg == "-C" ) {
			if ( certaddr == CERTADDR_SET ) { exit_error( "Certification address is already set!" ); }

			certaddr = CERTADDR_READ;
		}
		else if ( arg == "-a" ) {
			if ( authentication == AUTHENTICATION_SET ) { exit_error( "Authentication file is already set!" ); }

			authentication = AUTHENTICATION_READ;
		}
		else if ( arg == "-o" ) {
			if ( out_dir == OUT_DIR_SET ) { exit_error( "Output directory is already set!" ); }

			out_dir = OUT_DIR_READ;
		}

		else if ( arg == "-T" ) {
			if ( options.ciphering() != options.NONE ) { exit_error( "Ciphering is already set!" ); }

			options.ciphering( options.POP3S );
		}
		else if ( arg == "-S" ) {
			if ( options.ciphering() != options.NONE ) { exit_error( "Ciphering is already set!" ); }

			options.ciphering( options.STLS );
		}

		else if ( arg == "-d" ) {
			options.delete_it( true );
		}
		else if ( arg == "-n" ) {
			options.only_new( true );
		}

		else if ( arg == "-v" ) {
			verbous = true;
		}
		else if ( arg == "-b" ) {
			progress = true;
		}
		else if ( arg == "-q" ) {
			quiet = true;
		}



		else {
			if ( !options.server().empty() )   { exit_error( "You maybe use unknown parameter ('" + arg +"')! Use '--help' for help." ); }
			options.server( arg );
		}
	}

	if (           port == PORT_READ )           { exit_error( "You use '-p' argument but no set the port! Use '--help' for help." ); }
	if (       certfile == CERTFILE_READ )       { exit_error( "You use '-c' argument but no set the certification file! Use '--help' for help." ); }
	if (       certaddr == CERTADDR_READ )       { exit_error( "You use '-C' argument but no set the certification address! Use '--help' for help." ); }
	if ( authentication == AUTHENTICATION_READ ) { exit_error( "You use '-a' argument but no set the authentication file! Use '--help' for help." ); }
	if (        out_dir == OUT_DIR_READ )        { exit_error( "You use '-o' argument but no set the output directory! Use '--help' for help." ); }

	// if ( options.ciphering() != options.NONE && ( certfile == CERTFILE_UNSET || certaddr == CERTADDR_UNSET ) ) {
	// 	exit_error( "You set ciphering ('-T' or '-S' argument) but no set the ciphering files! Use '--help' for help." );
	// }
	if ( options.ciphering() == options.NONE && ( certfile != CERTFILE_UNSET || certaddr != CERTADDR_UNSET ) ) {
		exit_error( "You set the ciphering files but no set ciphering ('-T' or '-S' argument)! Use '--help' for help." );
	}

	if ( options.server().empty() )               { exit_error( "You must set the server adress! Use '--help' for help." ); }
	if ( authentication == AUTHENTICATION_UNSET ) { exit_error( "You must set the authentication file! Use '--help' for help." ); }
	if (        out_dir == OUT_DIR_UNSET )         { exit_error( "You must set the output directory! Use '--help' for help." ); }

	if ( port == PORT_UNSET ) {
		switch ( options.ciphering() ) {
			case options.NONE:  options.port( 110 ); break;
			case options.POP3S: options.port( 995 ); break;
			case options.STLS:  options.port( 110 ); break;
		}
	}
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

// Check if is it folder/file
bool is_file ( std::string file_name ) {
	struct stat buffer;
	return ( ( stat( file_name.c_str(), &buffer ) == 0 ) && ( ( buffer.st_mode & S_IFMT ) == S_IFREG ) ); 
}

bool is_folder ( std::string file_name ) {
	struct stat buffer;
	return ( ( stat( file_name.c_str(), &buffer ) == 0 ) && ( ( buffer.st_mode & S_IFMT ) == S_IFDIR ) ); 
}

std::string MMM2mm ( std::string input ) {
	if ( input == "Jan" ) { return "01"; }
	if ( input == "Feb" ) { return "02"; }
	if ( input == "Mar" ) { return "03"; }
	if ( input == "Apr" ) { return "04"; }
	if ( input == "May" ) { return "05"; }
	if ( input == "Jun" ) { return "06"; }
	if ( input == "Jul" ) { return "07"; }
	if ( input == "Aug" ) { return "08"; }
	if ( input == "Sep" ) { return "09"; }
	if ( input == "Oct" ) { return "10"; }
	if ( input == "Nov" ) { return "11"; }
	if ( input == "Dec" ) { return "12"; }

	return "00";
}

void draw_progress ( float actual, float total ) {
	if ( progress ) {
		int i;
		float part = total / 40;

		out ( "\r[" , "" );
		for ( i = 0 ; i < actual / part ; ++i  ) {
			out ( "#" , "" );
		}
		for ( ; i < 40 ; ++i ) {
			out ( " " , "" );
		}
		out ( actual == total ? "\r                                          \r" : "]" , "" );
	}
}

void replace_all ( std::string &input, std::string from, std::string to ) {
	for ( size_t pos = input.find( from, 0 ) ; pos != string::npos ; pos = input.find( from, pos ) ) {
		input.replace( pos, from.length(), to );
		pos += to.length();
	}
}