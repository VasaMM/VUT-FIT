/**
 * @file Header.cpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (15:14)
 * @brief Objekt reprezentující hlavičku komprimovaného souboru
 */

#include <iostream>

#include "Header.hpp"
#include "Info.hpp"


using namespace std;
using namespace Coder;


void Header::set_prop( HeaderProp prop, uint8_t value ) {
	// Vyjimka pro zapsani vice bitu
	if ( prop == TO_IGNORE ) {
		header = ( header & ( ~prop ) ) | ( value << TO_IGNORE_OFFSET );
		return;
	}


	if ( value ) {
		header |= prop;
	}
	else {
		header = ( header & ( ~prop ) );
	}
}


uint8_t Header::get_prop( HeaderProp prop ) {
	// Vyjimka pro cteni vice bitu
	if ( prop == TO_IGNORE ) {
		return ( header & prop ) >> TO_IGNORE_OFFSET;
	}

	return header & prop;
}
