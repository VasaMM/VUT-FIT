/**
 * @file Coder.hpp
 * @author Václav Matinka
 * @date 07. 12. 2020 (19:15)
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>


using namespace std;

// Bit uchovavam jako bool
// Neni to efektivni, ale je to snadne...
typedef bool Bit;

// Bity uchovavam jako 64bit int
typedef uint64_t Bits;

// Par bitu uchovavam taky jako 8bit int
typedef uint8_t TwoBits;

// Cena cesty
typedef unsigned long long Price;


// Bit 0
#define BIT_0 0

// Bit 1
#define BIT_1 1


/**
 * @brief Ziska bit na konkretni pozici
 * 
 * @param bits Sada bitu
 * @param pos Pozice zadaneho bitu
 * 
 * @return True pokud je bit 1 jinak false
 */
inline const Bit bitAtPos(const Bits bits, const size_t pos) {
	return bits & (1ULL << pos);
}


/**
 * @brief Prevede sadu bitu na string
 * 
 * @param data Sada bitu
 * @param length Pocet uzitecnych bitu (aby vystup nebyl zbytecne dlouhy), neni-li zadano (nebo je zadana 0), vypisou se vsechny bity
 * 
 * @return String obsahujici 1 a 0
 */
const string bits2str(const Bits bits, size_t length = 0);


/**
 * @brief Prevede dvojici bitu na string
 * 
 * @param bits Vstupni dva bity
 * @return String (00, 01, 10, 11)
 */
inline const string twoBits2Str(const TwoBits bits) {return bits2str(Bits(bits), 2);}



/// Třída implementujici koder
class Coder {
private:
	// Velikost vnitrni pameti
	size_t _size;

	// Aktualni data
	Bits _data = 0, _upper, _lower;
	
	// Trellis diagram reprezentujici tento koder
	vector<vector<TwoBits>> _trellisDiagram;

	/**
	 * @brief XOR vsech bitu v `bits`
	 * @param bits Vstupni bity
	 * @return XOR vsech bitu v `bits`
	 */
	const Bit xorSum(const Bits bits);


public:
	/**
	 * @brief Konstruktor
	 * 
	 * @param _stages Velikost vnitrni pameti
	 * @param _upper Bitova reprezentace horni vazby
	 * @param _lower Bitova reprezentace dolni vazby
	 * @return Objekt koderu
	 */
	Coder (const size_t _stages, const Bits _upper, const Bits _lower);

	/**
	 * @brief Prevede objekt na string
	 * @return Coder reprezentovany jako string
	 */
	const string str();

	/**
	 * @brief Pristup k Trellis diagramu koderu
	 * @return Trellis diagram
	 */
	vector<vector<TwoBits>> &trellisDiagram() {return _trellisDiagram;}
	
	/**
	 * @brief Pristup ke konkretnimu vystupu z Trellis diagramu
	 * 
	 * @param state Vychozi stav
	 * @param by Bit na vstupu
	 * @return Vystupni bity
	 */
	TwoBits trellisDiagram(Bits state, Bit by) {return _trellisDiagram[state][by];}

	/**
	 * @brief Nacte bit do koderu
	 * 
	 * @param bit Vstupni bit (true pokud je to 1, jinak false)
	 * @return Vystup koderu po nacteni bitu
	 */
	const TwoBits load(const Bit bit);

	/**
	 * @brief Velikost vnitrniho registru
	 * @return Velikost vnitrniho registru
	 */
	const size_t size() {return _size;}

	/**
	 * @brief Hammingova vzdalenost mezi dvema sadami bitu
	 * 
	 * @param data_a Prvni sada bitu
	 * @param data_b Druha sada bitu
	 * @return Pocet rozdilnych bitu
	 */
	static const Price hammnigDistance(const Bits data_a, const Bits data_b);

	/**
	 * @brief Nacteni 0 do sady bitu
	 * 
	 * @param bits Vstupni sada bitu
	 * @return `bits` posunute o jedna doprava (vlevo doplena 0)
	 */
	static const Bits put0(Bits bits) {return bits >> 1;}

	/**
	 * @brief Nacteni 1 do sady bitu
	 * 
	 * @param bits Vstupni sada bitu
	 * @param size Pocet bitu na vstupu
	 * 
	 * @return `bits` posunute o jedna doprava a zleva doplneny 1
	 */
	static const Bits put1(Bits bits, size_t size) {return (bits >> 1) | (1 << (size - 1));}
};

