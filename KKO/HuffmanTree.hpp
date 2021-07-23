/**
 * @file HuffmanTree.hpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (14:13)
 * @brief Objekt reprezentující Huffmanův strom
 */

#ifndef HUFFMANTREE_HPP
#define HUFFMANTREE_HPP


#include <iostream>
#include <list>
#include <iomanip>
#include <vector>
#include <cstring>

#include "Bytes.hpp"
#include "File.hpp"
#include "Info.hpp"
#include "Exception.hpp"


using namespace std;


namespace Coder {


/// Konstanta pro nedefinované ID uzlu
const uint8_t NO_ID = 0;

/// Konstanta pro nedefinovanou frekvenci (četnost) uzlu
const uint NO_FREQUENCY = 0;

/// Konstanta pro nedefinového potomka uzlu
const int NO_CHILDREN = -1;

/// Konstanta pro nedefinového rodiče uzlu
const int NO_PARENT     = -1;

/// Konstanta pro nedefinový index
const int NO_INDEX = -1;


/// Frekvenční výskyt všech symbolů
typedef vector< ByteFrequency > Frequency;


/// Typ zdrojových dat pro vytvoření statického Huffmanova stromu
enum HuffmanTreeMakeFrom {
	COMPRESSED_INPUT,   ///< Komprimovaný vstup (strom se načte ze souboru)
	NORMAL_INPUT,       ///< Normální vstup (proběhne frekvenční analýza)
};


/// Struktura pro popis jednoho uzlu stromu
typedef struct {
	uint8_t id;       ///< ID symbolu (odpovida jeho hodnotě)
	uint frequency;   ///< Frekvence (četnost) symbolu
	int left;         ///< Levý potomek
	int right;        ///< Pravý potomek
	int parent;       ///< Rodič
} Node;




/// Třída popisující statický Huffmanův strom
class StaticHuffmanTree {
private:
	/// Strom je vnitřně reprezentován jako vektor
	vector< Node > tree;

	/**
	 * @brief Získám četnost výskytu jendotlivých znaků
	 * @param input Vstupní data
	 * @return Frekvence jednotlivých znaků
	 */
	Frequency get_frequency( vector< uint8_t > input );


	/**
	 * @brief Převede pole s výskytem jednotlivých znaků na seznam. Odmaže nulové výskyty a seznam seřadí podle četnoti.
	 * @param frequency Pole ziskané funkcí #get_frequency()
	 * @return Seřazený seznam bez nulových vyskytů
	 */
	list< Node > nodes_from_frequency( Frequency frequency );


	/**
	 * @brief Načte data ze souboru (vektoru znaků). Provede frekvenční analýzu a na jejím základě zkonstruuje strom.
	 * @param input Vstupní soubor
	 */
	void load_normal_input( vector< uint8_t > &input );


	/**
	 * @brief Načte data z komprimovaného souboru (vstupu). Zde očekává uložený strom.
	 * 
	 * @param input Vstupní soubor (data)
	 * @param file_pointer Index do souboru (vstupního pole) od kterého má probíat čtení, tento ukazatel je postupně inkrementován a po ukončení funkce ukazuje na začátek dat.
	 */
	void load_compressed_input( vector< uint8_t > &input, size_t *file_pointer );


public:
	/**
	 * @brief Konstruktor stromu
	 * 
	 * @param input Vstupní data (soubor)
	 * @param from Typ vstupních dat (normální/komprimovaná)
	 * @param file_pointer Jsou li vstupní data komprimovaná, je utné definovat pozici, od které jsou čteny
	 */
	StaticHuffmanTree( vector< uint8_t > &input, HuffmanTreeMakeFrom from, size_t *file_pointer = nullptr ) {
		if ( from == NORMAL_INPUT ) {
			load_normal_input( input );
		}
		else if ( from == COMPRESSED_INPUT ) {
			load_compressed_input( input, file_pointer );
		}
		else {
			throw Exception( UNSPECIFIED, "Unspecified HuffmanTreeMakeFrom in StaticHuffmanTree()!" );
		}
	}
	
	/// Implicitní destruktor
	~StaticHuffmanTree() {}


	/**
	 * @brief Získá uzel na zadaném indexu
	 * 
	 * @param index Index uzlu
	 * @return [description]
	 */
	Node get_node( uint index ) { return tree[ index ]; }
	

	/**
	 * @brief Reverzní průchod stromem. Výsledkem je vektor sloužící jako mapa pro přímý převod symbolu na kódový znak
	 * 
	 * @param output Ukazatel na výstupní vektor. Očekává se vektor o velikosti BYTE_COMBINATIONS
	 * @param id Ukazatel na kořen (pod)stromu. Implicitně 0.
	 */
	void decode_tree( CodeTable &output, uint id = 0 );


	/// Debugovací výpis aktuální struktury stromu
	void print();
};




/// Třída popisující adaptivní Huffmanův strom
class AdaptiveHuffmanTree {

public:
	/// Konstruktor stromu. Inicializuje výchozí hodnoty
	AdaptiveHuffmanTree() {
		tree  = vector< Node >( 2 * ( BYTE_COMBINATIONS + 1 ), { NO_ID, NO_FREQUENCY, NO_CHILDREN, NO_CHILDREN, NO_PARENT } );
		index = vector< int32_t >( BYTE_COMBINATIONS, -1 );
		_root_index = _NYT_index = 2 * BYTE_COMBINATIONS + 1;
	}

	/// Implicitní destruktor
	~AdaptiveHuffmanTree() {}

	/**
	 * @brief Získá uzel na daném indexu
	 * 
	 * @param index Index uzlu
	 * @return Uzel
	 */
	Node get_node( uint index ) { return tree[ index ]; }


	/// Debugovací výpis aktuální struktury stromu
	void print() {
		cout << str() <<endl; 
	}


	/**
	 * @brief Debugovací výpis aktuální struktury stromu jako string
	 * @return Struktura stromu jako string
	 */
	string str();


	/**
	 * @brief Otestuje existenci daného symbolu ve stromu
	 * @param symbol Symbol k otestování (očekává se jeden byte)
	 * @return Jeho existence
	 */
	bool exist( uint8_t symbol ) { return index[ symbol ] != -1; }


	/**
	 * @brief Získá index daného symbolu
	 * @param symbol Symbol k otestování (očekává se jeden byte)
	 * @return Jeho index
	 */
	int32_t find( uint8_t symbol ) { return index[ symbol ]; }


	/**
	 * @brief Přidá daný symbol do stromu
	 * @param symbol Symbol k otestování (očekává se jeden byte)
	 */
	void add( uint8_t symbol );


	/**
	 * @brief Aktualizuje daný symbol ve stromu => zvýší jeho frekevci o jedna
	 * @param symbol Symbol k otestování (očekává se jeden byte)
	 */
	void update( uint8_t symbol ) { update_index( find( symbol ) ); }


	/**
	 * @brief Aktualizuje symbol na daném indexu => zvýší jeho frekevci o jedna
	 * @param index Index symbolu
	 */
	void update_index( int32_t index );


	/**
	 * @brief Získá index NYT uzlu
	 * @return Index NYT uzlu
	 */
	int32_t NYT_index() { return _NYT_index; }


	/**
	 * @brief Otestuje, zda je daný uzel NYT
	 * @param index Index uzlu
	 * @return Je NYT?
	 */
	bool is_NYT( int32_t index ) { return index == _NYT_index; }
	

	/**
	 * @brief Získá index kořenového uzlu
	 * @return Index kořenového uzlu
	 */
	int32_t root_index() { return _root_index; }


	/**
	 * @brief Získá binární hodnotu (Huffmanův kód) konkrétního uzlu
	 * @param index Index uzlu
	 * @return Binární hodnota
	 */
	MaskedByte value( int32_t index );

	
private:
	/// STrom je vnitřně reprezentován jako vektor uzlů
	vector< Node > tree;

	
	/// Ukazatel na NYT uzel
	int32_t _NYT_index;


	/// Ukazatel na kořenový uzel
	int32_t _root_index;


	/// Převodník symbol/uzel
	vector< int32_t > index;


	/// Přesune NYT uzel
	void move_NYT() {
		tree[ NYT_index() - 2 ].parent = NYT_index();
		_NYT_index = _NYT_index < 2 ? 0 : _NYT_index - 2;
	}


	/**
	 * @brief Prohodí dva uzly na zadaných indexech
	 * 
	 * @param a Index prvního uzlu
	 * @param b Index druhého uzlu
	 */
	void swap( int32_t a, int32_t b );
};


}

#endif



