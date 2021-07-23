/**
 * @file Exception.hpp
 * @author Václav Matinka
 * @date 21. 04. 2019 (12:20)
 * @brief Třída s implementací výjimky
 */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP


#include <cstring>
#include <exception>
#include <iostream>
#include <string>


using namespace std;


/// Výčet ID pro jednotlivé výjimky
enum ExceptionId {
	MSG_WITHOUT_TYPE,     ///< Zpráva neobsahovala položku type
	MSG_UNKNOWN_TYPE,     ///< Zpráva obsahovala neznámou položku type
	MSG_INVALID_TYPE,     ///< Zpráva obsahovala nepovolený typ
	NO_ACK,               ///< Nedostal jsem \c ACK na zprávu
	UNKNOWN_DESTINATION,  ///< Zprava dorzila z nezname destinace
	INVALID_ARGUMENT,     ///< Funkce dostala neplatný argument
	PIPELINE_ERROR,       ///< Chyba při práci s pojmenovanou rourou
	INVALID_IPV4,         ///< Neplatná IPv4 adresa
	INVALID_PORT,         ///< Neplatné číslo portu
	INVALID_ID,           ///< Neplatné ID instance
};


/// Třída výjimek
class Exception : public exception {
private:
	/// Podrobný popis chyby
	string message;

	/// ID chyby
	ExceptionId id;

public:
	/**
	 * @brief Konstruktor výjimky
	 * 
	 * @param _id ID chyby
	 * @param msg Podrobný popis chyby
	 */
	Exception( ExceptionId const _id, string const msg = "" ) { id = _id; message = msg; }

	/**
	 * @brief Funkce what (volána implicitně, není-li výjimka ošetřena)
	 * @return Popis chyby
	 */
	const char* what() const noexcept {
		string tmp =  "ERROR " + to_string( id ) + ": " + message;
		
		char* output = new char[ tmp.length() + 1 ];
		strcpy( output, tmp.c_str() );

		return output;
	}
};

#endif