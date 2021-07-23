/* * * * * * * * * * * * * * * * * * *
 * AUTOR: Vaclav Martinka (xmarti76)
 * POP3 klient - síťové funkce
 * 17. 11. 2017 FIT VUT
 * * * * * * * * * * * * * * * * * * */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <openssl/ssl.h>

#include "options.hpp"


#define BUFFER_SIZE 512

using namespace std;

class Client {
private:
	// Struktura pro uchovani globalniho nastaveni
	Options *options;

	// Promenne pro socket
	int pop3_socket;

	// Promenne pro openSSL
	SSL *ssl;
	SSL_CTX *ctx;
	bool use_SSL;

	// Buffer
	char buffer[ BUFFER_SIZE + 1 ];

	// Jednoduchy test na verzi IP
	inline bool is_IPv4 ( addrinfo *res ) { return ( res->ai_family != AF_INET6 ); }

	// Nacte certifikaty z Options
	// Pokud nebyly zadany, pouziji se defaultni
	void loadCertificates();

	// Inicializuje SSL pripojeni
	void init_SSL();

	// Zasle zprau serveru, neceka na odpoved
	void send_message ( string message );
	
	// Zasle zprau serveru, CEKA na odpoved
	// message_size specifikuje ocekavnou velikost prichozi zpravy
	//     < 0  = ocekavam kratkou zpravu o maximalni velikosti bufferu
	//       0  = vim, ze zprava bude vetsi nez buffer, ale nevim, jak moc
	//     > 1  = znam presnou velikost zpravy
	void send_message ( string message, string &answer, int message_size = -1 );
	
	// Prijmu zpravu o vlikosti message_size (viz send_message)
	void receive_message ( string &answer, int message_size = -1 );

	// Otestuje odpoved serveru
	// => Odpovedel '+OK'
	bool is_it_OK ( string &message );

public:
	// Pripoji se na server
	void connect_server ();

	// Odpoji se od serveru
	bool disconnect_server ();

	// Prihlasi uzivatele
	// Ocekava login a password v options
	// Je nutne zavolat funkci parse_user_file!
	bool log_in ();

	// Pozada o seznam zprav na serveru
	bool list ( string &answer );

	// Ziska pocet zprav na serveru
	int statistics ( string &answer );
	
	// Pozada o obsah zpravy s indexem index
	// Je-li pred tim volana funkce list - lze vyzit znalost a velikosti zpravy
	bool read_message ( string &answer, int index, int message_size = -1 );

	// Smaze zpravi s indexem index
	bool delete_message ( string &answer, int index );

	// Konstruktor
	Client( Options *options_ ) {
		options = options_;
		use_SSL = false;
	}
};

#endif