/**
 * @file File.cpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (14:34)
 * @brief Implementace tříd \c File pro práci se soubory
 */

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "Exception.hpp"
#include "File.hpp"
#include "Info.hpp"


using namespace std;
using namespace Coder;


InputFile::InputFile ( string addr ) {
	info.say( "Open input " + addr );
	ifstream input_file( addr, ios::in | ios::binary );

	if ( input_file ) {
	    data = vector< uint8_t > ( istreambuf_iterator< char >( input_file ), istreambuf_iterator< char >() );

	    input_file.close();
	}
	else {
		throw Exception( INVALID_FILE, "Invalid input file address '" + addr + "'!" );
	}
}



uint8_t File::read( size_t index ) {
	info.say( "Read byte from " + index );

	if ( index < size() ) {
		return data[ index ];
	}
	else {
		throw Exception( MEMORY_ERROR, "Out of index." );
	}
}



vector< uint8_t > File::read( size_t index, size_t length ) {
	info.say( "Read " + to_string( length ) + " bytes from " + to_string( index ) );

	if ( index + length <= size() ) {
		vector< uint8_t >::const_iterator start = data.begin() + index;
		vector< uint8_t >::const_iterator stop  = data.begin() + index + length;
		return vector< uint8_t > ( start, stop );
	}
	else {
		throw Exception( MEMORY_ERROR, "Out of index." );
	}
}



void OutputFile::write( size_t index, uint8_t byte ) {
	// info.say( "Write byte on position " + to_string( index ) );

	// POZOR, indexuje se od nuly, proto + 1
	if ( index + 1 > size() ) {
		// info.say( "Resize data" );
		data.resize( index + 1 );
	}
	data[ index ] = byte;
}



void OutputFile::write( size_t index, uint8_t *bytes, size_t length ) {
	// info.say( "Write " + to_string( length ) + " bytes from position " + to_string( index ) );

	if ( index + length > size() ) {
		// info.say( "Resize data" );
		data.resize( index + length );
	}

	for ( uint i = 0 ; i < length ; ++i ) {
		// DEBUG_MSG( "Write " +  to_binstring( bytes[ i ] ) + " on position " + to_string( index + i ) );
		data[ index + i ] = bytes[ i ];
	}	
}



void OutputFile::write( size_t index, uint32_t *bytes, size_t count, bool reverse ) {
	if ( reverse ) {
		switch ( count ) {
			case 4: write( index + 3, ((uint8_t *)((void *)bytes))[ 0 ] );
			case 3: write( index + 2, ((uint8_t *)((void *)bytes))[ 1 ] );
			case 2: write( index + 1, ((uint8_t *)((void *)bytes))[ 2 ] );
			case 1: write( index    , ((uint8_t *)((void *)bytes))[ 3 ] );
		}
	}
	else {
		switch ( count ) {
			case 4: write( index + 3, ((uint8_t *)((void *)bytes))[ 3 ] );
			case 3: write( index + 2, ((uint8_t *)((void *)bytes))[ 2 ] );
			case 2: write( index + 1, ((uint8_t *)((void *)bytes))[ 1 ] );
			case 1: write( index    , ((uint8_t *)((void *)bytes))[ 0 ] );
		}
	}
}



void OutputFile::write( size_t index, uint16_t *bytes, size_t count, bool reverse ) {
	if ( reverse ) {
		switch ( count ) {
			case 2: write( index + 1, ((uint8_t *)((void *)bytes))[ 0 ] );
			case 1: write( index    , ((uint8_t *)((void *)bytes))[ 1 ] );
		}
	}
	else {
		switch ( count ) {
			case 2: write( index + 1, ((uint8_t *)((void *)bytes))[ 1 ] );
			case 1: write( index    , ((uint8_t *)((void *)bytes))[ 0 ] );
		}
	}
}



void OutputFile::remove( Position position, size_t length ) {
	if ( length > size() ) {
		length = size();
	}

	switch ( position ) {
		case BEGIN:
			data.erase( data.begin(), data.begin() + length );
			break;

		case END:
			data.resize( data.size() - length );
			break;
			
		case ALL:
			data.clear();
			break;
	}
}



void OutputFile::remove( size_t index ) {
	if ( index < size() ) {
		data.erase( data.begin() + index );
	}
	else {
		throw Exception( MEMORY_ERROR, "Out of index." );
	}
}



void OutputFile::remove( size_t from, size_t to ) {
	if ( from > size() || to > size() ) {
		throw Exception( MEMORY_ERROR, "Out of index." );
	}
	else {
		data.erase( data.begin() + from, data.begin() + to );
	}
}



void OutputFile::save() {
	info.say( "Open output " + address );
	ofstream output( address, ios::binary | fstream::trunc );
	
	if ( ! output ) {
		throw Exception( INVALID_FILE, "Invalid output file address '" + address + "'!" );
	}
	

	info.say( "Saving data to output file" );

	ostream_iterator< uint8_t > output_iterator( output );
	copy( data.begin(), data.end(), output_iterator);

	output.close();
}

