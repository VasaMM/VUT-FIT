/**
 * @file Bytes.cpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (14:35)
 * @brief Podpora pro práci s byty a bity
 */

#include <iostream>
#include <vector>

#include "Bytes.hpp"
#include "Info.hpp"


using namespace std;
using namespace Coder;



// Prevede 32 bit int na binární string
string Coder::to_binstring( uint32_t input, uint8_t size, uint8_t from ) {
	string output = "";
	
	uint32_t mask = 1 << from;

	for ( uint8_t i = 0 ; i < size ; ++i ) {
		output = ( input & mask ? "1" : "0" ) + output;
		mask = mask << 1;
	}

	return output;
}



//Převede strukturu #MaskedByte na string
string Coder::to_binstring( MaskedByte input ) { return to_binstring( input.val, input.offset ); }

//Převede 32 bitový int na binární string
string Coder::to_binstring( uint32_t input ) { return to_binstring( input, 32 ); }

//Převede 16 bitový int na binární string
string Coder::to_binstring( uint16_t input ) { return to_binstring( input, 16 ); }

//Převede 8 bitový int na binární string
string Coder::to_binstring( uint8_t  input ) { return to_binstring( input, 8 ); }



bool BiteReader::get_bite( bool &file_end ) {
	// Vystup
	bool output = (*data)[ index ] & mask;

	// Ctu posledni byte
	if ( index + 1 == data->size() ) {
		// Buď jsem precetl vsechny bity nebo jsem dorazil na ignorovaci zarazku
		if ( mask == 0b00000001 || mask == ignore ) {
			file_end = true;
			return output;
		}
	}

	// Zpracoval jsem byte, nactu dalsi
	if ( mask == 0b00000001 ) {
		++index;
		mask = mask << 7;
	}
	else {
		mask = mask >> 1;
	}
	return output;
}