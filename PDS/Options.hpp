/**
 * @file Options.hpp
 * @author Václav Matinka
 * @date 22. 04. 2019 (20:38)
 * @brief Objekt pro uchování nastavení a dalších podpůrných funkcí
 */

#ifndef OPTIONS_HPP
#define OPTIONS_HPP


#include <string>

#include "Destination.hpp"

using namespace std;



/// Nápověda k pro peera
const std::string PEER_HELP = {
	"\n"
	"PDS 2018 peer\n"
	"Author: Vaclav Martinka (xmarti76)\t\t17. 04. 2019\t\tFIT VUT\n\n"
	
	"Available options:\n"
	"  -h, --help            Display this help and exit.\n\n"
	
	"  --id <number>         Program instance ID (default 0)\n"
	"  --username <string>   Username in chat (default user)\n"
	"  --chat-ipv4 <string>  Peer IPv4 address (default 127.0.0.1 aka localhost)\n"
	"  --chat-port <number>  Peer port (default 6677)\n"
	"  --reg-ipv4 <string>   Node IPv4 address (default 127.0.0.1 aka localhost)\n"
	"  --reg-port <number>   Node port (default 6677)\n"
	"\n"
	"  -v, --verbous         Debuging  reports will be displayed.\n"
};


/// Nápověda k pro node
const std::string NODE_HELP = {
	"\n"
	"PDS 2018 node\n"
	"Author: Vaclav Martinka (xmarti76)\t\t17. 04. 2019\t\tFIT VUT\n\n"
	
	"Available options:\n"
	"  -h, --help            Display this help and exit.\n\n"
	
	"  --id <number>         Program instance ID (default 0)\n"
	"  --reg-ipv4 <string>   Node IPv4 address (default 127.0.0.1 aka localhost)\n"
	"  --reg-port <number>   Node port (default 6677)\n"
	"\n"
	"  -v, --verbous         Debuging  reports will be displayed.\n"
};



/// Objekt pro uchování nastavení a dalších podpůrných funkcí
class Options {
public:
	/// Pro jaký program je nastavení načteno
	enum ProgramType {
		PEER,  ///< peer
		NODE,  ///< node
	};


	/**
	 * @brief Konstruktor objektu očekává proměnné \c argc a \c argv z volání funkce main
	 * 
	 * @param argc Počet argumentů předaných programu
	 * @param argv Seznam argumentů předaných programu
	 * @param type Jedná se o \c PEER nebo \c NODE
	 */
	 Options ( int const argc, char** const argv, ProgramType const type );


private:
	/// Pro jaký program je nastavení načteno
	ProgramType _type;

	/// ID programu
	uint _id = 0;

	/// Username
	string _username = "user";

	/// Adresa peera
	Destination _chat;

	/// Adresa nodu
	Destination _reg;

public:
	/// Vypíše nápovědu a ukončí program
	void help();


	/**
	 * @brief  Nastaví ID
	 * @param i ID
	 */
	void id( uint const i ) {
		if ( i < 0 ) throw Exception( INVALID_ID );
		_id = i;
	}

	/**
	 * @brief  Nastaví ID
	 * @param i ID
	 */
	void id( string const i ) { id( strtol( i.c_str(), nullptr, 10 ) ); }

	/**
	 * @brief  Nastaví uživatelské jméno
	 * @param u Uživatelské jméno
	 */
	void username( string const &u ) { _username = u; }
	
	/**
	 * @brief  Nastaví adresu peera
	 * @param ch Adresa peera
	 */
	void chat( Destination const &ch ) { _chat = ch; }

	/**
	 * @brief  Nastaví adresu nodu
	 * @param r Adresa nodu
	 */
	void reg( Destination const &r ) { _reg = r; }


	/**
	 * @brief Vrácí ID
	 * @return ID
	 */
	uint id() const { return _id; }
	

	/**
	 * @brief Vrací uživatelské jméno
	 * @return Uživatelské jméno
	 */
	string &username() { return _username; }


	/**
	 * @brief Vrací uživatelské jméno
	 * @return Uživatelské jméno
	 */
	string username() const { return _username; }
	

	/**
	 * @brief Vrací adresu peera
	 * @return Adresa peera
	 */
	Destination &chat() { return _chat; }


	/**
	 * @brief Vrací adresu peera
	 * @return Adresa peera
	 */
	Destination chat() const { return _chat; }


	/**
	 * @brief Vrací adresu nodu
	 * @return Adresa nodu
	 */
	Destination &reg() { return _reg; }
	

	/**
	 * @brief Vrací adresu nodu
	 * @return Adresa nodu
	 */
	Destination reg() const { return _reg; }
};



#endif