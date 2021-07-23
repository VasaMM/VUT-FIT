/**
 * @file Options.cpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (15:22)
 * @brief Implementace objektu \c Options a jeho metod
 */


#include <string>

#include "Info.hpp"
#include "Options.hpp"


using namespace std;
using namespace Coder;


/// Vypíše nápovědu a ukončí program
void help() {
	cout << HELP;
	exit( 0 );
}


Options::Options ( int argc, char* argv[] ) {
	// Stavy pri cteni argumentu
	enum Status {
		READ, SET, UNSET,
	};
	
	Status huffman = UNSET;
	Status input   = UNSET;
	Status output  = UNSET;
	Status width   = UNSET;

	bool print_help = false;

	for ( auto i = 1 ; i < argc ; ++i ) {

		string arg( argv[i] );

		// Napoveda a verze
		if ( arg == "--help" ) {
			help();
		}
		else if ( arg == "--version" ) {
			cout << VERSION << endl;
			exit(0);
		}


		// Parametry s argumentem - cteni argumentu
		else if ( huffman == READ ) {
			huffman = SET;
			if ( arg == "static" ) {
				this->huffman_mode( STATIC );
				print_help = false;
			}
			else if ( arg == "adaptive" ) {
				this->huffman_mode( ADAPTIVE );
				print_help = false;
			}
			else if ( print_help ) {
				help();
			}
			else {
				exit_error( "You set unknown Huffman mode: '" + arg + "'!" );
			}
		}
		else if ( input == READ ) {
			input = SET;
			this->input( arg );
		}
		else if ( output == READ ) {
			output = SET;
			this->output( arg );
		}
		else if ( width == READ ) {
			continue;
		}


		// Parametry s argumentem
		else if ( arg == "-h" || arg == "--huffman" ) {
			if ( arg == "-h" ) { print_help = true; }
			huffman = READ;
		}
		else if ( arg == "-i" || arg == "--input" ) {
			input = READ;
		}
		else if ( arg == "-o" || arg == "--output" ) {
			output = READ;
		}
		else if ( arg == "-w" || arg == "--width" ) {
			width = READ;
		}


		// Ostatni parametry
		else if ( arg == "-c" || arg == "--compression" ) {
			mode( COMPRESSION );
		}
		else if ( arg == "-d" || arg == "--decompression" ) {
			mode( DECOMPRESSION );
		}


		else if ( arg == "-m" || arg == "--model" ) {
			model( true );
		}


		else if ( arg == "-v" || arg == "--verbous" ) {
			info.set_verbous();
		}
		else if ( arg == "-q" || arg == "--quiet" ) {
			info.set_quiet();
		}


		else {
			exit_error( "You use unknown parameter '" + arg +"'! Use '--help' for help." );
		}
	}

	if ( print_help ) { help(); }


	// Kontrola parametru s argumentem
	if (   input == READ ) { exit_error( "You use '-i' argument but no set the input file! Use '--help' for help." ); }
	if (  output == READ ) { exit_error( "You use '-o' argument but no set the output file! Use '--help' for help." ); }


	// Kontrola povinych argumentu
	if ( input == UNSET ) {
		exit_error( "You must set the input file! Use '--help' for help." );
	}

}

