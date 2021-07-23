/**
 * @file parser.hpp
 * @author Václav Matinka
 * @date 21. 04. 2019 (13:06)
 * @brief Parsování přijatých zpráv
 */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <boost/variant.hpp>
#include <string>

#include "Txid.hpp"
#include "bencode.hpp"


using namespace bencode;

/// Vytvořím si proměnnou pro uložení chybových hlášení
#define PREPARE() string val

/**
 * @brief Pokusím se získat hodnotu z opovědi
 * 
 * @param _type Datový typ hodnoty (integer, string, dict, list)
 * @param _typeStr Idetifikátor hodnoty (string)
 * 
 * @return Hodnota pokud existuje, jinak výjimka
 */
#define GET( _type, _typeStr )  get< _type >( msg.at( _typeStr ) ); val = _typeStr


using namespace std;


/// Seznam možných typů zpráv jako string
const string MsgTypeString[] = {
	"hello",       ///< hello
	"bye",         ///< bye
	"getlist",     ///< getlist
	"list",        ///< list
	"message",     ///< message
	"update",      ///< update
	"disconnect",  ///< disconnect
	"ack",         ///< ack
	"error",       ///< error
};


/// Seznam možných typů zpráv
enum MsgType {
	HELLO,       ///< hello
	BYE,         ///< bye
	GETLIST,     ///< getlist
	LIST,        ///< list
	MESSAGE,     ///< message
	UPDATE,      ///< update
	DISCONNECT,  ///< disconnect
	ACK,         ///< ack
	ERROR,       ///< error
	__OTHER__    ///< Nedefinováno
};


/// Struktura pro uchování načených hodnot
typedef struct {
	MsgType type;  ///< Typ zprávy
	Txid    txid;  ///< \c Txid zprávy

	string username;  ///< Jméno
	string msg;       ///< Zpráva
	string ipv4;      ///< IP adresa
	uint   port;      ///< port
	dict   data;      ///< Slovník (např. peerů)
} MsgData;


/**
 * @brief Parser zprávy
 * @param msg Přijatá zpráva
 * @return Struktura s načtenými daty
 */
MsgData parse( dict const &msg );



#endif