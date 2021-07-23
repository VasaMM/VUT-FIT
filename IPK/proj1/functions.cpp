/* * * * * * * * * *
 * AUTHOR: xmarti76
 * Functions pro IPK
 * 08. 03. 2017
 * * * * * * * * * */

#include "functions.hpp"

using namespace std;

enum read_args_state {ROOT_FOLDER_READ, ROOT_FOLDER_SET, ROOT_FOLDER_UNSET, PORT_READ, PORT_SET, PORT_UNSET};

// Parse adress from string
static void read_address ( string path, client::Options &options ) {
	string tmp_path ( path );
	// Odmazu http na zacatku
	if ( tmp_path.compare( 0, 7, "http://" ) == 0 ) { tmp_path = tmp_path.substr( 7 ); }

	// Nastavim adresu serveru
	int pos = tmp_path.find( '/' );
	if ( pos == -1 ) {
		options.set_address( tmp_path );
		tmp_path = "";
	}
	else {
		options.set_address( tmp_path.substr( 0, pos ) );
		tmp_path = tmp_path.substr( pos );
	}
	
	// Kontrola na port
	if ( (pos = options.get_address().find_last_of( ':' ) ) != -1 ) {
		// Max jednou :
		if ( int( options.get_address().find( ':' ) ) != pos ) { exit_error( "Multiple port declaration!" ); }
		// Kontrola na intiger
		if ( options.get_address().substr( pos + 1, std::string::npos ).find_first_not_of( "0123456789" ) != string::npos ) { exit_error( "Invalid port '" + options.get_address().substr( pos + 1, std::string::npos ) + "'!" ); }
		
		options.set_port( stoi( options.get_address().substr( pos + 1, std::string::npos ) ) );
		options.set_address( options.get_address().substr( 0, pos ) );
	}

	// Pridam http://
	if ( options.get_address() != "localhost" ) { options.set_address( "http://" + options.get_address() ); }
	
	// Parsovani zbytku cesty
	if ( tmp_path.length() ) {
		to_html_spaces( tmp_path );
		options.set_remote_path( '.' + tmp_path );
	}
	else {
		options.set_remote_path( "./" );
	}
}
// Read server args
void read_args (server::Options &options, unsigned int argc, char *argv[]) {

	if ( argc > 5 ) {
		exit_error ( "To much of arguments! Use '--help' for help.");
	}

	auto root_folder_state = ROOT_FOLDER_UNSET;
	auto port_state        = PORT_UNSET;
	
	for ( unsigned int i = 1 ; i < argc ; i++ ) {
		string arg (argv[i]);

		if ( arg == "--help" || arg == "-h" ) {
			cout << SERVER_HELP;
			exit(0);
		}
		else if ( arg == "-r" ) {
			if ( root_folder_state == ROOT_FOLDER_SET ) { exit_error ( "Root folder is already set!" ); }

			root_folder_state = ROOT_FOLDER_READ;
		}
		else if ( arg == "-p" ) {
			if ( port_state == PORT_SET ) { exit_error( "Port is already set!" ); }

			port_state = PORT_READ;
		}
		else if ( root_folder_state == ROOT_FOLDER_READ ) {
			root_folder_state = ROOT_FOLDER_SET;

			// Oprava na / na zacatku a konci
			if (  arg[0] == '.' && arg[1] == '/' ) {
				arg = arg.substr( 2, std::string::npos );
			}
			else if (  arg[0] == '/' ) {
				arg = arg.substr( 1, std::string::npos );
			}
			if ( arg[ arg.length() - 1 ] != '/' ) { arg += '/'; }
			options.set_root_folder( arg );
		}
		else if ( port_state == PORT_READ ) {
			port_state = PORT_SET;

			// Kontrola na intiger
			if ( arg.find_first_not_of( "0123456789" ) != string::npos ) { exit_error( "The '" + arg + "' isn't valid number!" ); }

			options.set_port( stoi( arg ) );
		}
		else {
			exit_error( "Invalid argument '" + arg + "'! Use '--help' for help.");
		}
	}

	if ( root_folder_state == ROOT_FOLDER_READ ) { exit_error( "You use '-r' argument but no set the root path! Use '-help' for help." ); }
	if ( root_folder_state == ROOT_FOLDER_READ ) { exit_error( "You use '-p' argument but no set the port! Use '-help' for help." ); }
}

// Read client args
void read_args (client::Options &options, unsigned int argc, char *argv[]) {

	if ( argc > 4 )      { exit_error( "Too much of arguments! Use '--help' for help."); }
	else if ( argc < 2 ) { exit_error( "Too little of arguments! Use '--help' for help."); }
	
	// Postupne nactu argumenty
	string arg ( argv[1] );
	if ( arg == "--help" || arg == "-h" ) {
		cout << CLIENT_HELP;
		exit(0);
	}
	if ( arg == "del" ) {
		if ( argc != 3 ) exit_error( "You use too much aruments!  Use '--help' for help." );
		
		options.set_command( client::DELETE );
		read_address( argv[2], options );
	}
	else if ( arg == "get" ) {
		options.set_command( client::GET );
		read_address( argv[2], options );
		
		if ( argc == 4 ) { options.set_local_path( argv[3] ); }
	}
	else if ( arg == "put" ) {
		if ( argc != 4 ) exit_error( "You use put, but not set the local path!  Use '--help' for help." );
		
		options.set_command( client::PUT );
		read_address( argv[2], options );		
		options.set_local_path( argv[3] );
	}
	else if ( arg == "lst" ) {
		if ( argc != 3 ) exit_error( "You use too much aruments!  Use '--help' for help." );
		
		options.set_command( client::LST );
		read_address( argv[2], options );
	}
	else if ( arg == "mkd" ) {
		if ( argc != 3 ) exit_error( "You use too much aruments!  Use '--help' for help." );
		
		options.set_command( client::MKD );
		read_address( argv[2], options );
	}
	else if ( arg == "rmd" ) {
		if ( argc != 3 ) exit_error( "You use too much aruments!  Use '--help' for help." );
		
		options.set_command( client::RMD );
		read_address( argv[2], options );
	}
	else {
		exit_error( "Invalid argument '" + arg + "'! Use '--help' for help.");
	}

	options.prepare_file();
}

string get_time () {
	time_t tmp_time;
	time( &tmp_time );
	struct tm *act_time = gmtime ( &tmp_time );

	static const string wday_name[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	static const string mon_name[]  = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	                                   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	// Wed, 22 Feb 2017 15:50:00 CET
	return wday_name[act_time->tm_wday] + ", " +
	       to_string( act_time->tm_mday ) + " " +
	       mon_name[act_time->tm_mon] + " " +
	       to_string( 1900 + act_time->tm_year ) + " " +
	       to_string( act_time->tm_hour ) + ":" +
	       to_string( act_time->tm_min ) + ":" +
	       to_string( act_time->tm_sec ) + " GMT";
}

bool call_ls ( string path, std::vector<char> &output ) {
	FILE *file = NULL;
	string tmp ( "ls '" );
	tmp.append( path );
	tmp.append( "'" );
	if ( ( file = popen( tmp.c_str(), "r" ) ) == NULL )
		return false;

	static const int BUFFER_SIZE = 4098;
	char buffer[ BUFFER_SIZE ];
	
	tmp.clear();
	while ( fgets( buffer, BUFFER_SIZE, file ) != NULL ) {
		tmp.append( buffer );
	}
	if ( pclose( file ) == -1 )
		return false;

	// Oprava /n na konci
	// if ( tmp[ tmp.size() - 1] == '\x0D' ) {		//CR
	// 	tmp.pop_back();	// Odmazu CR
	// 	tmp.pop_back();	// Odmazu LF
	// }
	// else if ( tmp[ tmp.size() ] == '\x0D' || tmp[ tmp.size() - 1] == '\x0A' ) {	// CR || LF
	// 	tmp.pop_back();	// Odmazu CR nebo LF
	// }

	std::copy( tmp.begin(), tmp.end(), std::back_inserter( output ) );
	
	return true;
}

// Read file
void file_to_string ( std::fstream &file, std::string &output ) {
	output.clear();
	output.append( ( std::istreambuf_iterator<char> ( file ) ),
	               ( std::istreambuf_iterator<char> ()     ) );
}

void file_to_vector ( std::fstream &file, std::vector<char> &output ) {
	// output.clear();
	std::copy( std::istreambuf_iterator<char> ( file ),
		       std::istreambuf_iterator<char> (),
		       std::back_inserter( output ) );
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

// Convert spaces to/from %20
void to_html_spaces ( std::string &path ) {
	int pos = 0;
	while ( ( pos = path.find( ' ', pos ) ) != -1 ) {
		path.replace( pos, 1, "%20" );
		pos += 3;
	}
}
void from_html_spaces ( std::string &path ) {
	int pos = 0;
	while ( ( pos = path.find( "%20", pos ) ) != -1 ) {
		path.replace( pos, 3, " " );
		pos += 1;
	}
}


int count_substring ( std::string input, std::string substring ) {
	int pos = input.find( substring );
	int output = 0;

	while ( pos > 0 ) {
		pos = input.find( substring, ++pos );
		output++;
	}

	return output;
}
