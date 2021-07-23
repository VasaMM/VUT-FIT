/**
 * @file Exception.hpp
 * @author Václav Matinka
 * @date 07. 04. 2019 (22:15)
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
	INVALID_FILE,     ///< Nevalidní soubor
	FILE_ERROR,       ///< Problém se souborem
	MEMORY_ERROR,     ///< Problém s pamětí
	INDEX_ERROR,      ///< Špatný index
	UNSPECIFIED,      ///< Nespecifikovaná chyba
	TOO_BIG_SYMBOL,   ///< Příliš velký symbol
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
	Exception( const ExceptionId _id, const string msg ) { id = _id; message = msg; }

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