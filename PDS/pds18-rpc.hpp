/**
 * @file pds18-rpc.hpp
 * @author Václav Matinka
 * @date 21. 04. 2019 (10:05)
 * @brief Konstanty použité v RPC a obecné inline funkce
 */

#include <string>

#include "Exception.hpp"
#include "Info.hpp"


/// Výpis cílů (peer, node)
enum Target {
	RPC_PEER,  ///< Příkaz bude předán \c peeru
	RPC_NODE,  ///< Příkaz bude předán \c nodu
	RPC_UNSET  ///< Nenastaveno
};


/// Seznam možných příkazů
enum Command {
	RPC_MESSAGE,     ///< Odeslání zprávy
	RPC_GETLIST,     ///< Odeslání \c getlist příkazu
	RPC_PEERS,       ///< Odeslání \c getlist příkazu a vypís odpovědi
	RPC_RECONNECT,   ///< Připojení k jinému nodu
	RPC_DATABASE,    ///< Zobrazení seznamu peeru a jejich adres
	RPC_NEIGHBORS,   ///< Zobrazení sousedních nodů
	RPC_CONNECT,     ///< Spojení s novým nodem
	RPC_DISCONNECT,  ///< Odpojení od všech nodů
	RPC_SYNC,        ///< Synchronizace databáze se sousedy
	RPC_QUIT,        ///< Ukončí program
	RPC___END__      ///< Konec (nedefinováno)
};


/// Seznam možných příkazů jako stringy
const string commandString[] = {
	"message",     ///< Odeslání zprávy
	"getlist",     ///< Odeslání \c getlist příkazu
	"peers",       ///< Odeslání \c getlist příkazu a vypís odpovědi
	"reconnect",   ///< Připojení k jinému nodu
	"database",    ///< Zobrazení seznamu peeru a jejich adres
	"neighbors",   ///< Zobrazení sousedních nodů
	"connect",     ///< Spojení s novým nodem
	"disconnect",  ///< Odpojení od všech nodů
	"sync",        ///< Synchronizace databáze se sousedy
	"quit",        ///< Ukončí program
};


/// Seznam možných příkazů jako chary
/// \c h je rezervováno pro nápovědu!
const char commandChar[] = {
	'm',  ///< (message) Odeslání zprávy
	'g',  ///< (getlist) Odeslání \c getlist příkazu
	'p',  ///< (peers) Odeslání \c getlist příkazu a vypís odpovědi
	'r',  ///< (reconnect) Připojení k jinému nodu
	'd',  ///< (database) Zobrazení seznamu peeru a jejich adres
	'n',  ///< (neighbors) Zobrazení sousedních nodů
	'c',  ///< (connect) Spojení s novým nodem
	'x',  ///< (disconnect) Odpojení od všech nodů
	's',  ///< (sync) Synchronizace databáze se sousedy
	'q',  ///< (quit) ukončí program
};


/// Struktura pro uchovaní všech parametrů
typedef struct {
	uint id;          ///< ID cíle
	Target target;    ///< Typ cíle
	Command command;  ///< Příkaz

	string from;      ///< Jméno odesílatele
	string to;        ///< Jméno příjemce
	string msg;       ///< Zpráva
	string ipv4;      ///< Nová IP adresa
	string port;      ///< Nový port
} Params;


/// Nápověda
const std::string RPC_HELP = {
	"\n"
	"PDS 2018 RPC\n"
	"Author: Vaclav Martinka (xmarti76)\t\t15. 04. 2019\t\tFIT VUT\n\n"
	
	"Available options:\n"
	"  --id (-i)                   Set ID of peer or node (default 0)\n"
	"  --peer or --node (-n, -p)   Select peer or node\n"
	"  --command <params> (-c)     Select some RPC with params\n\n"
	
	"Available commands:\n"
	"  -p -c [m]essage --from <username1> --to <username2> --message <message>\n"
	"                          Pokusí se odeslat chat zprávu\n"
	"  -p -c [g]etlist         Vynutí aktualizaci seznamu v síti známých peerů\n"
	"  -p -c [p]eers           Zobrazí aktuální seznam peerů v síti\n"
	"  -p -c [r]econnect --reg-ipv4 <IP> --reg-port <port>\n"
	"                          Odpojí se od současného uzlu a připojí se k novému\n"
	"  -n -c [d]atabase        Zobrazí aktuální databázi peerů a jejich mapování\n"
	"  -n -c [n]eighbors       Zobrazí seznam aktuálních sousedů registračního uzlu\n"
	"  -n -c [c]onnect --reg-ipv4 <IP> --reg-port <port>\n"
	"                          Pokusí se navázat sousedství s novým registračním uzlem\n"
	"  -n -c [x]disconnect     Zruší sousedství se všemi uzly a odpojí node od sítě\n"
	"  -n -c [s]ync            Vynutí synchronizaci DB s uzly, se kterými node aktuálně sousedí\n"
	"  -n|-p -c [q]uit         Ukončí program\n\n"
};



/**
 * @brief Dekóduje string na RPC příkaz
 * 
 * @param commandStr String k dekódování
 * @return RPC příkaz
 */
inline Command decodeString2Command( string commandStr ) {
	Command command = RPC___END__;
	if ( commandStr.size() == 1 ) {
		for ( int i = 0; i < RPC___END__; ++i )	{
			if ( commandStr[0] == commandChar[ i ] ) {
				command = static_cast< Command >( i );
				break;
			}
		}
	}
	else {
		for ( int i = 0; i < RPC___END__; ++i )	{
			if ( commandStr == commandString[ i ] ) {
				command = static_cast< Command >( i );
				break;
			}
		}
	}

	return command;
}



/**
 * @brief Připravý pojmenovanou rouru ke čtení
 * 
 * @param target Jedná se o \c PEER nebo \c NODE
 * @param id Jeho id
 * 
 * @return Číslo roury
 */
inline int preparePipe( Target target, uint id ) {
	// Vytvorim pojmenovanou rouru
	string pipe =  "/tmp/";
	switch ( target ) {
		case RPC_PEER: pipe += "peer_"; break;
		case RPC_NODE: pipe += "node_"; break;
		default: throw Exception( INVALID_ARGUMENT, "Invalid argument." );
	}
	pipe += to_string( id );

	if ( mkfifo( pipe.c_str(), 0666 ) < 0 ) {
		print_error( "Cannot create pipeline '" + pipe + "'. Maybe exist? I'm trying to open it..." );
	}

	// Otevru ji
	// Musim pro cteni i zapis, jinak by program cekal na pripojeni pro zapis
	int input = open( pipe.c_str(), O_RDWR );
	if ( input < 0 ) {
		exit_error( "Cannot open pipeline '" + pipe + "'." );
	}

	// Nastavim neblokujici cteni
	if ( fcntl( input, F_SETFL, O_NONBLOCK ) < 0 || fcntl( STDIN_FILENO, F_SETFL, O_NONBLOCK ) < 0 )  {
		exit_error( "Cannot read from pipeline '" + pipe + "'." );
	}

	return input;
}



/**
 * @brief Uzavře pojmenovanou rouru
 * 
 * @param input Její číslo
 * @param target Jedná se o \c PEER nebo \c NODE
 * @param id ID peera nebo nodu
 */
inline void closePipe( int input, Target target, uint id ) {
	string pipe =  "/tmp/";
	switch ( target ) {
		case RPC_PEER: pipe += "peer_"; break;
		case RPC_NODE: pipe += "node_"; break;
		default: throw Exception( INVALID_ARGUMENT, "Invalid argument." );
	}
	pipe += to_string( id );

	close( input );
    unlink( pipe.c_str() );
}