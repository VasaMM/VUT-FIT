/**
 * @file Info.hpp
 * @author Václav Matinka
 * @date 21. 04. 2019 (09:44)
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
	 * @param v \c True = povolené informační výpisy
	 */
	void set_verbous( bool v ) { verbous = v; }


	/**
	 * @brief Vypíše na konzoli hlášení. Je-li nastaveno \c quiet, nevypíše nic
	 * @param msg Vypsaná zpráva
	 * @param end Ukončovací řetězec (imlicitně nový řádek)
	 * @param start Uvozovací řetězec ohraničený [] (imlicitně INFO => vypíše [INFO])
	 */
	void out ( const string msg, const char* end = "\n", const char* start = "INFO" ) {
		if ( ! quiet ) {
			cout << "[" << start << "] " << msg << end << flush;
		}
	}
	
	
	/**
	 * @brief Vypíše na konzoli zprávu pouze je-li nastaveno \c verbous
	 * @param msg Vypsaná zpráva
	 * @param end Ukončovací řetězec (imlicitně nový řádek)
	 * @param start Uvozovací řetězec ohraničený [] (imlicitně DEBUG => vypíše [DEBUG])
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
 * @param end Ukončovací řetězec (imlicitně nový řádek)
 * @param start Uvozovací řetězec ohraničený [] (imlicitně ERROR => vypíše [ERROR])
 */
inline void print_error( string const error, const char* end = "\n", const char* start = "ERROR" ) {
	cerr << "[" << start << "] " << error << end << flush;
}

/**
 * @brief Vypíše chybu na \c stderr
 * @param end Ukončovací řetězec (imlicitně nový řádek)
 * @param msg Zpráva k vypsání
 */
inline void error( string const msg, const char* end = "\n" ) {
	cerr << msg << end << flush;
}

/**
 * @brief Vypíše chybu na \c stderr ve tvaru ERROR: \<zpráva\> a ukončí program
 * 
 * @param error Zpráva k vypsání
 * @param code Chybový kód s kterým bude program ukončen (výchozí hodnota 1)
 */
inline void exit_error( string const error, int const code = 1 ) {
	print_error( error );
	exit ( code );
}

/**
 * @brief Vypíše chybu na \c stderr a ukončí program
 * 
 * @param err Zpráva k vypsání
 * @param code Chybový kód s kterým bude program ukončen (výchozí hodnota 1)
 */
inline void exit( string const err, int const code = 1 ) {
	error( err );
	exit ( code );
}

/// Globální objekt pro výpis hlášek
extern Info info;



#ifdef DEBUG
	inline void DEBUG_MSG ( const char*  msg, const char* end = "\n" ) { printf( "%s%s",  msg, end ); fflush( stdout ); }  ///< Vypíše zprávu na konzoli, lze denfinovat ukončení (výchozí nový řádek) 
	inline void DEBUG_MSG ( const int    msg, const char* end = "\n" ) { printf( "%i%s",  msg, end ); fflush( stdout ); }  ///< Vypíše zprávu na konzoli, lze denfinovat ukončení (výchozí nový řádek) 
	inline void DEBUG_MSG ( const uint   msg, const char* end = "\n" ) { printf( "%i%s",  msg, end ); fflush( stdout ); }  ///< Vypíše zprávu na konzoli, lze denfinovat ukončení (výchozí nový řádek) 
	inline void DEBUG_MSG ( const size_t msg, const char* end = "\n" ) { printf( "%lu%s", msg, end ); fflush( stdout ); }  ///< Vypíše zprávu na konzoli, lze denfinovat ukončení (výchozí nový řádek) 
	inline void DEBUG_MSG ( const double msg, const char* end = "\n" ) { printf( "%f%s",  msg, end ); fflush( stdout ); }  ///< Vypíše zprávu na konzoli, lze denfinovat ukončení (výchozí nový řádek) 
	inline void DEBUG_MSG ( const string msg, const char* end = "\n" ) { printf( "%s%s",  msg.c_str(), end ); fflush( stdout ); }  ///< Vypíše zprávu na konzoli, lze denfinovat ukončení (výchozí nový řádek) 


	inline void DEBUG_MSG ( const char* msg, const char*  val, const char* end = "\n" ) { printf( "%s: %s%s",  msg, val, end ); fflush( stdout ); }  ///< Vypíše hodnotu na konzoli s úvodní zprávou, lze definovat ukončení (výchozí nový řádek)
	inline void DEBUG_MSG ( const char* msg, const int    val, const char* end = "\n" ) { printf( "%s: %i%s",  msg, val, end ); fflush( stdout ); }  ///< Vypíše hodnotu na konzoli s úvodní zprávou, lze definovat ukončení (výchozí nový řádek)
	inline void DEBUG_MSG ( const char* msg, const uint   val, const char* end = "\n" ) { printf( "%s: %i%s",  msg, val, end ); fflush( stdout ); }  ///< Vypíše hodnotu na konzoli s úvodní zprávou, lze definovat ukončení (výchozí nový řádek)
	inline void DEBUG_MSG ( const char* msg, const size_t val, const char* end = "\n" ) { printf( "%s: %lu%s", msg, val, end ); fflush( stdout ); }  ///< Vypíše hodnotu na konzoli s úvodní zprávou, lze definovat ukončení (výchozí nový řádek)
	inline void DEBUG_MSG ( const char* msg, const double val, const char* end = "\n" ) { printf( "%s: %f%s",  msg, val, end ); fflush( stdout ); }  ///< Vypíše hodnotu na konzoli s úvodní zprávou, lze definovat ukončení (výchozí nový řádek)
	inline void DEBUG_MSG ( const char* msg, const string val, const char* end = "\n" ) { printf( "%s: %s%s",  msg, val.c_str(), end ); fflush( stdout ); }  ///< Vypíše hodnotu na konzoli s úvodní zprávou, lze definovat ukončení (výchozí nový řádek)

#else
	inline void DEBUG_MSG ( const char*  msg, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const int    msg, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const uint   msg, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const size_t msg, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const double msg, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const string msg, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	
	inline void DEBUG_MSG ( const char* msg, const char*  val, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const char* msg, const int    val, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const char* msg, const uint   val, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const char* msg, const size_t val, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const char* msg, const double val, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG
	inline void DEBUG_MSG ( const char* msg, const string val, const char* end = "\n" ) {}  ///< Nevypíše nic => není defiováno \c DEBUG

#endif

#endif