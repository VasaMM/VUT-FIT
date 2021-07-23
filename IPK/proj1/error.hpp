/* * * * * * * * * *
* AUTOR: xmarti76
* Error pro IPK
* 08. 03. 2017
* * * * * * * * * */

#ifndef ERROR_HPP
#define ERROR_HPP 

#include <iostream>

inline void print_error( std::string error ) {
	std::cerr << "ERROR: " << error << std::endl;
}
inline void exit_error( std::string error, int code = 1 ) {
	std::cerr << "ERROR: " << error << std::endl;
	exit ( code );
}
inline void exit( std::string error, int code = 1 ) {
	std::cerr << error << std::endl;
	exit ( code );
}

#endif