/* * * * * * * * * * * * * * * * * * *
 * AUTOR: Vaclav Martinka (xmarti76)
 * Struktura pro uchovani informaci
 * 17. 11. 2017 FIT VUT
 * * * * * * * * * * * * * * * * * * */

#include <string>

#ifndef OPTIONS_HPP
#define OPTIONS_HPP 


// Trida pro ulozeni nastaveni (argumentu) programu
class Options {
	public: enum cip_type { NONE, POP3S, STLS };

private:
	// Povinne parametry
	std::string server_;
	std::string authentication_;
	std::string out_dir_;

	// Volitelne parametry
	bool delete_it_     = false;
	bool only_new_      = false;
	cip_type ciphering_ = NONE;
	std::string certfile_;
	std::string certaddr_;
	unsigned int port_;

	// Uzivatelske udaje
	std::string name_;
	std::string password_;


public:
	void         server ( std::string data ) { server_ = data; }
	std::string  server ()                   { return server_; }
	
	void         authentication ( std::string data ) { authentication_ = data; }
	std::string  authentication ()                   { return authentication_; }

	void         out_dir ( std::string data );	// Provadi rovnou kontrolu na existenci
	std::string  out_dir ()                   { return out_dir_; }

	void         port ( unsigned int data ) { port_ = data; }
	unsigned int port ()                    { return port_; }

	void         delete_it ( bool data ) { delete_it_ = data; }
	bool         delete_it ()            { return delete_it_; }

	void         only_new ( bool data ) { only_new_ = data; }
	bool         only_new ()            { return only_new_; }

	void         ciphering ( cip_type data ) { ciphering_ = data; }
	cip_type     ciphering ()                { return ciphering_; }

	void         certfile ( std::string data ) { certfile_ = data; }
	std::string  certfile ()                   { return certfile_; }

	void         certaddr ( std::string data ) { certaddr_ = data; }
	std::string  certaddr ()                   { return certaddr_; }

	void         name ( std::string data ) { name_ = data; }
	std::string  name ()                   { return name_; }

	void         password ( std::string data ) { password_ = data; }
	std::string  password ()                   { return password_; }

	// Funkce parsuje soubor s autentizacnimi udaji
	// Je pomerne benevolenti vuci mezeram a neznamym parametrum
	// Lze vyuzit napr. pro specifikaci vystupniho portu nebo adresy serveru
	bool parse_user_file ();

	// Construktor and destruktor
	 Options () {}
	~Options () {}
};

#endif