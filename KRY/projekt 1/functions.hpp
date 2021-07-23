/**
 * @file functions.hpp
 * @version 1.0
 * @author Vaclav Martinka
 * @date 03. 04. 2021
 * @brief Pomocne funkce pro _Projekt 1 (KRY)_, VUT Brno
 */

#pragma once

#include <string>
#include <vector>



constexpr const char *HELP_MSG =
	"Projekt 1 (KRY)\n"
	"Vaclav Martinka (xmarti76)\t03. 04. 2021\tFIT VUT Brno\n"
	"Pouziti:\n"
	"-h, --help         Zobrazi tuto napovedu\n"
	"-d, --decode KLIC  Desifruje text ze stdin pomoci KLIC\n"
	"-e, --encode KLIC  Zasifruje text ze stdin pomoci KLIC\n"
	"-a, --attack       Pokusi se prolomit text na stdin, vypise odhadovanou delku hesla a heslo (vychozi volba)\n"
	"-s, --solve        Pokusi se prolomit text na stdin, vypise desifrovany text ze stdin\n";

typedef enum {
	DECODE,
	ENCODE,
	ATTACK,
	SOLVE,
	HELP,
	UNKNOWN,
} Mode;

/**
 * @brief Vytiskne chybovou hlasku ve tvaru [CHYBA] `msg`
 * @param msg Obsah chybove hlasky
 */
inline void error(const std::string &msg) {
	std::cerr << "[CHYBA] " << msg << std::endl;
}


#ifndef NODEBUG
	/**
	 * @brief Makro pro tisk ladicich informaci ve tvaru [LOG] `msg`: `value`
	 * @param msg Jmeno hodnoty
	 * @param value Hodnota
	 */
	#define log(msg, value) {std::cerr << "[LOG] " << msg << ": " << value << std::endl;}
#else
	#define log(msg, value)
#endif

/**
 * @brief Funkce pro zpracovani prvniho argumentu
 * 
 * @param &arg Reference na prvni argument
 * @return Mode Rezim programu
 */
Mode parseArg(const std::string &arg);

/**
 * @brief Zobrazi napovedu
 */
inline void printHelp() {
	std::cout << HELP_MSG << std::endl;
}


/**
 * @brief Ziska faktory cisla
 * 
 * @param n Vstupni cislo
 * @return std::vector<int> Vektor vsech faktoru
 */
std::vector<int> factors(const int n);

/**
 * @brief Nacte vstup od uzivatele a odstrani nepotrebne znaky
 * @return string& Nacteny vstup se znaky A - Z
 */
std::string readInput();

