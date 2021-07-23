/**
 * @file Huffman.cpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (14:34)
 * @brief Implementace třídy \c Huffman pro kódování a dekódovaní do/z statického a adqaptivního Huffmanova kódu
 */

#include <cmath>
#include <iostream>
#include <vector>

#include "Huffman.hpp"
#include "HuffmanTree.hpp"
#include "Header.hpp"
#include "File.hpp"
#include "Bytes.hpp"
#include "Info.hpp"


using namespace std;
using namespace Coder;



/**
 * @brief Vyprízdní buffer do souboru
 * 
 * @param buffer Reference na buffer
 * @param output Reference na výstupní soubor
 * @param bites Reference na počítadlo bitů
 * @param file_pointer Reference na ukazatel do souboru
 */
inline void buffer_flush( uint32_t &buffer, OutputFile &output, uint8_t &bites, uint &file_pointer ) {
	while ( bites >= 8 ) {
		output.write( file_pointer, &buffer, 1, true );
		++file_pointer;

		// Posunu vlevo a snizim pocet bytu
		buffer = buffer << 8;
		bites -= 8;
	}
}





// Koduji
void StaticHuffman::code( vector< uint8_t > *input, OutputFile &output ) {
	StaticHuffmanTree tree( *input, NORMAL_INPUT );
	
	// TODO Pridat kontrolu na maximalni hloubku stromu 32

	CodeTable codes( BYTE_COMBINATIONS, { 0, 0 } );
	tree.decode_tree( codes );


	// Ukazatel do souboru pro zapis
	// Vynecham prvni byte pro hlavicku
	uint file_pointer = 1;
	

	// Nastavim hlavicku
	Header header( output.read( 0 ) );

	
	// Postune ulozim vsechny hodnoty ze stromu
	int8_t ignore = 0;
	for ( size_t i = 0 ; i < BYTE_COMBINATIONS ; ++i ) {
		if ( codes[ i ].offset ) {
			uint8_t offset = codes[ i ].offset;
			uint8_t byte_size = ceil( codes[ i ].offset / 8.0 );

			if ( ignore > 0 ) {
				output.write( file_pointer++, -ignore );
				ignore = 0;
			}

			output.write( file_pointer, offset );
			output.write( file_pointer + 1, &codes[ i ].val, byte_size );
			file_pointer += byte_size + 1;
		}
		else {
			++ignore;

			if ( ignore == 127 ) {
				output.write( file_pointer++, -ignore );
				ignore = 0;
			}
		}
	}
	if ( ignore > 0 ) {
		output.write( file_pointer++, -ignore );
		ignore = 0;
	}


	// Smaotne kodovani
	uint32_t buffer = 0;
	uint8_t  bites  = 0;
	for ( size_t i = 0 ; i < input->size() ; ++i ) {
		// Posunu a pripojim data
		bites += codes[ input->at( i ) ].offset;
		buffer |= codes[ input->at( i ) ].val << ( ( sizeof( buffer ) << 3 ) - bites );   // << 3 == *8

		buffer_flush( buffer, output, bites, file_pointer );
	}

	// Zapisu buffer do souboru
	output.write( file_pointer, &buffer, ceil( bites / 8.0 ), true );

	// Zapisu pocet bitu k ingorovani do hlavicky
	header.set_prop( TO_IGNORE, bites % 8 ? 8 - ( bites % 8 ) : 0 );

	// Zapisu hlavicku do souboru
	output.write( 0, header.get() );
}



void StaticHuffman::decode( vector< uint8_t > *input, OutputFile &output ) {
	size_t file_pointer = 1;

	StaticHuffmanTree tree( *input, COMPRESSED_INPUT, &file_pointer );

	// Zpracuji hlavicku
	Header header( input->at( 0 ) );


	CodeTable codes( BYTE_COMBINATIONS, { 0, 0 } );
	tree.decode_tree( codes );


	// Inicializuji nacitani bitu
	BiteReader reader( input, header.get_prop( TO_IGNORE ), file_pointer );
	
	// TODO kontorlu na chyby
	// Dekoduji
	int  tree_index = 0;
	uint i = 0;
	for ( bool file_end = false ; ! file_end ; ) {
		// Jdu vlevo nebo vpravo
		if ( reader.get_bite( file_end ) ) {
			tree_index = tree.get_node( tree_index ).left;
		}
		else {
			tree_index = tree.get_node( tree_index ).right;
		}

		// Narazil jsem na konec
		if ( tree.get_node( tree_index ).left == NO_CHILDREN && tree.get_node( tree_index ).right == NO_CHILDREN ) {
			output.write( i++, tree.get_node( tree_index ).id );
			tree_index = 0;
		}
	}
}









// Koduji
void AdaptiveHuffman::code( vector< uint8_t > *input, OutputFile &output ) {
	AdaptiveHuffmanTree tree;
	
	// Ukazatel do souboru pro zapis
	// Vynecham prvni byte pro hlavicku
	uint file_pointer = 1;
	

	// Nastavim hlavicku
	Header header( output.read( 0 ) );
	header.set_prop( HUFFMAN_ADAPTIVE, true );


	uint32_t buffer = 0;
	uint8_t  bites  = 0;
	for ( size_t i = 0 ; i < input->size() ; ++i ) {
		// Symbol uz je ve strome zastoupen
		// Vypisu jeho kod a aktualizuji strom
		if ( tree.exist( input->at( i ) ) ) {
			MaskedByte byte = tree.value( tree.find( input->at( i ) ) );

			// Posunu a pripojim data
			bites += byte.offset;
			buffer |= byte.val << ( ( sizeof( buffer ) << 3 ) - bites );   // << 3 == *8

			buffer_flush( buffer, output, bites, file_pointer );

			tree.update( input->at( i ) );
		}
		else {
			MaskedByte byte = tree.value( tree.NYT_index() );

			bites += byte.offset;
			buffer |= byte.val << ( ( sizeof( buffer ) << 3 ) - bites );   // << 3 == *8

			buffer_flush( buffer, output, bites, file_pointer );

			bites += 8;
			buffer |= input->at( i ) << ( ( sizeof( buffer ) << 3 ) - bites );   // << 3 == *8

			buffer_flush( buffer, output, bites, file_pointer );
			
			tree.add( input->at( i ) );
		}
	}
	// Zapisu buffer do souboru
	output.write( file_pointer, &buffer, ceil( bites / 8.0 ), true );

	// Zapisu pocet bitu k ingorovani do hlavicky
	header.set_prop( TO_IGNORE, bites % 8 ? 8 - ( bites % 8 ) : 0 );

	// Zapisu hlavicku do souboru
	output.write( 0, header.get() );
}




void AdaptiveHuffman::decode( vector< uint8_t > *input, OutputFile &output ) {
	AdaptiveHuffmanTree tree;

	// Zpracuji hlavicku
	Header header( input->at( 0 ) );

	// Inicializuji nacitani bitu
	// 1 byte je hlavicka
	// 2 byte je nekodovany, proto je muzu preskocit
	BiteReader reader( input, header.get_prop( TO_IGNORE ), 2 );

	
	// Nactu prvni symbol
	tree.add( input->at( 1 ) );
	output.write( 0, input->at( 1 ) );


	// Dekoduji
	int32_t  tree_index = tree.root_index();
	size_t i = 1;
	bool file_end = false;
	while ( ! file_end ) {
		// Jdu vlevo nebo vpravo
		if ( reader.get_bite( file_end ) ) {
			tree_index = tree.get_node( tree_index ).right;
		}
		else {
			tree_index = tree.get_node( tree_index ).left;
		}

		// Narazil jsem na konec
		if ( tree.get_node( tree_index ).left == NO_CHILDREN  ) {
			// Nacten novy symbol
			if ( tree.is_NYT( tree_index ) ) {
				uint8_t byte =
					reader.get_bite( file_end ) << 7 |
					reader.get_bite( file_end ) << 6 |
					reader.get_bite( file_end ) << 5 |
					reader.get_bite( file_end ) << 4 |
					reader.get_bite( file_end ) << 3 |
					reader.get_bite( file_end ) << 2 |
					reader.get_bite( file_end ) << 1 |
					reader.get_bite( file_end ) << 0;

				output.write( i++, byte );
				tree.add( byte );
			}
			// Nacten existujici symbol
			else {
				output.write( i++, tree.get_node( tree_index ).id );
				tree.update_index( tree_index );
			}

			tree_index = tree.root_index();
			// TODO Pridat kontrolu na maximalni hloubku stromu 32
		}
	}
}
