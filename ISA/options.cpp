/* * * * * * * * * * * * * * * * * * *
 * AUTOR: Vaclav Martinka (xmarti76)
 * Struktura pro uchovani informaci
 * 17. 11. 2017 FIT VUT
 * * * * * * * * * * * * * * * * * * */

#include <fstream>
#include <iostream>
#include <string>
#include <regex>


#include "options.hpp"
#include "functions.hpp"
#include "error.hpp"

using namespace std;


bool Options::parse_user_file () {
	say( authentication() );
	if ( is_file( authentication() ) ) {
		ifstream file( authentication() );

		if ( file.is_open() ) { 
			regex user ("^username[ \\t]*=[ \\t]*(.+)");
			regex pass ("^password[ \\t]*=[ \\t]*(.+)");
			smatch match;

			for( string line ; getline( file, line ) ; ) {
				// Username
				if ( regex_search ( line, match, user ) ) { name( match[1] ); }

				// Password
				if ( regex_search ( line, match, pass ) ) { password( match[1]); }
			}
			
			file.close();
			return true;
		}
	}
	
	return false;
}

void Options::out_dir ( std::string data ) {
	// Otestuji vystupni slozku
	if ( ! is_folder( data ) ) { exit_error( "Output direction '" + data + "' isn't exists!" ); }

	// Opravim cestu
	if ( data[ data.size() ] != '/' ) {
		data += '/';
	}

	out_dir_ = data;
}

