/**
 * @file Peer.hpp
 * @author Václav Matinka
 * @date 21. 04. 2019 (13:25)
 * @brief Třída implementující funkce potřebné pro běh \c peera
 */

#ifndef PEER_HPP
#define PEER_HPP

#include <map>

#include "Communicator.hpp"
#include "Options.hpp"
#include "Txid.hpp"
#include "parser.hpp"


/// Maximální čas (v sekundách) čekání na odpověď list
const uint LIST_TIMEOUT = 2;

/// Maximální doba, po k
const uint USER_CLEAN_TIME = 5;


/// Třída peer
class Peer : public Communicator {
private:
	/// Můj username
	string username;

	/// Seznam uživatelů
	/// Klíčem je username
	/// Hodnotou struktura User
	std::map< string, User > users;

	/// Seznam přijatých listů
	map< string, User > tmpUsersList;

	/// Mam ulozit prjaty list
	bool saveUsersList = false;

	// Flag informujici o prijeti listu
	bool receivedUserList = false;


public:
	/**
	 * @brief Konstruktor, volá konstruktor pro \c Communicator a nastaví username.
	 * 
	 * @param options Reference na nastavení
	 * @param running Ukazatel na ukončovací bool
	 */
	Peer( Options const &options, bool *running ) : Communicator( running, options.chat(), options.reg() ) {
		username = options.username();
	}


	/**
	 * @brief Nastaví novou adresu serveru. Zároveň zasílá i novou hello zprávu.
	 * 
	 * @param s Nová adresa serveru
	 */	
	void updateServer( Destination const &s ) {
		server = s;
		sendHello();
	}


	/**
	 * @brief Odešlš \c GETLIST na server
	 * @return \c Txid zprávy
	 */
	Txid sendGetlist() const;


	/**
	 * @brief Zpracuje přijatý list a vloží uživatele do mapy
	 * @param data Data ke zpracování
	 */
	void receiveList( dict const &data );


	/**
	 * @brief Počká na doručení listu.
	 * @return Vrací false pokud vypršel timeout, jinak true.
	 */
	bool waitList();


	/**
	 * @brief Odešle \c GETLIST a vypíše formátovanou odpověď
	 * @return Úspěšnost
	 */
	bool printList();



	/// Provede uklid v databazich
	void clean();


	/**
	 * @brief Odešle zprávu jinému \c peerovi
	 * 
	 * @param from Jméno odesílatele
	 * @param to Jméno příjemce
	 * @param message Zpráva
	 * @return \c Txid odeslané zprávy
	 */
	Txid sendMsg( string const &from, string const &to, string const &message );

	
	/**
	 * @brief Odešlě \c hello zprávu na server
	 * @return \c Txid zprávy
	 */
	Txid sendHello() const;


	/**
	 * @brief Odešlě \c hello zprávu s nulovou IP adresou a nulovým portem na server
	 * @return \c Txid zprávy
	 */	
	Txid sendBye() const;
};

#endif
