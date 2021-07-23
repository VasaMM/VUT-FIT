/**
 * @file Info.hpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (10:43)
 * @brief Podpora pro výpis informačních, ladících a chybových hlášek
 */

#ifndef INFO_HPP
#define INFO_HPP 

#include <iostream>
#include <string>

using namespace std;


/// Třída pro výpis hodnot na \c stdout a \c stderr
class Info {
public:
	/// Nastaví program na tichý -> žádný výstup na \c stdout
	void set_quiet() { quiet = true; }
	

	/**
	 * @brief Nastavení tichosti programu
	 * @param q \c True = žádný výstup na stdout
	 */
	void set_quiet( bool q ) { quiet = q; }

	
	/// Povolí informační výpisy	
	void set_verbous() { verbous = true; }


	/**
	 * @brief Nastavení výpisy programu
	 * @param q \c True = povolené informační výpisy
	 */
	void set_verbous( bool v ) { verbous = v; }


	/**
	 * @brief Vypíše na konzoli hlášení. Je-li nastaveno \c quiet, nevypíše nic
	 * @param msg Vypsaná zpráva
	 * @param end Ukončovací řetězec (imlicitně nový řádek)
	 */
	void out ( const string msg, const char* end = "\n", const char* start = "INFO" ) {
		if ( ! quiet ) {
			cout << "[" << start << "] " << msg << end << flush;
		}
	}
	
	
	/**
	 * @brief Vypíše na konzoli zprávu pouze je-li nastveno \c verbous
	 * @param msg Vypsaná zpráva
	 * @param end Ukončovací řetězec (imlicitně nový řádek)
	 */
	void say ( const string msg, const char* end = "\n", const char* start = "DEBUG" ) {
		if ( verbous ) {
			out( msg, end, start );
		}
	}


private:
	/// Uložení nastavení \c quiet
	bool quiet   = false;


	/// Uložení nastavení \c verbous
	bool verbous = false;
};





/**
 * @brief Vypíše chybu na \c stderr ve tvaru ERROR: \<zpráva\>
 * @param error Zpráva k vypsání
 */
inline void print_error( const string error ) {
	cerr << "ERROR: " << error << endl;
}

/**
 * @brief Vypíše chybu na \c stderr ve tvaru ERROR: \<zpráva\> a ukončí program
 * 
 * @param error Zpráva k vypsání
 * @param code Chybový kód s kterým bude program ukončen (výchozí hodnota 1)
 */
inline void exit_error( const string error, const int code = 1 ) {
	cerr << "ERROR: " << error << endl;
	exit ( code );
}

/**
 * @brief Vypíše chybu na \c stderr a ukončí program
 * 
 * @param error Zpráva k vypsání
 * @param code Chybový kód s kterým bude program ukončen (výchozí hodnota 1)
 */
inline void exit( const string error, const int code = 1 ) {
	cerr << error << endl;
	exit ( code );
}

/// Globální objekt pro výpis hlášek
extern Info info;



#ifdef DEBUG
	inline void DEBUG_MSG ( const string msg, const char* end = "\n" ) { cout << msg << end << flush; }  ///< Vypíše zprávu na konzoli, lze denfinovat ukončení (výchozí nový řádek) 
	inline void DEBUG_MSG ( const char*  msg, const char* end = "\n" ) { cout << msg << end << flush; }  ///< Vypíše zprávu na konzoli, lze denfinovat ukončení (výchozí nový řádek) 
	inline void DEBUG_MSG ( const int    msg, const char* end = "\n" ) { cout << msg << end << flush; }  ///< Vypíše zprávu na konzoli, lze denfinovat ukončení (výchozí nový řádek) 
	inline void DEBUG_MSG ( const uint   msg, const char* end = "\n" ) { cout << msg << end << flush; }  ///< Vypíše zprávu na konzoli, lze denfinovat ukončení (výchozí nový řádek) 
	inline void DEBUG_MSG ( const size_t msg, const char* end = "\n" ) { cout << msg << end << flush; }  ///< Vypíše zprávu na konzoli, lze denfinovat ukončení (výchozí nový řádek) 
	inline void DEBUG_MSG ( const double msg, const char* end = "\n" ) { cout << msg << end << flush; }  ///< Vypíše zprávu na konzoli, lze denfinovat ukončení (výchozí nový řádek) 


	inline void DEBUG_MSG ( const char* msg, const string val, const char* end = "\n" ) { cout << msg << ": " << val << end << flush; }  ///< Vypíše hodnotu na konzoli s úvodní zprávou, lze definovat ukončení (výchozí nový řádek)
	inline void DEBUG_MSG ( const char* msg, const char*  val, const char* end = "\n" ) { cout << msg << ": " << val << end << flush; }  ///< Vypíše hodnotu na konzoli s úvodní zprávou, lze definovat ukončení (výchozí nový řádek)
	inline void DEBUG_MSG ( const char* msg, const int    val, const char* end = "\n" ) { cout << msg << ": " << val << end << flush; }  ///< Vypíše hodnotu na konzoli s úvodní zprávou, lze definovat ukončení (výchozí nový řádek)
	inline void DEBUG_MSG ( const char* msg, const uint   val, const char* end = "\n" ) { cout << msg << ": " << val << end << flush; }  ///< Vypíše hodnotu na konzoli s úvodní zprávou, lze definovat ukončení (výchozí nový řádek)
	inline void DEBUG_MSG ( const char* msg, const size_t val, const char* end = "\n" ) { cout << msg << ": " << val << end << flush; }  ///< Vypíše hodnotu na konzoli s úvodní zprávou, lze definovat ukončení (výchozí nový řádek)
	inline void DEBUG_MSG ( const char* msg, const double val, const char* end = "\n" ) { cout << msg << ": " << val << end << flush; }  ///< Vypíše hodnotu na konzoli s úvodní zprávou, lze definovat ukončení (výchozí nový řádek)

#else
	inline void DEBUG_MSG ( const string msg, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const char*  msg, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const int    msg, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const uint   msg, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const size_t msg, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const double msg, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	
	inline void DEBUG_MSG ( const char* msg, const string val, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const char* msg, const char*  val, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const char* msg, const int    val, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const char* msg, const uint   val, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const char* msg, const size_t val, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const char* msg, const double val, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG

#endif

#endif