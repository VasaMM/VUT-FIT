/**
 * @file Preprocessor.hpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (11:10)
 * @brief Objekt pro předzpracování vstupních dat
 */

#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP


// #include <iostream>
// #include <list>
// #include <iomanip>
// #include <vector>
// #include <cstring>

// #include "Bytes.hpp"
#include "File.hpp"
// #include "Info.hpp"
// #include "Exception.hpp"


using namespace std;


namespace Coder {


/// Třída pro předzpracování obrazu
class Preprocessor {
private:
	Preprocessor();

public:
	/**
	 * @brief Přepíše data ve vstupním souboru na základu rozdílu hodnot oproti předchozímu bytu
	 * @param input Vastupní soubor
	 */
	static void pixelDifferencion( InputFile &input );
	
	/**
	 * @brief Opraví výstpní data z rozdílu hodnot na původní hodnoty 
	 * @param output Výstupní soubor
	 */
	static void pixelDifferencion( OutputFile &output );
};



}

#endif



