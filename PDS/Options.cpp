/**
 * @file Options.cpp
 * @author Václav Matinka
 * @date 21. 04. 2019 (13:03)
 * @brief Implementace objektu \c Options a jeho metod
 */

#include <vector>
#include <string>

#include "Info.hpp"
#include "Options.hpp"


/// Otestuje, zda byl načten parametr
#define TEST_LOADED()  if ( loaded ) { loaded = false; continue; }

/// Nastaví načtení parametru
#define SET_LOADED()   loaded = true; break;

/// Nutno volat před čtením parametrů
#define LOADED_INIT()  bool loaded = false;


using namespace std;


void Options::help() {
	if ( _type == PEER ) {
		cout << PEER_HELP;
	}
	else if ( _type == NODE ) {
		cout << NODE_HELP;
	}
	exit( 0 );
}


Options::Options( int const argc, char** const argv, ProgramType const type ) {
	_type = type;

	// Stavy pri cteni argumentu
	enum Status {
		READ, SET, UNSET,
	};

	const string paramsStrings[] = {
		"id",
		"username",
		"chat-ipv4",
		"chat-port",
		"reg-ipv4",
		"reg-port",
	};
	

	enum Params { ID, USERNAME, CHAT_IPV4, CHAT_PORT, REG_IPV4, REG_PORT, __END__ };

	Status paramsStatus[ __END__ ];
	for ( int i = ID ; i < __END__; ++i ) {
		paramsStatus[ i ] = UNSET;
	}
	
	LOADED_INIT()

	for ( auto i = 1 ; i < argc ; ++i ) {
		string arg( argv[i] );

		// Napoveda
		if ( arg == "-h" || arg == "--help" ) {
			help();
		}

		for ( int i = ID ; i < __END__ ; ++i ) {
			if ( paramsStatus[ i ] == READ ) {
				paramsStatus[ i ] = SET;
	
				switch ( i ) {
					case ID:        this->id( arg );           break;
					case USERNAME:  this->username( arg );     break;
					case CHAT_IPV4: this->chat().ipv4( arg );  break;
					case CHAT_PORT: this->chat().port( arg );  break;
					case REG_IPV4:  this->reg().ipv4( arg );   break;
					case REG_PORT:  this->reg().port( arg );   break;

					default: exit_error( "Unknown argument: '" + arg + "'!" );
				}

				SET_LOADED()
			}
		}

		TEST_LOADED()

		// Parametry s argumentem
		for ( int i = ID ; i < __END__ ; ++i ) {
			if ( arg == "--" + paramsStrings[ i ] ) {
				paramsStatus[ i ] = READ;
				SET_LOADED()
			}
		}

		TEST_LOADED()

		// Ostatni parametry
		if ( arg == "-v" || arg == "--verbous" ) {
			info.set_verbous();
		}
		else if ( arg == "-q" || arg == "--quiet" ) {
			info.set_quiet();
		}


		else {
			exit_error( "You use unknown parameter '" + arg + "'! Use '--help' for help." );
		}
	}


	// Kontrola parametru s argumentem
	for ( int i = ID ; i < __END__ ; ++i ) {
		if ( paramsStatus[ i ] == READ ) {
			exit_error( "You use '--" + paramsStrings[ i ] + "' argument but no set value! Use '--help' for help." );
		}
	}


	if ( type == NODE ) {
		vector< Params > onlyPeer = { USERNAME, CHAT_IPV4, CHAT_PORT };

		for ( size_t i = 0 ; i < onlyPeer.size() ; ++i ) {
			if ( paramsStatus[ onlyPeer[ i ] ] != UNSET ) {
				exit_error( "You use unsupported parameter '" + paramsStrings[ i ] + "'! Use '--help' for help." );
			}
		}

	}
}

