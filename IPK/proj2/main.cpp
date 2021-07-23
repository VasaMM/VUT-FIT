/* * * * * * * * * * * * * * * * * * *
 * AUTOR: Vaclav Martinka (xmarti76)
 * Traceroute
 * 21. 04. 2017 FIT VUT
 * * * * * * * * * * * * * * * * * * */

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>		// Format std::cout
#include <cerrno>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>
#include <linux/errqueue.h>

#include "functions.hpp"
#include "options.hpp"
#include "error.hpp"

using namespace std;

int main ( int argc, char *argv[] ) {
	cout << fixed;	// Formatovani vystupu
	Options options;

	// Nactu argumenty
	read_args( options, argc, argv );

	// Pripojim se k serveru
	addrinfo *res;
	addrinfo flags = { 
		0,           // ai_flags
		AF_UNSPEC,   // ai_family
		SOCK_DGRAM,  // ai_socktype
	};
	// Otestuji adresu
	if ( getaddrinfo( options.address().c_str(), nullptr, &flags, &res ) != 0) { exit_error( "Unable to connect '" + options.address() + "'. Maybe bad address?" ); }
	// Nastavim verzi IP
	bool ipv4 = true;
	if ( res->ai_family == AF_INET6 ) { ipv4 = false; }

	// Najdu volny port <49152;65535>
	if ( options.port() == 0 ) {
		int port = 49152;
		while ( port <= 65535 ) {
			if ( getaddrinfo( options.address().c_str(), to_string( port ).c_str(), &flags, &res ) == 0) { break; }
			++port;
		}
		if ( port > 65535 ) { exit_error( "Unable to connect '" + options.address() + "'. All ports are busy!" ); }
	}
	// Byl nastaven port uzivatelem -> otestuji ho
	else if ( getaddrinfo( options.address().c_str(), to_string( options.port() ).c_str(), &flags, &res ) != 0) {
		exit_error( "Unable to connect '" + options.address() + "'. Port " + to_string( options.port() ) + " are busy!" );
	}
	// Vytvorim sockety
	int echo_socket;
		if ( ( echo_socket = socket( res->ai_family, res->ai_socktype, res->ai_protocol ) ) < 0 ) { exit_error( "Creating socket was unsuccessful!" ); }
	int set_err = 1;
	if ( ipv4 ) {
		if( setsockopt( echo_socket, SOL_IP, IP_RECVERR, &set_err, sizeof( set_err ) ) < 0) { exit_error( "Couldn't set IPv4 socket." ); }
	}
	else {
		if( setsockopt( echo_socket, SOL_IPV6, IPV6_RECVERR, &set_err, sizeof( set_err ) ) < 0) { exit_error( "Couldn't set IPv6 socket." ); }
	}

	// Budu delat ptr dotaz?
	if ( options.ptr() == options.INITIAL ) {
		if ( address_type( options.address() ) == HOSTNAME )
			options.ptr( options.YES );
		else
			options.ptr( options.NO );
	}

	// Samotny traceroute
	bool end = false;
	for ( unsigned int ttl = options.first_ttl() ; ttl <= options.max_ttl() ; ++ttl ) {
		cout << right << setw(2) << ttl << "    ";
		fflush( stdout );
		
		// Nastavim TTL socketu
		if ( ipv4 ) {
			if( setsockopt( echo_socket, IPPROTO_IP, IP_TTL, &ttl, sizeof( ttl ) ) < 0) { exit_error( "Couldn't set IPv4 socket." ); }
		}
		else {
			if( setsockopt( echo_socket, IPPROTO_IPV6, IPV6_UNICAST_HOPS, &ttl, sizeof( ttl ) ) < 0) { exit_error( "Couldn't set IPv6 socket." ); }
		}

		// Ulozim si cas odeslani zpravy
		struct timespec ts1, ts2;
			clock_gettime(CLOCK_MONOTONIC, &ts1);
		
		// Odeslu packet
		if( sendto( echo_socket, nullptr, 0, 0, res->ai_addr, res->ai_addrlen ) < 0) { exit_error( "Couldn't send packet to server (" + to_string(errno) + ")." ); }
		
		// Pockam si na spravnou odpoved
		const unsigned int buffer_size = 512;
		char buffer[ buffer_size ] = {};	// buffer pro odpoved
		icmphdr icmp_h;
		iovec iov = {
			&icmp_h,          // iov_base
			sizeof( icmp_h )  // iov_len
		};
		cmsghdr *cmsg;
		sockaddr_storage address; 
 		msghdr msg = {
			&address,        // msg_name
			sizeof(address), // msg_namelen
			&iov,            // msg_iov
			1,               // msg_iovlen
			buffer,          // msg_control
			sizeof(buffer),  // msg_controllen
			0,               // msg_flags
		};

		while ( true ) {
			clock_gettime(CLOCK_MONOTONIC, &ts2);
			
			// Vyprsel cas -> konec
			if ( ts2.tv_sec - ts1.tv_sec + 1 > options.timeout() ) {
				cout << setw(56) << '*' << endl;
				break;
			}

			// Prijmu odpoved
			if ( recvmsg(echo_socket, &msg, MSG_ERRQUEUE) < 0 ) {
				// Nepovedlo se, zkusím to znovu
				continue;
			}
			// Prijato, poznamenam si cas
			clock_gettime(CLOCK_MONOTONIC, &ts2);

			// Ziskam adresu nebo kod chyby
			for ( cmsg = CMSG_FIRSTHDR( &msg ) ; cmsg ; cmsg = CMSG_NXTHDR( &msg, cmsg ) ) {
				if ( ( cmsg->cmsg_level == SOL_IP   && cmsg->cmsg_type == IP_RECVERR )
				  || ( cmsg->cmsg_level == SOL_IPV6 && cmsg->cmsg_type == IPV6_RECVERR ) ) {
					struct sock_extended_err *sock_err = reinterpret_cast <struct sock_extended_err*> CMSG_DATA( cmsg );
					if ( sock_err && ( sock_err->ee_origin == SO_EE_ORIGIN_ICMP || sock_err->ee_origin == SO_EE_ORIGIN_ICMP6 ) ) {
						char *address_str = &buffer[0];
						
						// Ziskam IP adresu
						if ( ipv4 ) {
							sockaddr_in *sin = reinterpret_cast <struct sockaddr_in *> ( sock_err + 1 );
							inet_ntop( res->ai_family, &sin->sin_addr, address_str, buffer_size );
							
							// Ziskam hostname
							if ( options.ptr() == options.YES ) {
								struct hostent *host_info;
								host_info = gethostbyaddr( &sin->sin_addr, sizeof( in_addr ), res->ai_family );
								if ( host_info ) { address_str = host_info->h_name; }
							// 		host_info = gethostbyaddr( &sin->sin6_addr, sizeof(&sin->sin6_addr), res->ai_family );
							}
						}
						else {
							sockaddr_in6 *sin = reinterpret_cast <struct sockaddr_in6 *> ( sock_err + 1 );
							inet_ntop( res->ai_family, &sin->sin6_addr, address_str, buffer_size );
						
							// Ziskam hostname
							if ( options.ptr() == options.YES ) {
								struct hostent *host_info;
								host_info = gethostbyaddr( &sin->sin6_addr, sizeof( in6_addr ), res->ai_family );
								if ( host_info ) { address_str = host_info->h_name; }
							}
						}

						cout << setw(50) << left << address_str;
						
						if ( ipv4 ) {
							if ( sock_err->ee_type == ICMP_DEST_UNREACH ) {
								end = true;		// Dosahl jsem ciloveho serveru

								switch ( sock_err->ee_code ) {
									case ICMP_HOST_UNREACH:
										cout << setw(7) << right << "H!" << endl;
										break;
									case ICMP_NET_UNREACH:
										cout << setw(7) << right << "N!" << endl;
										break;
									case ICMP_PROT_UNREACH:
										cout << setw(7) << right << "P!" << endl;
										break;
									case ICMP_PKT_FILTERED:
										cout << setw(7) << right << "X!" << endl;
										break;
									// Vyprselo TTL -> tisknu sas
									default:
										cout << setprecision(3) << setw(10) << right << ( ts2.tv_sec - ts1.tv_sec ) * 1e3 + ( ts2.tv_nsec - ts1.tv_nsec ) * 1e-6 << " ms" << endl;
								}
							}
							else {
								cout << setprecision(3) << setw(10) << right << ( ts2.tv_sec - ts1.tv_sec ) * 1e3 + ( ts2.tv_nsec - ts1.tv_nsec ) * 1e-6 << " ms" << endl;
							}
						}
						else {
							if ( sock_err->ee_type == ICMP6_DST_UNREACH || sock_err->ee_type == 101 ) {		// 101 pro www6.fit.vutbr.cz
								end = true;		// Dosahl jsem ciloveho serveru

								switch ( sock_err->ee_code ) {
									case ICMP6_DST_UNREACH_ADDR:
										cout << setw(7) << right << "H!" << endl;
										break;
									case ICMP6_DST_UNREACH_NOROUTE:
										cout << setw(7) << right << "N!" << endl;
										break;
									case 7:
										cout << setw(7) << right << "P!" << endl;
										break;
									case ICMP6_DST_UNREACH_ADMIN:
										cout << setw(7) << right << "X!" << endl;
										break;
									// Vyprselo TTL -> tisknu sas
									default:
										cout << setprecision(3) << setw(10) << right << ( ts2.tv_sec - ts1.tv_sec ) * 1e3 + ( ts2.tv_nsec - ts1.tv_nsec ) * 1e-6 << " ms" << endl;
								}
							}
							else {
								cout << setprecision(3) << setw(10) << right << ( ts2.tv_sec - ts1.tv_sec ) * 1e3 + ( ts2.tv_nsec - ts1.tv_nsec ) * 1e-6 << " ms" << endl;
							}
						}
					}
				} 
			}
			break;
		}

		if ( end ) { break; }
	}

	return 0;
}
