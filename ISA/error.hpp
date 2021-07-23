/* * * * * * * * * * * * * * * * * * *
 * AUTOR: Vaclav Martinka (xmarti76)
 * Funkce pro tisk chybovych hlasek

 * 17. 11. 2017 FIT VUT
 * * * * * * * * * * * * * * * * * * */

#ifndef ERROR_HPP
#define ERROR_HPP 

#include <iostream>

inline void print_error( std::string error = "" ) {
	std::cerr << "ERROR: " << error << std::endl;
}
inline void exit_error( std::string error = "", int code = 1 ) {
	std::cerr << "ERROR: " << error << std::endl;
	exit ( code );
}
inline void exit( std::string error = "", int code = 1 ) {
	std::cerr << error << std::endl;
	exit ( code );
}

#endif