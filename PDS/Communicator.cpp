/**
 * @file Communicator.cpp
 * @author Václav Matinka
 * @date 23. 04. 2019 (20:13)
 * @brief Třída obalující samotnou UDP komunikaci a jejich volání společná pro \c peer i \c node (např. odesílání a příjem \c ack)
 */


#include <omp.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>


#include "Communicator.hpp"
#include "Info.hpp"


// http://www.builder.cz/rubriky/c/c--/protokol-udp-1-cast-156226cz
// http://www.builder.cz/rubriky/c/c--/protokol-udp-2-cast-156227cz
Txid Sender::sendMsg( dict msg, Destination const &to ) {
	Txid id;

	// Zkontroluji a pripadne pridam txid
	try {
		id = get< Txid >( msg.at( "txid" ) );
	}
	// Musim pridat ID
	catch ( out_of_range &e ) {
		#pragma omp critical(updateTxid)
			id = txid++;

		msg[ "txid" ] = id;
	}


	// Odeslu zpravu
	string msgEncoded = encode( msg );

	#pragma omp critical(send)
	{
		hostent *host;
		if ( ( host = gethostbyname( to.ipv4().c_str() ) ) == NULL ) {
			print_error( "Cannot send message to " + to.str() );
		}

		// Nastavim port serveru
		sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port   = htons( to.port() );
		memcpy( &serverAddr.sin_addr, host->h_addr, host->h_length );

		if ( sendto( *socket, msgEncoded.c_str(), msgEncoded.size(), 0, static_cast< sockaddr* >( static_cast< void* >( &serverAddr ) ), sizeof( serverAddr ) ) < 0 ) {
			print_error( "Cannot send message to " + to.str() );
			id = NO_ID;
		}
	}

	info.say( "Message " + to_string( id ) + " for " + to.str() + " sended: " + msgEncoded );

	return id;
}



Receiver::Receiver( int *_socket ) {
	socket = _socket;

	// Nastavim timeout
	struct timeval readTimeout;
	readTimeout.tv_sec  = 0;
	readTimeout.tv_usec = 10;
	setsockopt( *socket, SOL_SOCKET, SO_RCVTIMEO, &readTimeout, sizeof readTimeout );
}


	
vector<char> Receiver::receive( Destination *from ) {
	std::vector<char> buffer;

	// Poznačím si velikost struktury clientInfo.
	sockaddr_in clientInfo;
	static uint addrlen = sizeof( clientInfo );
	

	int size;
	buffer.resize( BUFFER_SIZE );
	size = recvfrom( *socket, buffer.data(), BUFFER_SIZE, 0, static_cast< sockaddr* >( static_cast< void* >( &clientInfo ) ), &addrlen );

	if ( size != -1 ) {
		buffer.resize( size );

		// Ulozim si udaje odesilatele
		if ( from != nullptr ) {
			char ipv4[ 128 ] = { 0, };
			inet_ntop( AF_INET, &clientInfo.sin_addr, ipv4, 128 );
			
			from->port( ntohs( clientInfo.sin_port ) );
			from->ipv4( ipv4 );
		}	
	}
	else {
		buffer.clear();
	}

	return buffer;
}



Communicator::Communicator( bool *r, Destination const meDest, Destination const serverDest ) {
	running = r;

	me = meDest;
	server = serverDest;

	// Vytvorim socket
	if ( ( mySocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) < 0 ) {
		exit_error( "Creating socket was unsuccessfully." );
	}

	// Nastavim lokalni port
	sockaddr_in socketName;
	memset( &socketName, 0, sizeof( socketName ) );
	socketName.sin_family      = AF_INET;
	socketName.sin_port        = htons( me.port() );
	socketName.sin_addr.s_addr = INADDR_ANY;

	if ( bind( mySocket, static_cast< sockaddr* >( static_cast< void* >( &socketName ) ), sizeof( socketName ) ) < 0 ) {
		exit_error( "Cannot use port " + me.portStr() + ". It is probably being used by another application." );
	}

	sender   = new Sender(   &mySocket );
	receiver = new Receiver( &mySocket );
}



Txid Communicator::sendAck( Txid const txid, Destination const &to ) const {
	return sender->sendMsg( dict{
		{ "type", "ack" },
		{ "txid", txid },
	}, to );
}



void Communicator::receiveAck( Txid const txid ) {
	ackMap[ txid ] = time(0);
}



bool Communicator::checkAck( Txid const txid ) const {
	try {
		ackMap.at( txid );
		return true;
	}
	catch ( out_of_range &e ) {
		return false;
	}
}



bool Communicator::waitAck( Txid const txid ) const {
	double waitTime = omp_get_wtime() + ACK_TIMEOUT;
	while ( *running && omp_get_wtime() < waitTime ) {
		if ( checkAck( txid ) ) {
			return true;
		}

		usleep( 10 );
		#pragma omp flush(running)
	}

	return false;
}



Txid Communicator::sendError( Txid const txid, string const &msg, Destination const &to ) const {
	return sender->sendMsg( dict{
		{ "type",    "error" },
		{ "txid",    txid },
		{ "verbose", msg },
	}, to );
}




void Communicator::clearAcks() {
	auto it = ackMap.begin();
	while ( it != ackMap.end() ) {
		if ( time(0) - it->second > ACK_CLEAN_TIME ) {
			it = ackMap.erase( it );
		}
		else {
			++it;
		}
	}
}

