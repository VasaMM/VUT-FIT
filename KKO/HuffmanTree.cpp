/**
 * @file HuffmanTree.cpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (14:13)
 * @brief Objekt reprezentující Huffmanův strom
 */

#include <iostream>
#include <vector>
#include <list>
#include <cmath>

#include "HuffmanTree.hpp"
#include "File.hpp"
#include "Bytes.hpp"
#include "Header.hpp"
#include "Info.hpp"


/// Prohodí dvě číselné hodnoty
#define SWAP( a, b ) a = (a) + (b); b = (a) - (b); a = a - b;


using namespace std;
using namespace Coder;


/**
 * @brief Řadící funkce pro dva uzly
 * 
 * @param first První prvek
 * @param second Druhý prvek
 * 
 * @return Je první prvek větší než druhý
 */
bool sort_function( const Node& first, const Node& second ) { return ( first.frequency > second.frequency ); }



void StaticHuffmanTree::print() {
	cout << "  i     ID  freq  left right" << endl;
	for ( size_t i = 0 ; i < tree.size() ; ++i ) {
		Node n = get_node( i );
		cout << setw( 3 ) << i << ": { " << setw( 3 ) << (int)n.id << ", " << setw( 5 ) << n.frequency << ", " << setw( 3 ) << n.left << ", " << setw( 3 ) << n.right << " }" << endl;
	}
}



void StaticHuffmanTree::load_normal_input( vector< uint8_t > &input ) {
	Frequency frequency = get_frequency( input );
	list< Node > nodes = nodes_from_frequency( frequency );

	tree = vector< Node >( 1 );   // Rezervuji misto pro koren

	while( nodes.size() > 1 ) {
		Node center;
		Node *left, *right;

		tree.reserve( tree.size() + 2 );
		tree.push_back( nodes.back() );
		nodes.pop_back();
		left = &tree.back();
		
		tree.push_back( nodes.back() );
		nodes.pop_back();
		right = &tree.back();

		center.id = NO_ID;
		center.left  = tree.size() - 1;
		center.right = tree.size() - 2;
		center.frequency = left->frequency + right->frequency;

		nodes.push_back( center );

		// FIXME optimize
		nodes.sort( sort_function );
	}
	
	// Presunu koren
	tree.front() = nodes.front();
	nodes.pop_back();
}



void StaticHuffmanTree::load_compressed_input( vector< uint8_t > &input, size_t *file_pointer ) {
	// Vektor nactenych bytu a jejich kodu
	vector< MaskedByteWithID > vals;

	for ( uint id = 0 ; id < BYTE_COMBINATIONS ; ) {
		// Ziskam velikost symbolu v bytech
		int8_t offset = static_cast< int8_t >( input[ *file_pointer ] );

		if ( offset > 0 ) {
			uint8_t byte_size = ceil( offset / 8.0 );

			uint32_t val = 0;
			switch ( byte_size ) {
				case 4:  val |= input[ *file_pointer + 4 ] << 24;
				case 3:  val |= input[ *file_pointer + 3 ] << 16;
				case 2:  val |= input[ *file_pointer + 2 ] << 8;
				case 1:  val |= input[ *file_pointer + 1 ];
				break;

				default: throw Exception( TOO_BIG_SYMBOL, "Input file contains too big symbol (more then 32 bits)!" );
			}

			vals.push_back( { static_cast< uint8_t >( id++ ), val, static_cast< uint8_t >( offset ) } );
			
			*file_pointer += byte_size + 1;
		}
		// Zaporna hodnota znaci pocet preskocenych symbolu
		else  {
			id += -offset;
			*file_pointer += 1;
		}
	}


	// Vytvorim strom
	tree = vector< Node >( 1, { NO_ID, NO_FREQUENCY, NO_CHILDREN, NO_CHILDREN } );   // Rezervuji misto pro koren
	

	// Iteruji pres vstup
	for ( uint i = 0; i < vals.size() ; ++i ) {
		int  new_index, index = 0;  // Ukazatele do stromu

		// Postupne projdu strom a vytvorim chybejici uzly
		for ( uint32_t mask = 1 << ( vals[i].offset - 1 ) ; mask > 1 ; mask = mask >> 1, index = new_index ) {
			// Jdu vlevo
			if ( vals[i].val & mask ) { new_index = tree[ index ].left; }
			// Jdu vpravo
			else { new_index = tree[ index ].right; }

			// Vytvorim uzel
			if ( new_index == NO_CHILDREN ) {
				new_index = tree.size();
				tree.push_back( { NO_ID, NO_FREQUENCY, NO_CHILDREN, NO_CHILDREN } );
				
				// Pripojim ho do stromu
				if ( vals[i].val & mask ) { tree[ index ].left = new_index; }
				else { tree[ index ].right = new_index; }
			}
		}

		// Vlozim list
		tree.push_back( { vals[ i ].id, NO_FREQUENCY, NO_CHILDREN, NO_CHILDREN } );

		if ( vals[i].val & 0x01 ) { tree[ index ].left = tree.size() - 1; }
		else { tree[ index ].right = tree.size() - 1; }
	}
}



// Prevod stromu na mapu
void StaticHuffmanTree::decode_tree( CodeTable &output, uint id ) {
	static MaskedByte mask = { 0, 0 };

	if ( get_node( id ).left > NO_CHILDREN ) {
		mask.val = mask.val << 1;
		mask.val = mask.val | 0x01;
		++mask.offset;
		
		decode_tree( output, get_node( id ).left );
	}
	// Vyjimka pro soubory tvorene jednim znakem
	else if ( id == 0 ) {
		output[ get_node( id ).id ] = { 0, 1 };
	}
	else {
		output[ get_node( id ).id ] = mask;
	}

	if ( get_node( id ).right > NO_CHILDREN ) {
		mask.val  = mask.val << 1;
		++mask.offset;

		decode_tree( output, get_node( id ).right );
	}

	mask.val  = mask.val >> 1;
	--mask.offset;

	// Obnovim funkci pro dalsi pouziti
	if ( id == 0 ) {
		mask = { 0,0 };
	}
}



// Ziska cetnost znaku
Frequency StaticHuffmanTree::get_frequency( vector< uint8_t > input ) {
	Frequency frequency;

	// Pripravim si vector
	frequency.reserve( BYTE_COMBINATIONS );
	for ( uint i = 0 ; i < BYTE_COMBINATIONS ; ++i ) {
		frequency.push_back( ByteFrequency{ static_cast< uint8_t >( i ), 0 } );
	}	

	// Iteruji pres vstup a zvysuji cetnost u jednotlivych znaku
	for ( size_t i = 0 ; i < input.size() ; ++i ) {
		++frequency[ input[ i ] ].frequency;
	}

	return frequency;
}



// Vytvorim usporadany seznam
list< Node > StaticHuffmanTree::nodes_from_frequency( Frequency frequency ) {
	list< Node > nodes;

	for ( uint i = 0; i < BYTE_COMBINATIONS ; ++i ) {
		if ( frequency.back().frequency > 0 ) {
			Node tmp;
			tmp.frequency = frequency.back().frequency;
			tmp.id        = frequency.back().id;
			tmp.left      = NO_CHILDREN;
			tmp.right     = NO_CHILDREN;

			nodes.push_back( tmp );
		}

		frequency.pop_back();
	}

	// Seradim
	nodes.sort( sort_function );

	return nodes;
}



void AdaptiveHuffmanTree::add( uint8_t symbol ) {
	// Vytvorim novy uzel s potomky
	tree[ NYT_index() ].left  = NYT_index() - 2;
	tree[ NYT_index() ].right = NYT_index() - 1;

	// Pravy potomek je pridavany symbol
	tree[ NYT_index() - 1 ] = { symbol, 0, NO_CHILDREN, NO_CHILDREN, NYT_index() };
	index[ symbol ] = NYT_index() - 1;

	move_NYT();

	// Musim aktualizovat strom
	update_index( NYT_index() + 1 );
}



void AdaptiveHuffmanTree::update_index( int32_t index ) {
	// DEBUG_MSG("aktualizuji", index);
	Node *n = &tree[ index ];
	// Ukoncujici podminka
	if ( n->parent != NO_PARENT ) {
		// Pokud by se porusila rovnovaha, musim to opravit
		if ( n->frequency >= tree[ index + 1 ].frequency ) {
			// Najdu nejpravejsi uzel se stejnou frekvenci
			int32_t new_index = index;
			while ( n->frequency >= tree[ new_index ].frequency ) {
				if ( tree[ new_index ].parent == NO_PARENT ) {
					break;
				}
				++new_index;
			}
			
			--new_index;   // Vratim se o krok zpet

			// Nesmim se prohodit se svym rodicem!
			if ( n->parent != new_index && new_index != index ) {
				swap( new_index, index );
				n = &tree[ new_index ];
			}

		}

		// Zvysim frekvenci symbolu
		++( n->frequency );

		// Totez provedu s rodici
		update_index( n->parent );
	}
	else {
		++( n->frequency );
	}
}



void AdaptiveHuffmanTree::swap( int32_t a, int32_t b ) {
	// Nemam potomky -> musim opravit ukazatel v indexu
	if ( tree[ a ].left == NO_CHILDREN ) {
		index[ tree[ a ].id ] = b;
	}
	if ( tree[ b ].left == NO_CHILDREN ) {
		index[ tree[ b ].id ] = a;
	}

	// Prohodim uzly
	Node tmp = tree[ a ];

	tree[ a ] = tree[ b ];
	tree[ b ] = tmp;

	// Prohodim jejich rodice
	SWAP( tree[ a ].parent, tree[ b ].parent );

	// Musim opravit rodice u potomku
	if ( tree[ a ].left != NO_CHILDREN ) {
		tree[ tree[ a ].left  ].parent = a ;
		tree[ tree[ a ].right ].parent = a ;
	}
	
	if ( tree[ b ].left != NO_CHILDREN ) {
		tree[ tree[ b ].left  ].parent = b ;
		tree[ tree[ b ].right ].parent = b ;
	}
}



MaskedByte AdaptiveHuffmanTree::value( int32_t index ) {
	MaskedByte output = { 0, 0 };

	MaskedByte data = { 1, 0 };

	for ( int32_t i = index ; tree[ i ].parent != NO_PARENT ; i = tree[ i ].parent ) {
		++output.offset;

		if ( tree[ tree[ i ].parent ].right == i ) {
			output.val |= data.val;
		}
		data.val = data.val << 1;
	}

	return output;
}


string AdaptiveHuffmanTree::str() {
	ostringstream stream;

	stream << "NYT index: " << _NYT_index << endl;
	stream << "  i     ID  freq  left right pare" << endl;
	
	for ( size_t i = _NYT_index ; i < tree.size() ; ++i ) {
		Node n = get_node( i );
		stream << "#" << setw( 3 ) << i << ": { " << setw( 3 ) << (int)n.id << ", " << setw( 5 ) << n.frequency << ", " << setw( 3 ) << n.left << ", " << setw( 3 ) << n.right << ", " << setw( 3 ) << n.parent << " }" << endl;
	}

	stream << endl;
	
	for ( size_t i = 0 ; i < index.size() ; ++i ) {
		if ( index[i] != NO_INDEX ) {
			stream << setw( 3 ) << (int)i << " => " << setw( 3 ) << (int)index[ i ] << endl;
		}
	}

	return stream.str();
}
