/**
 * @file Node.hpp
 * @author Václav Matinka
 * @date 21. 04. 2019 (13:17)
 * @brief Třída implementující funkce potřebné pro běh \c nodu
 */

#ifndef NODE_HPP
#define NODE_HPP

#include <map>
#include <string>

#include "Destination.hpp"
#include "Options.hpp"
#include "Communicator.hpp"
#include "Txid.hpp"
#include "parser.hpp"

using namespace std;


/// Timeout po kterém bude užvatel smazán z databáze
const uint USER_TTL = 30;

/// Timeout po kterém bude \c Node smazán z databáze
const uint NODE_TTL = 12;


/// Node
class Node : public Communicator {
private:
	/// Mapa sousedů a jejich uživatelů
	map< Destination, map< string, User > > usersDatabase;

	/// Převede mapu sousedů a jejich uzlů na databázi vhodnou k odeslání
	dict prepareDb() const;

public:
	/**
	 * @brief Konstruktor, inicializuje \c Communicator
	 * 
	 * @param options Nastavení
	 * @param running Ukončovací proměnná
	 */
	Node( Options const &options, bool *running ) : Communicator( running, options.reg() ) {}


	/**
	 * @brief Přidá nového uživatele do databáze
	 * @param user Přijatá data zprávy
	 */
	void addUser( MsgData const &user );


	/**
	 * @brief Smaže uživatele z databáze
	 * @param username Username
	 */
	void eraseUser( string const &username );


	/// Vyčistí databázi
	void clean();


	/**
	 * @brief Přijme a zpracuje \c UPDATE zprávu
	 * 
	 * @param from Od koho byla přijata
	 * @param db Přijatá databáze
	 */
	void receiveUpdate( Destination const &from, dict const &db );


	/**
	 * @brief Přijme zprávu pro smazání souseda
	 * @param from Adresa souseda
	 */
	void receiveDisconnect( Destination const &from );


	/**
	 * @brief Odešle odpověď na \c GETLIST
	 * 
	 * @param txid \c Txid žádosti o list
	 * @param answerDest Adresa pro odpověď
	 * 
	 * @return \c Txid odpovědi (stejné jako Txid žádosti)
	 */
	Txid sendList( Txid const txid, Destination const &answerDest ) const;


	/// Zašle svoji databázi na všem svým sousedům
	void sendUpdate();


	/**
	 * @brief Zašle svoji databázi danému uzlu
	 * 
	 * @param to Cílová destinace
	 * @param db Reference na odesílanou databázi
	 * @return \c Txid zprávy
	 */
	Txid sendUpdate( Destination const &to, dict const &db ) const;


	/**
	 * @brief Zašle svoji databázi danému uzlu
	 * 
	 * @param to Cílová destinace
	 * @return \c Txid zprávy
	 */
	Txid sendUpdate( Destination const &to ) const;


	/**
	 * @brief Pokusí se navázat sousedství s novým uzlem
	 * @param dest Nový uzel
	 */
	void connect( Destination const &dest );


	/// Pošle \c DISCONNECT zprávu všem svým sousedům
	void disconnect();	


	/// Zobrazí aktuální databázi uživatelů
	void printUsers();


	/// Zobrazí aktuální databázi sousedů
	void printNeighbors();


	/**
	 * @brief Otestuje, zda souhlasí zadaná destinace s uzivatelem
	 * 
	 * @param dest Destinace ke kontrole
	 * @param username Uzivatel ke kontrole
	 * 
	 * @return Zadane udaje souhlasi
	 */
	bool checkUserDest( Destination &dest, string &username );


	/**
	 * @brief Zjisti, zda je dana destinace registrovana v databazi
	 * @param dest Destinace ke kontrole
	 * @return Existuje
	 */
	bool checkDest( Destination &dest );


	/**
	 * @brief Získá ukazatel na uživatelskou adresu. \c Nullptr pokud uživatel neexistuje
	 * 
	 * @param username Username uživatele
	 * @return Jeho adresa (ukazatel)
	 */
	Destination *getUserDestination( string &username ) {
		try {
			return &( usersDatabase[ me ].at( username ).dest );
		}
		catch ( out_of_range &e ) {
			return nullptr;
		}
	}
};

#endif
