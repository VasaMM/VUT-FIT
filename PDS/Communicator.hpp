/**
 * @file Communicator.hpp
 * @author Václav Matinka
 * @date 23. 04. 2019 (19:22)
 * @brief Třída obalující samotnou UDP komunikaci a jejich volání společná pro \c peer i \c node (např. odesílání a příjem \c ack)
 */

#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#include <map>

#include "Txid.hpp"
#include "Destination.hpp"
#include "Options.hpp"
#include "bencode.hpp"


using namespace std;
using namespace bencode;


/// Timeout pro čekání na \c ACK zprávu
const uint ACK_TIMEOUT = 2;

/// Timeout pro mazaní přijatých \c ACK zpráv
const uint ACK_CLEAN_TIME = 5;

/// Velikost bufferu
const uint BUFFER_SIZE = 32768;


/// Struktura pro popis jednoo uživatele (adresa, čas přidání)
typedef struct {
	Destination dest;  ///< Uživatelova adresa
	time_t stamp;      ///< Čas poslední registrace
} User;



/// Třída pro zasílání zpráv
class Sender {
public:
	/**
	 * @brief Konstruktor vyžadující existující socket
	 * @param _socket Existující socket
	 */
	Sender( int *_socket ) {
		socket = _socket;
	}


	/**
	 * @brief Odeslání jedné zprávy
	 * 
	 * @param msg Zpráva jako slovník
	 * @param to Cílová adresa
	 * 
	 * @return \c Txid odeslané zprávy
	 */
	Txid sendMsg( dict msg, Destination const &to );


private:
	/// Aktuální \c Txid
	Txid txid = 0;

	/// Ukazatel na socket
	int *socket;
};



/// Třída pro příjem zpráv
class Receiver {
public:
	/**
	 * @brief Konstruktor příjemce zpráv z existujícího socketu
	 * @param socket Ukazatel na socket
	 */
	Receiver( int *socket );


	/**
	 * @brief Přijme zprávu (neblokující)
	 * 
	 * @param from Ukazatel na adresu odesílatele, je-li \c nullptr, nepoužije se, jinak se nastaví podle hodnot ze síťové karty
	 * @return Vektor přijatých dat (může být i prázdný)
	 */
	vector< char > receive( Destination *from = nullptr );


private:
	/// Aktuální \c Txid
	int *socket;
};



/// Třídá obalující síťovou komunikaci
class Communicator {
protected:
	/// Seznam přijatých \c ACK
	/// Klíčem je \c Txid, hodnotou čas doručení
	std::map< Txid, time_t> ackMap;

	/// Moje adresa
	Destination me;

	/// Adresa serveru
	Destination server;

	/// Příjímač dat
	Receiver *receiver;

	/// Odesílač dat
	Sender   *sender;

	/// Můj socket
	int mySocket;

	/// Ukazatel ukončovací podmínku
	bool *running;


	/**
	 * @brief Konstruktor, vytvoří a inicializuje socket a objekty ro příjem/odesílání dat
	 * 
	 * @param running Ukončovací podmínka
	 * @param me Moje adresa (na této budu přijímat zprávy)
	 * @param server Adresa serveru (sem budu odesílat zprávy)
	 */
	Communicator( bool *running, Destination const me, Destination const server = Destination() );
	

	/// Destruktor, uzavře socket
	~Communicator() {
		close( mySocket );
	}


public:
	/**
	 * @brief Vrací přijatá data, NEBLOKUJÍCÍ - nutno kontrolovat velikost přečtených dat
	 * 
	 * @param from Ukazatel na adresu odesílatele, je-li \c nullptr, nepoužije se, jinak se nastaví podle hodnot ze síťové karty
	 * @return Přečtená data (může být i prázdná)
	 */
	vector<char> read( Destination *from = nullptr ) { return receiver->receive( from ); }


	/**
	 * @brief Odešlě \c ACK na \c Txid na zadanou adresu 
	 * 
	 * @param txid Potvrzované \c Txid
	 * @param to Cílová destinace
	 * 
	 * @return Txid zprávy (stejné jako odesílané)
	 */
	Txid sendAck( Txid const txid, Destination const &to ) const;


	/**
	 * @brief Zapíše přijaté \c ACK do seznamu
	 * 
	 * @param txid \c Txid přijatého \c ACK
	 */
	void receiveAck( Txid const txid );


	/**
	 * @brief Zkontroluje, zda bylo dané \c ACK doručeno
	 * 
	 * @param txid Očekávané \c Txid
	 * @return Bylo doručeno?
	 */
	bool checkAck( Txid const txid ) const;


	/**
	 * @brief Odstraní dané \c ACK ze seznamu
	 * @param txid \c Txid ke smazání
	 */
	void removeAck( Txid const txid ) const;
	

	/**
	 * @brief Počká na doručení \c ACK
	 * 
	 * @param txid Očekávané \c Txid
	 * @return Vrací \c true, pokud bylo \c ACK doručeno v limitu
	 */
	bool waitAck( Txid const txid ) const;


	/**
	 * @brief Odešle \c ERROR zprávu
	 * 
	 * @param txid \c Txid chyby
	 * @param msg Popis chyby
	 * @param to Cílová destinace
	 * @return Odeslané \c Txid (stejné jako parametr \c txid)
	 */
	Txid sendError( Txid const txid, string const &msg, Destination const &to ) const;


	/// Vymaže strará \c ACK z databáze
	void clearAcks();
};

#endif
