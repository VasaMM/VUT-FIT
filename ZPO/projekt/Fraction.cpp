/**
 * @Author: Vaclav Martinka
 * @Date:   05. 04. 2019 (09:11)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: Fraction.cpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:31)
 * @License: CC BY-SA 3.0
 */



#include <string>
#include <cmath>

#include "Fraction.hpp"


Fraction::Fraction( double number ) {
	val = number;
}


Fraction::Fraction( int a, int b ) {
	val = 1.0  * a / b;
}


std::string Fraction::to_string() const {
	if ( val < 1 ) {
		return "1/" + std::to_string( static_cast< int >( ceil( 1 / val ) ) );
	}
	return std::to_string( val );
}
