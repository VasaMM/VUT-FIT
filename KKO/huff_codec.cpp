/**
 * @file huff_codec.cpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (15:20)
 * @brief Proram pro (de)kódování souborů omocí statického i adaptivního Huffmanova kódování
 */


#include "Info.hpp"
#include "File.hpp"
#include "Huffman.hpp"
#include "Header.hpp"
#include "Options.hpp"
#include "Preprocessor.hpp"

#include <ctime>
#include <iomanip>
#include <chrono>


using namespace std;
using namespace chrono;
using namespace Coder;


/**
 * @brief Kompresní funkce
 * @param o Nastavení
 */
void compression( Options o );



/**
 * @brief Dekompresní funkce
 * @param o Nastavení
 */
void decompression( Options o );



/**
 * @brief Main funkce
 * 
 * @param argc Počet předanýcg arguentů
 * @param argv Hodnoty předaných argumentů
 * 
 * @return Návratová hodnota (0 = OK)
 */
int main ( int argc, char *argv[] ) {
	Options options( argc, argv );

	// Spustim stopky
	auto start = steady_clock::now();
	auto t = time( nullptr );
	stringstream t_string;
	             t_string << put_time( localtime( &t ), "%H:%M:%S" );
	info.out( "Start at " + t_string.str() );

	// Detekuji rezim
	switch ( options.mode() ) {
		case Options::COMPRESSION:     compression( options ); break;
		case Options::DECOMPRESSION: decompression( options ); break;

		default:
			exit_error( "Unspecified working mode!" );
	}


	// Ukoncim stopky
	auto end = steady_clock::now();
	t = time( nullptr );
	t_string.seekp( ios_base::beg );
	t_string << put_time( localtime( &t ), "%H:%M:%S" );
	info.out( "End at " + t_string.str() + " (" + to_string( duration< double, milli >( end - start ).count() ) + " ms)" );

	return 0;
}



void compression( Options o ) {
	info.out( "Compression" );

	// Nactu soubory
	InputFile input( o.input() );
	OutputFile output( o.output() );

	info.out( "Input file size: " + to_string( input.size() ) );

	Header header( 0x00 );
	// Zkontroluji model predzpracovani pixelu
	if ( o.model() ) {
		header.set_prop( PIXEL_PREPROCESSOR, true );

		Preprocessor::pixelDifferencion( input );
	}

	output.write( 0, header.get() );
	

	// Spustim staticke kodovani
	if ( o.huffman_mode() == Options::STATIC || o.huffman_mode() == Options::AUTO ) {
		info.out( "Hufmfman static mode" );
		StaticHuffman huff;
		huff.code( input, output );
	}
	// Spustim adaptivni kodovani
	else if ( o.huffman_mode() ==  Options::ADAPTIVE ) {
		info.out( "Hufmfman adaptive mode" );
		AdaptiveHuffman huff;
		huff.code( input, output );
	}

	info.out( "Output file size: " + to_string( output.size() ) );
	info.out( "Compression ratio: " + to_string( 100 - ( output.size() * 1.0 / input.size() ) * 100 ) + " %" );
}






void decompression( Options o ) {
	info.out( "Decompression" );

	// Nactu soubory
	InputFile input( o.input() );
	OutputFile output( o.output() );

	info.out( "Input file size: " + to_string( input.size() ) );

	// Nactu hlavicku
	Header header( input.read( 0 ) );

	if ( header.get_prop( EXTENDED_HEADER ) ) {
		exit_error( "The input file contains an extended header. Please use a newer version of the program." );
	}

	// Detekuji rezim z hlavicky
	Options::HuffmanMode h_mode = o.huffman_mode();
	if ( h_mode == Options::AUTO ) {
		h_mode = header.get_prop( HUFFMAN_ADAPTIVE ) ? Options::ADAPTIVE : Options::STATIC;
	}

	// Spustim staticke kodovani
	if ( h_mode == Options::STATIC ) {
		info.out( "Hufmfman static mode" );
		StaticHuffman huff;
		huff.decode( input, output );
	}
	// Spustim adaptivni kodovani
	else if ( h_mode == Options::ADAPTIVE ) {
		info.out( "Hufmfman adaptive mode" );
		AdaptiveHuffman huff;
		huff.decode( input, output );
	}


	// Zkontroluji model predzpracovani pixelu
	// Musim spoustet v opacnem poradi
	if ( o.model() || header.get_prop( PIXEL_PREPROCESSOR ) ) {
		Preprocessor::pixelDifferencion( output );
	}

	info.out( "Output file size: " + to_string( output.size() ) );
	info.out( "Decompression ratio: " + to_string( 100 - ( input.size() * 1.0 / output.size() ) * 100 ) + " %" );
}
