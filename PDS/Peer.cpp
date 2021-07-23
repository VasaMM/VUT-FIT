/**
 * @file Peer.cpp
 * @author Václav Matinka
 * @date 26. 04. 2019 (09:18)
 * @brief Třída implementující funkce potřebné pro běh \c peera
 */

#include <map>
#include <omp.h>
#include <iomanip>

#include "Peer.hpp"
#include "parser.hpp"
#include "Exception.hpp"
#include "Info.hpp"



void Peer::receiveList( dict const &data ) {
	if ( saveUsersList ) {
		tmpUsersList.clear();
	}

	try {
		for ( auto const& [ id, p ] : data ) {
			(void)id;
			dict peer = get< dict >( p );

			users[ get< string >( peer[ "username" ] ) ] = {
				Destination (
					get< string >( peer[ "ipv4" ] ),
					get< integer >( peer[ "port" ] )
				),
				time(0)
			};

			if ( saveUsersList ) {
				tmpUsersList[ get< string >( peer[ "username" ] ) ] = users[ get< string >( peer[ "username" ] ) ];
			}
		}
	}
	catch ( ... ) {
		tmpUsersList.clear();
		print_error( "Bad LIST structure\n" );
	}

	receivedUserList = true;
	#pragma omp flush(receivedUserList)
}



bool Peer::waitList() {
	double waitTime = omp_get_wtime() + LIST_TIMEOUT;
	while ( *running && omp_get_wtime() < waitTime ) {
		if ( receivedUserList ) {
			receivedUserList = false;
			return true;
		}

		usleep( 10 );
		#pragma omp flush(running, receivedUserList)
	}

	return false;
}



Txid Peer::sendHello() const {
	return sender->sendMsg( dict{
		{ "type",     "hello" },
		{ "username", username },
		{ "ipv4",     me.ipv4() },
		{ "port",     me.port() },
	}, server );
}



Txid Peer::sendBye() const {
	return sender->sendMsg( dict{
		{ "type",     "hello" },
		{ "username", username },
		{ "ipv4",     "0.0.0.0" },
		{ "port",     0 },
	}, server );
}



Txid Peer::sendGetlist() const {
	return sender->sendMsg( dict{ { "type", "getlist" } }, server );
}



Txid Peer::sendMsg( string const &from, string const &to, string const &message ) {
	Txid msgId = NO_ID;

	// Mam adresu uzivatele - zkusim poslat zpravu
	try {
		User user = users.at( to );
		msgId = sender->sendMsg( dict{
					{ "type",    "message" },
					{ "from",    from },
					{ "to",      to },
					{ "message", message },
				}, user.dest );

		// Pockam na ACK
		if ( ! waitAck( msgId ) ) {
			throw Exception( NO_ACK );
		}
	}
	// Neco se nepodarilo, zkusim ziskat adresu
	catch ( ... ) {
		try {
			Txid getlistId = sendGetlist();

			// Cekam na ack
			if( ! waitAck( getlistId ) ) {
				print_error( "The 'GETLIST' has been sent but has not been confirmed, but I'm still waiting for 'LIST'." );

			}
		
			// Cekam na prvni list
			if ( waitList() ) {
				User user = users.at( to );
				msgId = sender->sendMsg( dict{
							{ "type",    "message" },
							{ "from",    from },
							{ "to",      to },
							{ "message", message },
						}, user.dest );
			}
			else {
				print_error( "Failed to get list." );
			}
		}
		catch( out_of_range &e ) {
			print_error( "Node doesn't know the user '" + to + "'." );
		}
		catch ( ... ) {
			print_error( "Failed to send message." );
		}
	}

	return msgId;
}



bool Peer::printList() {
	// Odeslu zadost o list
	saveUsersList = true;
	int getlistId = sendGetlist();

	// Cekam na ack
	if( ! waitAck( getlistId ) ) {
		print_error( "The 'GETLIST' has been sent but has not been confirmed, but I'm still waiting for 'LIST'." );
	}

	// Zkusim ziskat list i kdyz ack nebylo doruceno
	if ( waitList() ) {
		stringstream output;
		output << "+-----+----------------------+-----------------------+" << endl;
		output << "| ID  | USERNAME             | IPV4:PORT             |" << endl;
		output << "+-----+----------------------+-----------------------+" << endl;
		uint i = 0;
	
		#pragma omp critical(listVector)
		{
			for( auto const& [ username, data ] : tmpUsersList ) {
				output << "| " << right << setw(3) << i++ << " | " << left << setw(20) << username << " | " << setw(21) << data.dest.str() << " |" << endl;
			}
			output << "+-----+----------------------+-----------------------+" << endl;
		
			saveUsersList = false;
			tmpUsersList.clear();
		}

		cout << output.str() << endl;

		return true;
	}

	return false;
}



void Peer::clean() {
	auto it = users.begin();
	while ( it != users.end() ) {
		if ( time(0) - it->second.stamp > USER_CLEAN_TIME ) {
			info.say( "Timeout for user: " + it->first );
			it = users.erase( it );
		}
		else {
			++it;
		}
	}

	clearAcks();
}
