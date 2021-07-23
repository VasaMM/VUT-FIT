/**
 * @file Node.cpp
 * @author Václav Matinka
 * @date 26. 04. 2019 (09:15)
 * @brief Třída implementující funkce potřebné pro běh \c nodu
 */

#include <omp.h>
#include <iomanip>
#include <ctime>

#include "Node.hpp"
#include "Info.hpp"


void Node::addUser( MsgData const &user ) {
	User previousData;
	#pragma omp critical(addUser)
	{
		info.say( "Add user: " + user.username );

		previousData = usersDatabase[ me ][ user.username ];
		usersDatabase[ me ][ user.username ] = { Destination ( user.ipv4, user.port ), time(0) };
	}

	// Doslo ke zmene, zaslu update
	if ( previousData.dest != Destination ( user.ipv4, user.port ) ) {
		sendUpdate();
	}
}



void Node::eraseUser( string const &username ) {
	info.say( "Remove user: " + username );
	usersDatabase[ me ].erase( username );
	sendUpdate();
}



void Node::clean() {
	auto it = usersDatabase[ me ].begin();
	while ( it != usersDatabase[ me ].end() ) {
		if ( time(0) - it->second.stamp > USER_TTL ) {
			info.say( "Timeout for user: " + it->first );
			it = usersDatabase[ me ].erase( it );
		}
		else {
			++it;
		}
	}

	auto it2 = usersDatabase.begin();
	while ( it2 != usersDatabase.end() ) {
		if ( it2->first != me && time(0) - it2->second.begin()->second.stamp > NODE_TTL ) {
			info.say( "Timeout for node: " + it2->first.str() );
			it2 = usersDatabase.erase( it2 );
		}
		else {
			++it2;
		}
	}

	// for( auto const& [ username, data ] : usersDatabase[ me ] ) {
	// 	if ( time(0) - data.stamp > USER_TTL ) {
	// 		info.say( "Timeout for user: " + username );
	// 		usersDatabase[ me ].erase( username );
	// 	}
	// }

	// for( auto const& [ neighbor, users ] : usersDatabase ) {
	// 	if ( neighbor != me && time(0) - users.begin()->second.stamp > NODE_TTL ) {
	// 		info.say( "Timeout for node: " + neighbor.str() );
	// 		usersDatabase.erase( neighbor );
	// 	}
	// }

	clearAcks();
}



void Node::receiveUpdate( Destination const &from, dict const &db ) {
	map< string, User > tmp;

	#pragma omp critical(update)
	{
		try {
			for ( auto const& [ destination, data ] : db ) {
				// Nebudu posilat sam sobe
				if ( destination == me ) {
					continue;
				}
				// Autoritativni zaznamy muzu pridat
				else if ( destination == from ) {
					for ( auto const& [ id, p ] : get< dict >( data ) ) {
						(void)id;
						dict peer = get< dict >( p );

						tmp[ get< string >( peer[ "username" ] ) ] = {
							Destination(
								get< string >( peer[ "ipv4" ] ),
								get< integer >( peer[ "port" ] )
							),
							time(0)
						};
					}

					usersDatabase[ Destination( destination ) ] = tmp;
				}
				// Pokusim se navazat spojeni
				else {
					sendUpdate( destination );
				}
			}
		}
		catch ( ... ) {
			print_error( "Bad 'UPDATE' structure\n" );
		}
	}
}



void Node::receiveDisconnect( Destination const &from ) {
	usersDatabase.erase( from );
}



Txid Node::sendList( Txid const txid, Destination const &answerDest ) const {
	dict peers;

	int i = 0;
	for ( auto const& [ owner, usersMap ] : usersDatabase ) {
		(void)owner;
		for( auto const& [ username, data ] : usersMap ) {
			peers[ to_string( i++ ) ] = dict{
				{ "username", username },
				{ "ipv4",     data.dest.ipv4() },
				{ "port",     data.dest.port() },
			};
		}
	}

	return sender->sendMsg( dict{
		{ "type",  "list" },
		{ "txid",  txid },
		{ "peers", peers },
	}, answerDest );
}



dict Node::prepareDb() const {
	dict db;

	for ( auto const& [ owner, usersMap ] : usersDatabase ) {
		dict peers;
		uint i = 0;
		for ( auto const& [ username, data ] : usersMap ) {
			peers[ to_string( i++ ) ] = dict{
				{ "username", username },
				{ "ipv4",     data.dest.ipv4() },
				{ "port",     data.dest.port() },
			};
		}

		db[ owner.keyStr() ] = peers;
	}

	return db;
}



void Node::sendUpdate() {
	dict db = prepareDb();

	for ( auto const& [ to, data ] : usersDatabase ) {
		(void)data;
		if ( me != to ) {
			sendUpdate( to, db );
		}
	}
}



Txid Node::sendUpdate( Destination const &to, dict const &db ) const {
	return sender->sendMsg( dict{
		{ "type", "update" },
		{ "db",   db },
	}, to );
}


Txid Node::sendUpdate( Destination const &to ) const {
	return sender->sendMsg( dict{
		{ "type", "update" },
		{ "db",   prepareDb() },
	}, to );
}



void Node::disconnect() {
	#pragma omp critical(update)
	{
		for ( auto const& [ to, data ] : usersDatabase ) {
			(void)data;
			if ( me != to ) {
				sender->sendMsg( dict{ { "type", "disconnect" } }, to );
				usersDatabase.erase( to );
			}
		}
	}
}



void Node::connect( Destination const &dest ) {
	sendUpdate( dest );
}



void Node::printUsers() {
	stringstream output;

	#pragma omp critical(addUser)
	{
		uint i = 0;

		output << "+-----+-----------------------+----------------------+-----------------------+" << endl;
		output << "| ID  | NODE ( IPV4:PORT )    | PEER USERNAME        | PEER IPV4:PORT        |" << endl;
		output << "+-----+-----------------------+----------------------+-----------------------+" << endl;
	
		for( auto const& [ username, val ] : usersDatabase[ me ] ) {
			output << "| " << right << setw(3) << i++ << " | me                    | " << left << setw(20) << username << " | " << setw(21) << val.dest.str() << " |" << endl;
		}

		for( auto const& [ dest, database ] : usersDatabase ) {
			string destStr = dest.str();
			if ( dest != me ) {
				for( auto const& [ username, val ] : database ) {
					output << "| " << right << setw(3) << i++ << " | " << left << setw(21) << dest.str() << " | " << setw(20) << username << " | " << setw(21) << val.dest.str() << " |" << endl;
				}
			}
		}

		output << "+-----+-----------------------+----------------------+-----------------------+" << endl;
	}

	cout << output.str();
}



void Node::printNeighbors() {
	stringstream output;

	#pragma omp critical(addUser)
	{
		output << "+-----+----------------------------------------------+" << endl;
		output << "| ID  |  IPV4:PORT                                   |" << endl;
		output << "+-----+----------------------------------------------+" << endl;
		output << "|  me | " << left << setw(44) << me.str() << " | " << endl;

		uint i = 0;
		for( auto const& [ dest, database ] : usersDatabase ) {
			(void)database;
			string destStr = dest.str();
			if ( dest == me ) {
				continue; 
			}
			output << "| " << right << setw(3) << i++ << " | " << left << setw(44) << dest.str() << " | " << endl;
		}
		output << "+-----+----------------------------------------------+" << endl << endl;
	}

	cout << output.str();
}



bool Node::checkUserDest( Destination &dest, string &username ) {
	try {
		return dest == usersDatabase[ me ].at( username ).dest;
	}
	catch ( out_of_range &e ) {
		return false;
	}
}



bool Node::checkDest( Destination &dest ) {
	for( auto const& [ username, data ] : usersDatabase[ me ] ) {
		(void)username;
		if ( data.dest == dest ) {
			return true;
		}
	}

	return false;
}