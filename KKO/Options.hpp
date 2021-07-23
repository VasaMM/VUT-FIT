/**
 * @file Options.hpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (15:25)
 * @brief Objekt pro uchování nastavení a dalších podpůrných funkcí
 */

#ifndef OPTIONS_HPP
#define OPTIONS_HPP

const string VERSION = "1.0";


// #include <iostream>
#include <string>


/// Nápověda k programu
const std::string HELP = {
	"\n"
	"Huffman coder\n"
	"Author: Vaclav Martinka (xmarti76)\t\t14. 04. 2019\t\tFIT VUT\n"
	"Version: " + VERSION + "\n\n"
	
	"Available options:\n"
	"  -h, --help            Display this help and exit.\n"
	"  --version             Display version and exit.\n\n"
	
	"  -c, --compression     The input file will be compressed. (implicit)\n"
	"  -d, --decompression   the input file will be decompressed. You can't use it with --compression.\n"
	"\n"
	"  -h static,            Use Huffman static mode. (implicit).\n"
	"  --huffman static      \n"
	"  -h adaptive,          Use Huffman adaptive mode. You can't use it with static mode.\n"
	"  --huffman adaptive    (Autodetection in decompression mode.)\n"
	"  -m, --model           Activate a model for preprocessing input data. (Autodetection in decompression mode.)\n"
	"  -w, --width           Image width. ! NOT IMPLEMENTED !\n"
	"\n"
	"  -i, --input <file>    Input file.\n"
	"  -o, --output <file>   Output file. Implicit is out.raw.\n"
	"\n"
	"  -v, --verbous         Verbous, progress reports will be displayed.\n"
	"  -q, --quiet           Quiet, nothing display (only errors on stderr).\n\n\n"
};


using namespace std;
namespace Coder {

/// Objekt pro uchování nastavení a dalších podpůrných funkcí
class Options {
public:
	/**
	 * @brief Konstruktor objektu očekává proměnné \c argc a \c argv z volání funkce main
	 * 
	 * @param argc Počet argumentů předaných programu
	 * @param argv Seznam argumentů předaných programu
	 */
	 Options ( int argc, char* argv[] );
	 /// Desktruktor
	// ~Options ();

	/// Seznam možných režimů programu
	enum Mode {
		COMPRESSION,
		DECOMPRESSION
	};
	
	/// Seznam možných režimů programu
	enum HuffmanMode {
		STATIC,
		ADAPTIVE,
		AUTO
	};	
	

private:
	// Parametry
	Mode        _mode    = COMPRESSION;
	HuffmanMode _huffman = AUTO;
	bool        _model   = false;
	string      _input   = "";
	string      _output  = "out.raw";

	
public:
	/**
	 * @brief  Nastaví režim programu
	 * @param m Režim
	 */
	void mode( Mode m ) { _mode = m; }

	/**
	 * @brief  Nastaví typ huffmanova kódování
	 * @param h Typ kódování
	 */
	void huffman_mode( HuffmanMode h ) { _huffman = h; }
	
	/**
	 * @brief  Povolí (nebo zakáže) model předzpracování pixelu
	 * @param m Model
	 */
	void model( bool m ) { _model = m; }

	/**
	 * @brief  Nastaví adresu ke vstupnímu souboru
	 * @param i Vstupní soubor
	 */
	void input( string i ) { _input = i; }

	/**
	 * @brief  Nastaví adresu k výstupnímu souboru
	 * @param o Výstupní soubor
	 */
	void output( string o ) { _output = o; }



	/**
	 * @brief Vrácí režim programu
	 * @return Režim programu
	 */
	Mode mode() const { return _mode; }
	
	/**
	 * @brief Vrací typ huffmanova kódování
	 * @return Huffmanovo kódování
	 */
	HuffmanMode huffman_mode() const { return _huffman; }
	
	/**
	 * @brief Je-li true, bude použit model pro předzpracování pixelů
	 * @return True v případě předzpracování pixelů
	 */
	bool model() const { return _model; }

	/**
	 * @brief Vrací cestu ke vstupnímu souboru
	 * @return Cesta ke vstupnímu souboru
	 */
	string input() const { return _input; }

	/**
	 * @brief Vrací cestu ke výstupnímu souboru
	 * @return Cesta ke výstupnímu souboru
	 */
	string output() const { return _output; }

};

}

#endif