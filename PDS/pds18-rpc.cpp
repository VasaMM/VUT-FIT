/**
 * @file pds18-rpc.cpp
 * @author Václav Matinka
 * @date 21. 04. 2019 (11:59)
 * @brief Implementace RPC pro \c peera i \c noda
 */


#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "Info.hpp"
#include "pds18-rpc.hpp"

using namespace std;


/// Vypíše nápovědu
void help();


/**
 * @brief Načte a zpracuje zadané argumenty
 * 
 * @param argc Počet argumentů (argc z main)
 * @param argv Pole argumentů (argv z main)
 * 
 * @return Struktura s načtenými daty
 */
Params parseArgs( int argc, char *argv[] );



/**
 * @brief Main funkce
 * 
 * @param argc Počet argumentů
 * @param argv Pole argumentů
 * 
 * @return Úspšnost
 */
int main( int argc, char *argv[] ) {
	// Zpracuji arumenty
	Params p = parseArgs( argc, argv );

	// Cesta k pojmenované rouře
	string pipe =  "/tmp/" + string( p.target == RPC_NODE ? "node" : "peer" ) + "_" + to_string( p.id );

	// Vytvořím odesílaný příkaz
	string command = "\\" + string( commandString[ p.command ] );

	switch ( p.command ) {
		case RPC_MESSAGE:    command += " " + p.from + " " + p.to   + " " + p.msg + "\n";  break;
		case RPC_RECONNECT:  command += " " + p.ipv4 + " " + p.port + "\n";  break;
		case RPC_CONNECT:    command += " " + p.ipv4 + " " + p.port + "\n";  break;
		default:             command += "\n";  break;
	}


	// Otevřu rouru, odešlu data, zavřu ji a ukončím program
    int output = open( pipe.c_str(), O_WRONLY );
    write( output, command.c_str(), command.size() );
    close( output );

    return 0;
}



void help() {
	cout << RPC_HELP;
	exit( 0 );
}


Params parseArgs( int argc, char *argv[] ) {
	Params p = {
		0,             // id
		RPC_UNSET,     // target
		RPC___END__,   // command
		"",            // from
		"",            // to
		"",            // msg
		"",            // ipv4
		"",            // port
	};

	for ( auto i = 1 ; i < argc ; ++i ) {
		string arg( argv[i] );

		// Napoveda
		if ( arg == "-h" || arg == "--help" ) {
			help();
		}

		else if ( arg == "-i" || arg == "--id" ) {
			if ( argc <= ++i ) {
				exit_error( "You use '" + arg + "', but not defined the ID! Use '--help' for help." );
			}
		
			try {
				p.id = stoul( argv[ i ] );
			}
			catch ( std::invalid_argument &e ) {
				exit_error( "You set invalid ID '" + string( argv[ i ] ) + "'! Use '--help' for help." );
			}
		}

		else if ( arg == "-c" || arg == "--command" ) {
			if ( argc <= ++i ) {
				exit_error( "You use '" + arg + "', but not defined the command! Use '--help' for help." );
			}

			p.command = decodeString2Command( argv[ i ] );
			switch ( p.command ) {
				case RPC_MESSAGE:
					if ( argc <= i + 6 ) {
						exit_error( "You use command 'message', but not defined parametres! Use '--help' for help." );
					}

					for ( uint j = 0; j < 3; ++j ) {
						string arg( argv[++i] );
						if ( arg == "--from" ) {
							p.from = argv[++i];
						}
						else if ( arg == "--to" ) {
							p.to = argv[++i];
						}
						else if ( arg == "--message" ) {
							p.msg = argv[++i];
						}
						else {
							exit_error( "You use unknown parametr '" + arg + "'! Use '--help' for help." );
						}
					}

					break;

				case RPC_RECONNECT:
					if ( argc <= i + 4 ) {
						exit_error( "You use command 'reconnect', but not defined parametres! Use '--help' for help." );
					}

					for ( uint j = 0; j < 2; ++j ) {
						string arg( argv[++i] );
						if ( arg == "--reg-ipv4" ) {
							p.ipv4 = argv[++i];
						}
						else if ( arg == "--reg-port" ) {
							p.port = argv[++i];
						}
						else {
							exit_error( "You use unknown parametr '" + arg + "'! Use '--help' for help." );
						}
					}

					break;

				case RPC_CONNECT:
					if ( argc <= i + 4 ) {
						exit_error( "You use command 'connect', but not defined parametres! Use '--help' for help." );
					}

					for ( uint j = 0; j < 2; ++j ) {
						string arg( argv[++i] );
						if ( arg == "--reg-ipv4" ) {
							p.ipv4 = argv[++i];
						}
						else if ( arg == "--reg-port" ) {
							p.port = argv[++i];
						}
						else {
							exit_error( "You use unknown parametr '" + arg + "'! Use '--help' for help." );
						}
					}

					break;

				case RPC___END__:
					exit_error( "You set invalid command '" + string( argv[ i ] ) + "'! Use '--help' for help." );

				default: break;
			}

		}

		else if ( arg == "-p" || arg == "--peer" ) {
			if ( p.target != RPC_UNSET ) {
				exit_error( "You may not combine peer and node! Use '--help' for help." );
			}
			p.target = RPC_PEER;
		}

		else if ( arg == "-n" || arg == "--node" ) {
			if ( p.target != RPC_UNSET ) {
				exit_error( "You may not combine node and peer! Use '--help' for help." );
			}
			p.target = RPC_NODE;
		}
		
		else {
			exit_error( "You use unknown parameter '" + arg +"'! Use '--help' for help." );
		}
	}

	if ( p.target == RPC_UNSET ) {
		exit_error( "You must set the target (peer/node)! Use '--help' for help." );
	}

	// Zkontroluji validnost pouziti
	switch ( p.command ) {
		case RPC_QUIT:
			break;

		case RPC_MESSAGE:
		case RPC_GETLIST:
		case RPC_PEERS:
		case RPC_RECONNECT:
			if ( p.target != RPC_PEER ) {
				exit_error( "You may not combine command '" + commandString[ p.command ] + "' with node! Use '--help' for help." );
			}
			break;

		case RPC_DATABASE:
		case RPC_NEIGHBORS:
		case RPC_CONNECT:
		case RPC_DISCONNECT:
		case RPC_SYNC:
			if ( p.target != RPC_NODE ) {
				exit_error( "You may not combine command '" + commandString[ p.command ] + "' with peer! Use '--help' for help." );
			}
			break;

		case RPC___END__:
			exit_error( "You must set the command! Use '--help' for help." );
	}

	return p;
}
