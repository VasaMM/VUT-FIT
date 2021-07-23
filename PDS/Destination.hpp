/**
 * @file Destination.hpp
 * @author Václav Matinka
 * @date 21. 04. 2019 (12:52)
 * @brief Třída pro uchování destinace (IPv4 adresa a port)
 */

#ifndef DESTINATION_HPP
#define DESTINATION_HPP


#include <regex>
#include <string>

#include "Exception.hpp"

using namespace std;


/// Výchozí port je 6677
const uint   DEFAULT_PORT = 6677;

/// Výchozí IP adresa je \c localhost
const string DEFAULT_IPV4 = "127.0.0.1";



/// Třída pro uchování destinace (IPv4 adresa a port)
class Destination {
public:
	/// Implicitní konstruktor, použije výchozí adresu i port
	Destination () {}

	/**
	 * @brief Konstruktor z konkrétních hodnot
	 * 
	 * @param ipv4_input IP adresa
	 * @param port_input Port jako číslo
	 */
	Destination ( string const &ipv4_input, uint const port_input ) { ipv4( ipv4_input ); port( port_input); }
	

	/**
	 * @brief Konstruktor z konkrétních hodnot
	 * 
	 * @param ipv4_input IP adresa
	 * @param port_input Port jako string
	 */
	Destination ( string const &ipv4_input, string const &port_input ) { ipv4( ipv4_input ); port( port_input); }


	/**
	 * @brief Konstruktor ze stringu. Ocekavany  vstup x.x.x.x:y a x.x.x.x,y nebo x.x.x.x
	 * 
	 * @param dest Adresa
	 */
	Destination ( string const &dest ) {
		auto pos = dest.find( ":" );
		if ( pos == std::string::npos ) { pos = dest.find( "," ); }
		if ( pos == std::string::npos ) { ipv4( dest ); return; }

		ipv4( dest.substr( 0, pos ) );
		port( dest.substr( pos + 1 ) );
	}


	/**
	 * @brief Kopírovací konstruktor -> vytvoří kopii objektu
	 * @param dest Kopírovaný objekt
	 */
    Destination( Destination const &dest ) {
    	_ipv4 = dest._ipv4;
    	_port = dest._port;
    } 



private:
	/// IP adresa
	string _ipv4 = DEFAULT_IPV4;
	
	/// Port
	uint _port = DEFAULT_PORT;

	
public:
	/**
	 * @brief Nastaví port
	 * @param p Nový port
	 */
	void port( uint const p ) {
		if ( p > 65535 ) throw Exception( INVALID_PORT );
		_port = p;
	}
	

	/**
	 * @brief Nastaví port
	 * @param p Nový port jako string
	 */
	void port( string const p ) { port( strtol( p.c_str(), nullptr, 10 ) ); }
	
	/**
	 * @brief Získá port
	 * @return Port
	 */
	uint port() const { return _port; }


	/**
	 * @brief Získá port jako string
	 * @return Port jako string
	 */
	string portStr() const { return to_string( _port ); }


	/**
	 * @brief Nastaví IP adresu
	 * @param i Nová IP adresa
	 */
	void ipv4( string const i ) {
		if ( i == "localhost" ) {
			_ipv4 = "127.0.0.1";
		}
		else if ( ! regex_match( i, regex( "([0-9]{1,3}\\.){3}[0-9]{1,3}" ) ) ) {
			throw Exception( INVALID_IPV4 );
		}
		else {
			_ipv4 = i;
		}
	};
	

	/**
	 * @brief Získá IP adresu
	 * @return IP adresa
	 */
	string ipv4() const { return _ipv4; }


	/**
	 * @brief Získá referenci na IP adresu (pro přímé opravy)
	 * @return Reference na IP adresu
	 */
	string &accessIpv4() { return _ipv4; }


	/**
	 * @brief Vrací destinaci jako string ve tvaru ip_adresa:port
	 * @return Destinace jako string ve tvaru ip_adresa:port
	 */
	string str() const { return ipv4() + ":" + portStr(); }


	/**
	 * @brief Vrací destinaci jako string ve tvaru ip_adresa,port
	 * @return Destinace jako string ve tvaru ip_adresa,port
	 */
	string keyStr() const { return ipv4() + "," + portStr(); }


	/**
	 * @brief Porovnávací operátor
	 * @param a První destinace
	 * @param b Druhá destinace
	 * @return Jsou si rovny
	 */
	friend const bool operator == ( Destination const &a, Destination const &b ) {
		return a.port() == b.port() && a.ipv4() == b.ipv4();
	}


	/**
	 * @brief Porovnávací operátor
	 * @param a První destinace
	 * @param b Druhá destinace
	 * @return NEjsou si rovny
	 */
	friend const bool operator != ( Destination const &a, Destination const &b ) {
		return a.port() != b.port() || a.ipv4() != b.ipv4();
	}
	

	/**
	 * @brief Porovnávací operátor
	 * @param a První destinace
	 * @param b Druhá destinace
	 * @return Není větší
	 */
	friend const bool operator < ( Destination const &a, Destination const &b ) {
		if ( a.ipv4() < b.ipv4() ) {
			return true;
		}
		else if ( a.ipv4() == b.ipv4() ) {
			return a.port() < b.port();
		}
		else {
			return false;		
		}
	}
};


#endif

