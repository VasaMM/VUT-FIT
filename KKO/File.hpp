/**
 * @file File.hpp
 * @author Václav Matinka
 * @date 14. 04. 2019 (14:34)
 * @brief Třídy pro práci se soubory (jedna pro čtení, druhá pro zápis)
 */

#ifndef FILE_HPP
#define FILE_HPP


#include <iostream>
#include <vector>


using namespace std;
namespace Coder {


/// Pozice v souboru
enum Position {
	BEGIN,  //< Od začátku
	END,    //< Od konce
	ALL,    //< Vše
};



/// Objekt pro práci se souborem
class File {
protected:
	/// Konstruktor obecného objektu File. Konstructor je protected, to znamená, že tento objekt lze pouze dědit
	File() {}

	/// Destruktor
	~File() {}


	/// Vnitřní data na data
	vector< uint8_t > data;


public:
	/**
	 * @brief Získá aktuální velikost dat
	 * @return Aktuální velikost dat
	 * @warning Vrací velikost dat v paměti RAM, to nemusí odpovídat velikosti souboru na disku. Pro sjednocení je nutné volat metodu save()
	 */
	size_t const size() { return data.size(); }


	/**
	 * @brief Načte jeden byte ze souboru. Je-li index mimo rozsah, vrací výjimku
	 * 
	 * @param index Index čteného bytu
	 * @return Načtený byte
	 */
	uint8_t read( size_t index );


	/**
	 * @brief Načte vector bytů. Je-li index mimo rozsah, vrací výjimku
	 * 
	 * @param index Začátek čtení
	 * @param length Délka čteného rozsahu
	 * 
	 * @return Vector načtených bytů
	 */
	vector< uint8_t > read( size_t index, size_t length );


	/**
	 * @brief Vrací celý obsah souboru jako ukazatel na vektor
	 * @return Obsah souboru
	 */
	vector< uint8_t > *get_data() { return &data; }

};



/// Objekt pro vstupní soubor
class InputFile : public File {
public:
	/**
	 * @brief Konstruktor
	 * @param address Adresa k otevíranému souboru
	 */
	InputFile( string address );

	/// Destruktor
	~InputFile() {}
};



/// Objekt pro výstupní soubor
class OutputFile : public File {
public:
	/**
	 * @brief Konstruktor
	 * @param file_address Adresa k výstupnímu souboru
	 */
	OutputFile( string file_address ) { address = file_address; }


	/// Destruktor, postará se o korektní uložení dat a uzavření souboru
	~OutputFile() { save(); }

	
private:
	/// Adresa výstupního soubor
	string address;
	

public:
	/**
	 * @brief Zapíše jeden byte na zadaný index
	 * 
	 * @param index Index k zapsání. Je-li nižší než aktuální velikost souboru, alokuje se další paměť
	 * @param byte Byte k zapsání
	 */
	void write( size_t index, uint8_t byte );


	/**
	 * @brief Zapíše vektor bytů na zadaný index
	 * 
	 * @param index Index k zapsání. Je-li nižší než aktuální velikost souboru, alokuje se další paměť
	 * @param bytes Vector bytů
	 */
	void write( size_t index, vector< uint8_t > bytes ) { write( index, bytes.data(), bytes.size() ); }


	/**
	 * @brief Zapíše pole bytů na zadaný index
	 * 
	 * @param index Index k zapsání. Je-li nižší než aktuální velikost souboru, alokuje se další paměť
	 * @param bytes Ukazatel (reference) na pole bytů
	 * @param length Velikost pole
	 */
	void write( size_t index, uint8_t *bytes, size_t length );


	/**
	 * @brief Zapíše určitý počet bytů na zadaný index
	 * 
	 * @param index Index k zapsání. Je-li nižší než aktuální velikost souboru, alokuje se další paměť
	 * @param bytes Ukazatel na 32 int reprezentující až 4 bytů
	 * @param count Počet bytů k zapsaní (zapisuje se zleva), výchozí \em 4
	 * @param reverse Bity se zapíší v opačném pořadí, výchozí \em false
	 */
	void write( size_t index, uint32_t *bytes, size_t count = 4, bool reverse = false );


	/**
	 * @brief Zapíše určitý počet bytů na zadaný index
	 * 
	 * @param index Index k zapsání. Je-li nižší než aktuální velikost souboru, alokuje se další paměť
	 * @param bytes Ukazatel na 16 int reprezentující až 2 byty
	 * @param count Počet bytů k zapsaní (zapisuje se zleva), výchozí \em 2
	 * @param reverse Bity se zapíší v opačném pořadí, výchozí \em false
	 */
	void write( size_t index, uint16_t *bytes, size_t count = 2, bool reverse = false );


	/**
	 * @brief Vymaže data od začátku/konce souboru, popř. smaže celý soubor. Tato varianta je odolná vůči rozsahům mimo velikost dat
	 * 
	 * @param position Pozice, odkud se bude mazat
	 * @param length Délka mazaných dat, v případě možnosti \c ALL ignorováno
	 */
	void remove( Position position, size_t length = 0 );

	
	/**
	 * @brief Smaže jeden byte na daném indexu
	 * @param index Index mazaného bytu
	 */
	void remove( size_t index );

	
	/**
	 * @brief Smaže daný rozsah.
	 * @param from Začátek mazaných dat
	 * @param to Konec mazaných dat
	 */
	void remove( size_t from, size_t to );

	
	/// Uloží soubor, tzn. přepíše všechna data v souboru daty z RAM.
	void save();
};


}

#endif



