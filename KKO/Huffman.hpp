/**
 * @file Huffman.hpp
 * @author Václav Matinka
 * @date 08. 04. 2019 (12:16)
 * @brief Třídy  pro kódování a dekódovaní do/z statického a adqaptivního Huffmanova kódu
 */

#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP


#include <vector>

#include "File.hpp"
#include <iostream>


using namespace std;


namespace Coder {


/// Třída pro kódování/dekóování pomocí statického Huffmanova kódu
class StaticHuffman {
public:
	/// Konstruktor
	StaticHuffman() {}

	/// Destruktor
	~StaticHuffman() {}


	/**
	 * @brief Zakóduje daný vstup
	 * 
	 * @param input Vstupní soubor
	 * @param output Výstupní komprimovaný soubor
	 */
	void code( InputFile &input, OutputFile &output ) { code( input.get_data(), output ); }


	/**
	 * @brief Dekóduje daný vstup
	 * 
	 * @param input Vstupní komprimovaný soubor
	 * @param output Výstupní dekomprimovaný soubor
	 */
	void decode( InputFile &input, OutputFile &output ) { decode( input.get_data(), output ); }


private:
	/**
	 * @brief Zakóduje daný vstup
	 * 
	 * @param input Vstupní soubor jako vektor
	 * @param output Výstupní komprimovaný soubor
	 */
	void code( vector< uint8_t > *input, OutputFile &output );
	

	/**
	 * @brief Dekóduje daný vstup
	 * 
	 * @param input Vstupní komprimovaný soubor jako vektor
	 * @param output Výstupní dekomprimovaný soubor
	 */
	void decode( vector< uint8_t > *input, OutputFile &output );
};


/// Třída pro kódování/dekóování pomocí adaptivního Huffmanova kódu
class AdaptiveHuffman {
public:
	/// Konstruktor
	AdaptiveHuffman() {}

	/// Destruktor
	~AdaptiveHuffman() {}


	/**
	 * @brief Zakóduje daný vstup
	 * 
	 * @param input Vstupní soubor
	 * @param output Výstupní komprimovaný soubor
	 */
	void code( InputFile &input, OutputFile &output ) { code( input.get_data(), output ); }


	/**
	 * @brief Dekóduje daný vstup
	 * 
	 * @param input Vstupní komprimovaný soubor
	 * @param output Výstupní dekomprimovaný soubor
	 */
	void decode( InputFile &input, OutputFile &output ) { decode( input.get_data(), output ); }


private:
	/**
	 * @brief Zakóduje daný vstup
	 * 
	 * @param input Vstupní soubor jako vektor
	 * @param output Výstupní komprimovaný soubor
	 */
	void code( vector< uint8_t > *input, OutputFile &output );
	

	/**
	 * @brief Dekóduje daný vstup
	 * 
	 * @param input Vstupní komprimovaný soubor jako vektor
	 * @param output Výstupní dekomprimovaný soubor
	 */
	void decode( vector< uint8_t > *input, OutputFile &output );
};



}


#endif



