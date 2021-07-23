/**
 * @file Bytes.hpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (14:35)
 * @brief Podpora pro práci s byty a bity
 */

#ifndef BYTE_HPP
#define BYTE_HPP


#include <iostream>
#include <vector>

using namespace std;
namespace Coder {




/// Počet možnách kombinací uložitelných do jednoho bytu => 2^8
const uint BYTE_COMBINATIONS = 256;


/// Převede velikost symbolu na masku
const uint8_t OFFSET_TO_MASK[9] = {
	0b00000000,  ///< 0
	0b00000001,  ///< 1
	0b00000011,  ///< 2
	0b00000111,  ///< 3
	0b00001111,  ///< 4
	0b00011111,  ///< 5
	0b00111111,  ///< 6
	0b01111111,  ///< 7
	0b11111111,  ///< 8
};


/// Převede čislo na binarni reprezentaci
const uint8_t IGNORE_TO_MASK[8] = {
	0b00000001,  ///< 0
	0b00000010,  ///< 1
	0b00000100,  ///< 2
	0b00001000,  ///< 3
	0b00010000,  ///< 4
	0b00100000,  ///< 5
	0b01000000,  ///< 6
	0b10000000,  ///< 7
};


/// Struktura rprezentujici jeden byte a jeho frekvenci
typedef struct ByteFrequency {
    uint8_t id;       ///< ID (hodnota) symbolu
    uint frequency;   ///< Jeho frekvence
} ByteFrequency;


/// Struktura pro uchování maskovaného bytu
typedef struct {
	uint32_t val;      ///< Hodnota bytu
	uint8_t  offset;   ///< Počet platných bitů
} MaskedByte;


/// Struktura pro uchování maskovaného bytu
typedef struct {
    uint8_t  id;       ///< ID (hodnota) symbolu
	uint32_t val;      ///< Hodnota bytu
	uint8_t  offset;   ///< Počet platných bitů
} MaskedByteWithID;



/// Kódovací tabulka symbol -> byte
typedef vector< MaskedByte > CodeTable;


/**
 * @brief Převede až 332 bitů dlouhý int na binární string
 * @param input Int k převodu
 * @param size  Počet bitů ke čtení
 * @param from  Počet bitů k ignorování
 * @return Výsledný string
 */
string to_binstring( uint32_t input, uint8_t size, uint8_t from = 0 );


/**
 * @brief Převede strukturu MaskedByte na string
 * @param input Maskovaný byte k převodu
 * @return Výsledný string
 */
string to_binstring( MaskedByte input );


/**
 * @brief Převede 32 bitový int na binární string
 * @param input Int k převodu
 * @return Výsledný string
 */
string to_binstring( uint32_t input );


/**
 * @brief Převede 16 bitový int na binární string
 * @param input Int k převodu
 * @return Výsledný string
 */
string to_binstring( uint16_t input );


/**
 * @brief Převede 8 bitový int na binární string
 * @param input Int k převodu
 * @return Výsledný string
 */
string to_binstring( uint8_t input );


/// Třída pro čtení souboru (vstupu) po jednom bitu
class BiteReader {
private:
	/// Ukazatel do vstupních dat
	uint index;
	
	/// Počet koncových bitů k ignorovaní
	uint8_t ignore;

	/// Aktualní maska
	uint8_t mask = 0b10000000;

	/// Ukazatel na čtená data
	vector< uint8_t > *data;

public:
	/**
	 * @brief Objekt pro čtení vstupu bit po bitu
	 * 
	 * @param input Vector bitů ke čtení
	 * @param to_ignore Počet posledních bitů k ignorování
	 * @param start Index od kterého se začnou číst byty z vectoru input
	 */
	BiteReader( vector< uint8_t > *input, uint8_t to_ignore = 0, size_t start = 0 ) {
		index  = start;
		data   = input;
		ignore = IGNORE_TO_MASK[ to_ignore ];
	}


	/// Implicitní destruktor
	~BiteReader() {}
	

	/**
	 * @brief Načte další bit ze vstupu
	 * 
	 * @param file_end Ukazatel na flag, jestli byl načten celý soubor. Pokud ano, nastaví se true, jinak se nemění.
	 * @return True nebo false, pokud byl bit 1 nebo 0. POZOR nevrací hodnoty 1 a 0!
	 */
	bool get_bite( bool &file_end );
	

	/**
	 * @brief Načte jeden byte ze vstupu
	 * 
	 * @param file_end Ukazatel na flag, jestli byl načten celý soubor. Pokud ano, nastaví se true, jinak se nemění.
	 * @return Načtený byte
	 */
	uint8_t get_byte( bool &file_end );
};



}

#endif



