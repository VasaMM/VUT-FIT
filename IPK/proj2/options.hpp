/* * * * * * * * * * * * * * * * * * *
 * AUTOR: Vaclav Martinka (xmarti76)
 * Struktura pro uchovani atributu
 * 21. 04. 2017 FIT VUT
 * * * * * * * * * * * * * * * * * * */

// #include <netdb.h>

#ifndef OPTIONS_HPP
#define OPTIONS_HPP 


// Trida s nastavenim serveru
class Options {
	public: enum new_bool { INITIAL, YES, NO };

private:
	unsigned int first_ttl_ = 1;
	unsigned int max_ttl_   = 30;
	unsigned int timeout_   = 2;	// int v s
	unsigned int port_      = 0;
	new_bool ptr_           = INITIAL;
	std::string address_;

public:
	void          first_ttl ( unsigned int data ) { first_ttl_ = data; }
	unsigned int  first_ttl ()                    { return first_ttl_; }
	
	void          max_ttl ( unsigned int data ) { max_ttl_ = data; }
	unsigned int  max_ttl ()                    { return max_ttl_; }

	void          timeout ( unsigned int data ) { timeout_ = data; }
	unsigned int  timeout ()                    { return timeout_; }

	void          port ( unsigned int data ) { port_ = data; }
	unsigned int  port ()                    { return port_; }

	void          ptr ( new_bool data ) { ptr_ = data; }
	unsigned int  ptr ()                { return ptr_; }

	void        address ( std::string data ) { address_ = data; }
	std::string address ()                   { return address_; }

	// Construktor and destruktor
	 Options () {}
	~Options () {}
};

#endif