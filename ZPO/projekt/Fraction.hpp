/**
 * @Author: Vaclav Martinka
 * @Date:   05. 04. 2019 (09:11)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: Fraction.hpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:31)
 * @License: CC BY-SA 3.0
 */



#ifndef FRACTION_HPP
#define FRACTION_HPP

#include <string>
#include <ostream>
#include <QString>


// Třída pro vykreslení zlomků
class Fraction {

public:
	 Fraction( int a, int b );
	 Fraction( double number );
	~Fraction() {}

	operator std::string() const { return to_string(); }
	operator QString() const { return QString::fromStdString( to_string() ); }

	std::string to_string() const;


private:
	double val;

};


#endif
