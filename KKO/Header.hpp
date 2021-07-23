/**
 * @file Header.hpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (15:14)
 * @brief Objekt reprezentující hlavičku komprimovaného souboru
 */

#ifndef HEADER_HPP
#define HEADER_HPP


#include <iostream>


using namespace std;
namespace Coder {



// 7654 3210
// 0   ... Statické/adaptivní huffmanovo kódováni
// 21  ... Nepoužito
// 543 ... Počet bitů na konci souboru, ktere se mají ingorovat (0 - 7)
// 6   ... Použito předzpracování pixelu
// 7   ... Následuje další hlavička
/// Vlatstnosti uložené v hlavičce
enum HeaderProp: uint8_t {
	HUFFMAN_ADAPTIVE   = 0b00000001,   ///< Bylo použito daptivní Huffmanovo kódování
	PIXEL_PREPROCESSOR = 0b00000010,   ///< Použito předzpracování pixelu
	TO_IGNORE          = 0b00011100,   ///< Počet bitů na konci souboru, ktere se mají ingorovat (0 - 7)
	NONE_1             = 0b00100000,   ///< Nepoužito
	NONE_2             = 0b01000000,   ///< Nepoužito
	EXTENDED_HEADER    = 0b10000000,   ///< Následuje další hlavička
	ALL_PROP           = 0b11111111,   ///< Všechny hodnoty
};

const uint8_t TO_IGNORE_OFFSET = 2;



/// Objekt pro práci s hlavičkou
class Header {
	/// Obsah hlavičky
	uint8_t header;


public:
	/// Implcitní konstruktor
	Header() { header = 0; }

	/**
	 * @brief Konstruktor z hodnoty
	 * @param data Obsah hlavičky
	 */
	Header( uint8_t data ) { header = data; }


	/**
	 * @brief Získá hodnotu celé hlavičky
	 * @return Hodnota hlavičky
	 */
	uint8_t get() { return header; };

	/**
	 * @brief Nastaví konkrétní vlastnost
	 * 
	 * @param prop Vlastnost
	 * @param value Hodnota
	 */
	void    set_prop( HeaderProp prop, uint8_t value );
	

	/**
	 * @brief Získá konkrétní vlastnost
	 * 
	 * @param prop Vlastnost
	 * @return Hodnota
	 */
	uint8_t get_prop( HeaderProp prop );
};


}

#endif



