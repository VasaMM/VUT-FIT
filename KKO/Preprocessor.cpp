/**
 * @file Preprocessor.cpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (11:10)
 * @brief 
 */

// #include <iostream>
// #include <vector>
// #include <list>
// #include <cmath>

// #include "HuffmanTree.hpp"
// #include "File.hpp"
// #include "Bytes.hpp"
// #include "Header.hpp"
#include "Info.hpp"
#include "Preprocessor.hpp"


using namespace std;
using namespace Coder;


void Preprocessor::pixelDifferencion( InputFile &input ){
	vector< uint8_t > *f = input.get_data();

	uint8_t actual_byte;
	uint8_t prev_byte = f->at( 0 );
	for ( size_t i = 1 ; i < f->size() ; ++i ) {
		actual_byte = f->at( i );
		
		f->at( i ) = actual_byte - prev_byte;
		
		prev_byte = actual_byte;
	}
}


void Preprocessor::pixelDifferencion( OutputFile &output ){
	vector< uint8_t > *f = output.get_data();

	for ( size_t i = 1 ; i < f->size() ; ++i ) {
		f->at( i ) = f->at( i ) + f->at( i - 1 );
	}
}
