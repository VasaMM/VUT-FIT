/**
 * @file parser.cpp
 * @author Václav Matinka
 * @date 22. 04. 2019 (20:44)
 * @brief Parsování přijatých zpráv
 */

#include <regex>

#include "parser.hpp"
#include "bencode.hpp"
#include "Exception.hpp"

using namespace std;
using namespace bencode;



MsgData parse( dict const &msg ) {
	MsgData output = { __OTHER__, NO_ID, };
	PREPARE();

	try {
		string type = GET( string, "type" );
		output.txid = GET( integer, "txid" );

		int msgType;
		for ( msgType = 0 ; msgType < __OTHER__ ; ++msgType ) {
			if ( type == MsgTypeString[ msgType ] ) {
				output.type = static_cast< MsgType >( msgType );
				break;
			}
		}

		switch ( msgType ) {
			case __OTHER__:
				throw Exception( MSG_UNKNOWN_TYPE, "Message with unknown type '" + type + "'" );

			// Nemusim delat nic
			case GETLIST:
			case ACK:
			case DISCONNECT:
				break;

			case HELLO:
				output.username = GET( string, "username" );
				output.ipv4     = GET( string, "ipv4" );
				output.port     = GET( integer, "port" );
				
				if ( regex_match( output.ipv4, regex( "(0{1,3}\\.){3}0{1,3}" ) ) && output.port == 0 ) {
					output.type = BYE;
				}
				break;

			case LIST:
				output.data = GET( dict, "peers" );
				break;

			case MESSAGE:
				output.username = GET( string, "from" );
				output.msg      = GET( string, "message" );
				break;

			case UPDATE:
				output.data = GET( dict, "db" );
				break;

			case ERROR:
				output.msg = GET( string, "verbose" );
				break;

			default: throw Exception( MSG_UNKNOWN_TYPE, "msgType with unknown type '" + type + "'" );
		}
	}
	catch ( out_of_range &e ) {
		throw Exception( MSG_WITHOUT_TYPE, "Message without '" + val + "' value." );
	}

	return output;
}

