/**
 * @file pds18-node.cpp
 * @author Václav Matinka
 * @date 23. 04. 2019 (19:23)
 * @brief Main funkce pro \c node
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
#include "Node.hpp"
#include "pds18-rpc.hpp"


using namespace std;



/// Velikost bufferu pro čten RPC zpráv
#define RPC_BUFFER_SIZE 32768


/// Perioda úklidu v databázi (v sekundách)
const uint CLEAN_TIME = 1;

/// Perioda odesílání \c UPDATE zprávy (v sekundách)
const uint UPDATE_TIME = 3;


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
 * @param n Reference na \c Node
 */
void rpc( Options &o, Node &n );



/**
 * @brief Main funkce
 * 
 * @param argc Počet předanýcg arguentů
 * @param argv Hodnoty předaných argumentů
 * 
 * @return Návratová hodnota (0 = OK)
 */
int main ( int argc, char *argv[] ) {
	Options o( argc, argv, Options::NODE );

	info.say( "Start on " + o.reg().str() );


	// Navazu ukoncovaci signal
	signal( SIGINT, end );

	// Nastavim maximalni pocet vlaken
	omp_set_num_threads(24);

	// Vytvorim objekt noda
	Node n( o, &running );

	// Nastartuji paralelni sekce
	#pragma omp parallel sections shared(running)
	{
		// Sekce na zpracovani RPC
		#pragma omp section
		{
			rpc( o, n );
			info.say( "Peer " + to_string( o.id() ) + " RPC terminated." );
		}



		// Sekce pro pravidelne cinnosti
		//  * zasilani UPDATE zprav
		//  * uklid v databazich
		#pragma omp section
		{
			double cleanTime = 0;
			double updateTime = 0;
			while ( running ) {
				if( omp_get_wtime() > cleanTime ) {
					n.clean();
					cleanTime = omp_get_wtime() + CLEAN_TIME;
				}

				if( omp_get_wtime() > updateTime ) {
					n.sendUpdate();
					updateTime = omp_get_wtime() + UPDATE_TIME;
				}

				usleep( 1000 );
				#pragma omp flush(running)
			}

			n.disconnect();
			info.say( "Node " + to_string( o.id() ) + " UPDATE & cleaning terminated." );
		}



		// Prijem dat
		#pragma omp section
		{
			bool ready = true;
			while ( running ) {
				Destination dest;
				vector<char> msg = n.read( &dest );

				// Prijal jsem nova data
				if ( msg.size() ) {
					ready = false;

					#pragma omp task shared(ready)
					{
						// Musim si vytvorit kopii bufferu
						vector< char > msgCopy = vector< char >( msg );
						Destination answerDest = Destination( dest );
						ready = true;
						#pragma omp flush(ready)
						
						info.say( "From " + answerDest.str() + " receive: " + msgCopy.data() );

						try {
							MsgData msgData = parse( get<dict>( decode( msgCopy.data() ) ) );

							switch ( msgData.type ) {
								case ACK:
									n.receiveAck( msgData.txid );
									break;


								case HELLO:
									n.addUser( msgData );
									break;

								
								case BYE:
									if ( n.checkUserDest( answerDest, msgData.username ) ) {
										n.eraseUser( msgData.username );
									}
									else {
										if ( n.getUserDestination( msgData.username ) == nullptr ) {
											n.sendError( msgData.txid, "User '" + msgData.username + "' isn't registered!", answerDest );
										}
										else {
											n.sendError(
												msgData.txid,
												"User '" + msgData.username + "' isn't registered from address " + n.getUserDestination( msgData.username )->str() + ". Unsubscribe request from another address!",
												answerDest
											);
										}
									}

									break;

								
								case GETLIST:
									if ( n.checkDest( answerDest ) ) {
										n.sendAck( msgData.txid, answerDest );
										n.sendList( msgData.txid, answerDest );
									}
									else {
										n.sendError(
											msgData.txid,
											"'GETLIST' from unregistred address (" + answerDest.str() + ")!",
											answerDest
										);
									}
									break;


								case UPDATE:
									n.receiveUpdate( answerDest, msgData.data );
									break;


								case DISCONNECT:
									n.sendAck( msgData.txid, answerDest );
									n.receiveDisconnect( answerDest );
									break;

								
								case ERROR:
									print_error( "Received error with txid " + to_string( msgData.txid ) + "\n" + msgData.msg );
									break;


								default:
									print_error( "Received unknown message type '" + MsgTypeString[ msgData.type ] + "'." );
									n.sendError( msgData.txid, "Unknown message type!", answerDest );
									break;
							}
						}
						catch ( Exception &e ) {
							print_error( e.what() );
							print_error( string( "Message from " ) + answerDest.str() + "\n" + string( msgCopy.data() ) );
						}
						catch ( ... ) {
							print_error( "Other error" );
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

			info.say( "Node " + to_string( o.id() ) + " receiving messages terminated." );
		}
	}

	info.say( "Node " + to_string( o.id() ) + " terminated." );

	return 0;
}




void rpc( Options &o, Node &n ) {
	int input = preparePipe( RPC_NODE, o.id() );

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
						cout << "\\[d]atabase \\[n]eighbors \\[c]onnect \\[x]disconnect \\[s]ync \\[q]uit" << endl;
						continue;
					}

					try {
						// Nactu pripdne parametry a vykonam prikaz
						string ipv4, port;
						switch ( decodeString2Command( commandStr ) ) {
							case RPC_DATABASE:
								n.printUsers();
								break;


							case RPC_NEIGHBORS:
								n.printNeighbors();
								break;


							case RPC_CONNECT:
								buffer = buffer.substr( pos + 1 );

								ipv4 = buffer.substr( 0, buffer.find( " " ) );
								buffer = buffer.substr( ipv4.size() + 1 );
								
								port = buffer.substr( 0, buffer.rfind( "\n" ));

								n.connect( Destination( ipv4, port ) );
								break;


							case RPC_DISCONNECT:
								n.disconnect();
								break;


							case RPC_SYNC:
								n.sendUpdate();
								break;


							case RPC_QUIT:
								running = false;
								#pragma omp flush(running)
								break;

							default: print_error( "Invalid RPC '" + commandStr + "'" );
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

	closePipe( input, RPC_NODE, o.id() );
}
