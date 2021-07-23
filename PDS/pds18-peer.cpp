/**
 * @file pds18-peer.cpp
 * @author Václav Matinka
 * @date 26. 04. 2019 (09:11)
 * @brief Main funkce pro \c peer
 */


#include <omp.h>
#include <signal.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Info.hpp"
#include "Options.hpp"
#include "Destination.hpp"
#include "Info.hpp"
#include "Exception.hpp"
#include "Peer.hpp"
#include "pds18-rpc.hpp"
#include "parser.hpp"


using namespace std;



/// Velikost bufferu pro čten RPC zpráv
#define RPC_BUFFER_SIZE 32768


/// Perioda zasílání \c hello zprávy (v sekundách)
const uint HELLO_TIME = 10;

/// Perioda úklidu v databázi (v sekundách)
const uint CLEAN_TIME = 1;

/// Booleovská proměnná pro ukončení programu
bool running = true;


/**
 * @brief Ukončí program
 * @param s ID signálu (nepoužito)
 */
void end( int s ) {
	running = false;
	#pragma omp flush(running)
}


/**
 * @brief Zpracování RPC volání
 * 
 * @param o Reference na nastavení
 * @param p Reference na \c Peer
 */
void rpc( Options &o, Peer &p );


/**
 * @brief Main funkce
 * 
 * @param argc Počet předanýcg arguentů
 * @param argv Hodnoty předaných argumentů
 * 
 * @return Návratová hodnota (0 = OK)
 */
int main ( int argc, char *argv[] ) {
	Options o( argc, argv, Options::PEER );

	info.say( "Start on " + o.chat().str() );
	info.say( "Server   " + o.reg().str() );


	// Navazu ukoncovaci signal
	signal( SIGINT, end );

	// Nastavim maximalni pocet vlaken
	omp_set_num_threads(24);

	// Vytvorim objekt peera
	Peer p( o, &running );
	
	// Nastartuji paralelni sekce
	#pragma omp parallel sections shared(running)
	{
		// Sekce na zpracovani RPC
		#pragma omp section
		{
			rpc( o, p );
			info.say( "Peer " + to_string( o.id() ) + " RPC terminated." );
		}
		

		// Sekce pro pravidelne cinnosti
		//  * odesilani HELLO
		//  * uklid v databazich
		#pragma omp section
		{
			double helloTime = 0;
			double cleanTime = CLEAN_TIME;
			while ( running ) {
				if( omp_get_wtime() > helloTime ) {
					p.sendHello();
					helloTime = omp_get_wtime() + HELLO_TIME;
				}

				if( omp_get_wtime() > cleanTime ) {
					p.clean();
					cleanTime = omp_get_wtime() + CLEAN_TIME;
				}

				usleep( 10 );
				#pragma omp flush(running)
			}

			p.sendBye();

			info.say( "Peer " + to_string( o.id() ) + " HELLO & cleaning terminated." );
		}


		// Prijem dat
		#pragma omp section
		{
			bool ready = true;
			while ( running ) {
				Destination dest;
				vector<char> msg = p.read( &dest );

				// Prijal jsem nova data
				if ( msg.size() ) {
					ready = false;

					#pragma omp task shared(ready)
					{
						// Musim si vytvorit kopii bufferu
						vector< char > msgCopy = vector< char >( msg );
						Destination answerDest = Destination( dest );
						
						// Mam kopii, muzu pokracovat
						ready = true;
						#pragma omp flush(ready)
						
						info.say( "From " + answerDest.str() + " receive: " + msgCopy.data() );

						try {
							MsgData msgData = parse( get<dict>( decode( msgCopy.data() ) ) );
							
							// Zpracuji zpravu
							switch ( msgData.type ) {
								case ACK:
									p.receiveAck( msgData.txid );
									break;


								case LIST:
									// Kontrola, ze je zprava ze serveru
									if ( answerDest != o.reg() ) { throw Exception( UNKNOWN_DESTINATION, "List message from unknown destination '" + answerDest.str() + "'" ); }
									
									p.receiveList( msgData.data );
									p.sendAck( msgData.txid, answerDest );
									break;


								case MESSAGE:
									p.sendAck( msgData.txid, answerDest );

									cout << "+----------------------------------------" << endl;
									cout << "| MESSAGE from " << msgData.username << " (" + answerDest.str() +")" << endl;
									cout << "| " << msgData.msg << endl;
									break;


								case ERROR:
									print_error( "Received error with txid " + to_string( msgData.txid ) + "\n" + msgData.msg );
									break;


								default:
									throw Exception( MSG_INVALID_TYPE, "Invalid message type '" + MsgTypeString[ msgData.type ] + "'" );
							}
						}
						catch ( Exception &e ) {
							print_error( e.what() );
							print_error( string( "Message from " ) + answerDest.str() + "\n" + string( msgCopy.data() ) );
						}
						catch ( ... ) {
							print_error( "Invalid message" );
							print_error( string( "Message from " ) + answerDest.str() + "\n" + string( msgCopy.data() ) );
						}
					}

					// Pockam na vytvoreni kopie
					while( !ready ) {
						usleep( 10 );
						#pragma omp flush(ready)
					}
				}
	
				#pragma omp flush(running)
			}
			
			info.say( "Peer " + to_string( o.id() ) + " receiving messages terminated." );
		}
	}
	
	info.say( "Peer " + to_string( o.id() ) + " terminated." );
	return 0;
}



void rpc( Options &o, Peer &p ) {
	int input = preparePipe( RPC_PEER, o.id() );

	string buffer;
	char *bufferPointer;
	int readed;
	long long totalReaded;

	while ( running ) {
		readed = 0;
		totalReaded = 0;
		
		// Zkusim nacist data z pipeline
		do {
			buffer.resize( buffer.size() + RPC_BUFFER_SIZE );
			bufferPointer = buffer.data() + totalReaded;

			readed = read( input, bufferPointer, RPC_BUFFER_SIZE - 1 );
			totalReaded += readed;

		} while( readed == RPC_BUFFER_SIZE );
	
		// Nenacetl jsem nic, zkusim STDIN
		if ( totalReaded <= 0 ) {
			totalReaded = 0;
			
			buffer.clear();
			do {
				buffer.resize( buffer.size() + RPC_BUFFER_SIZE );
				bufferPointer = buffer.data() + totalReaded;

				readed = read( STDIN_FILENO, bufferPointer, RPC_BUFFER_SIZE - 1 );
				totalReaded += readed;
			} while( readed == RPC_BUFFER_SIZE );
		}

		// Nacetl jsem, muzi to zpracovat
		if ( totalReaded > 0 ) {
			buffer.resize( totalReaded );

			// Kazdy prikaz musi zacinat lomitkem
			if ( buffer[0] == '\\' ) {
				// a koncit mezerou
				auto pos = buffer.find( " " );
				// nebo novym radkem
				if ( pos == string::npos ) {
					pos = buffer.find( "\n" );
				}

				if ( pos != string::npos ) {
					// Nactu prikaz
					string commandStr = buffer.substr( 1, pos - 1 );

					if ( commandStr == "help" || commandStr == "h" ) {
						cout << "\\[g]etlist \\[p]eers \\[m]essage \\[r]econnect \\[q]uit" << endl;
						continue;
					}

					// Nactu pripdne parametry a vykonam prikaz
					try {
						Txid id;
						string from, to, message, ipv4, port;
						switch ( decodeString2Command( commandStr ) ) {
							case RPC_MESSAGE:
								buffer = buffer.substr( pos + 1 );

								from = buffer.substr( 0, buffer.find( " " ) );
								buffer = buffer.substr( from.size() + 1 );
								
								to = buffer.substr( 0, buffer.find( " " ) );
								buffer = buffer.substr( to.size() + 1 );
								
								message = buffer.substr( 0, buffer.rfind( "\n" ) );

								if ( ( id = p.sendMsg( from, to, message ) ) == NO_ID ) {
									print_error( "Sending message for '" + to + "' was unsuccessful." );
								}
								else if ( ! p.waitAck( id ) ) {
									print_error( "The message for '" + to + "' has been sent but has not been confirmed." );
								}
								break;
								

							case RPC_GETLIST:
								id = p.sendGetlist();
								if ( ! p.waitAck( id ) ) {
									print_error( "The 'GETLIST' has been sent but has not been confirmed." );
								}

								if ( ! p.waitList() ) {
									print_error( "Failed to get list." );
								}
								break;
							

							case RPC_PEERS:
								if ( ! p.printList() ) {
									print_error( "Failed to print list." );
								}
								break;

							
							case RPC_RECONNECT:
								buffer = buffer.substr( pos + 1 );

								ipv4 = buffer.substr( 0, buffer.find( " " ) );
								buffer = buffer.substr( ipv4.size() + 1 );
								
								port = buffer.substr( 0, buffer.rfind( "\n" ));

								p.sendBye();
								p.updateServer( Destination( ipv4, port ) );
								o.reg( Destination( ipv4, port ) );
								break;


							case RPC_QUIT:
								running = false;
								#pragma omp flush(running)
								break;


							default:
								print_error( "Invalid RPC '" + commandStr + "'" );
						}
					}
					catch ( out_of_range &e ) {
						print_error( "Invalid parametres for RPC '" + commandStr + "'" );		
					}
				}
				else {
					print_error( "Unknown RPC '" + buffer + "'. All RPCs must be ended by '\\n'" );
				}
			}
			else {
				print_error( "Unknown RPC '" + buffer + "'. All RPCs must start by \\." );
			}
		}

		usleep( 10 );
		#pragma omp flush(running)
	}

	closePipe( input, RPC_PEER, o.id() );
}
