/**
 * @file main.cpp
 * @version 1.0
 * @author Vaclav Martinka
 * @date 03. 04. 2021
 * @brief Hlavni funkce pro _Projekt 1 (KRY)_, VUT Brno
 */

#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <limits>

#include "functions.hpp"

using namespace std;


/// Počet písmen v abecedě
#define LETTERS 26

/// Index koincidence
#define INDEX_OF_COINCIDENCE 0.065

/// Frekvenční pravděpodobnost písmen v angličtině
/// https://en.wikipedia.org/wiki/Letter_frequency
const float FREQUENCY[LETTERS] = {
	0.082, 0.015, 0.028, 0.043, 0.13, 0.022, 0.02, 0.061, 0.07, 0.0015,
	0.0077, 0.04, 0.024, 0.067, 0.075, 0.019, 0.00095, 0.06, 0.063, 0.091,
	0.028, 0.0098, 0.024, 0.0015, 0.02, 0.00074
};


/**
 * @brief (De)sifrator Viegenerovy sifry
 * @param key Klic
 * @param way Smer sifrovani. Desifrovani -1, sifrovani +1.
 */
void vigenere(const string &key, const string &input, int way);


/// @brief Desifrovani Viegenerovy sifry
/// @param key Klic pro desifrovani
inline void decode(const string &key, const string &input) {vigenere(key, input, -1);}


/// @brief Sifrovani Viegenerovy sifry
/// @param key Klic pro sifrovani
inline void encode(const string &key, const string &input) {vigenere(key, input, +1);}

/**
 * @brief Kasiskiho test, vraci odhadovanou delku hesla
 * @param input Vstupni retezec
 * @return int Odhadnuta delka hesla
 */
int kasiski(const string &input);

/**
 * @brief Friedmanuv test, vraci odhadovanou delku hesla
 * @param input Vstupni retezec
 * @param password Ukazatel na string, do ktereho bude ulozeno vysledne heslo, neni-li `nullptr`.
 * @return float Odhadnuta delka hesla
 */
float friedman(const string &input);

/**
 * @brief Odhad hesla pomocí indexu koincidence
 * 
 * @param input Vstupni retezec
 * @param maxLength Maximální délka hesla. Je-li nastaveno 0, není omezena
 * @return string Vysledne heslo
 */
string coincidence(const string &input, size_t maxLength = 0);

/**
 * @brief Provede utok na zasifrovany test. Pomoci Kasiskiho a Friedmanova testu odhadne delku a zneni hesla. Vysledek vypise na konzoli
 * @return string Získané heslo
 */
string attack(const string &input) {
	float f = friedman(input);
	cout << f << ";" << flush;

	int k = kasiski(input);
	cout << k << ";" << flush;
	
	string password = coincidence(input, max(static_cast<int>(round(f)), k) * 10);
	cout << password.size() << ";" << password << endl;

	return password;
}



/**
 * @brief Hlavní funkce programu
 * 
 * @param argc Počet předaných argumentů
 * @param argv Argumenty
 * @return int Návratový kód
 */
int main(int argc, char const *argv[]) {
	if (argc > 3) {
		error("Neplatny pocet argumentu. Pouzijte --help pro zobrazeni napovedy.");
		return 1;
	}

	// Vychozi rezim je utok
	Mode mode = ATTACK;

	if (argc > 1) {
		mode = parseArg(argv[1]);
	}

	string input;

	switch (mode) {
		case DECODE:
			log("mode", "DECODE");
			if (argc == 3) {
				decode(argv[2], readInput());
			}
			else {
				error("Nebyl zadan klic. Pouzijte --help pro zobrazeni napovedy.");
			}
			break;

		case ENCODE:
			log("mode", "ENCODE");
			if (argc == 3) {
				encode(argv[2], readInput());
			}
			else {
				error("Nebyl zadan klic. Pouzijte --help pro zobrazeni napovedy.");
			}
			break;

		case ATTACK:
			log("mode", "ATTACK");
			attack(readInput());
			break;

		case SOLVE:
			log("mode", "SOLVE");
			input = readInput();
			decode(attack(input), input);
			break;

		default:
		case HELP:
			printHelp();
			break;

		case UNKNOWN:
			error("Neznamy argument '" + string(argv[1]) + "'. Pouzijte --help pro zobrazeni napovedy.");
	}
}



void vigenere(const string &key, const string &input, int way) {
	size_t p = 0;

	// prevedu klic na cislo
	vector<unsigned char>k;
	for (auto & c : key) {
		if (c >= 'a' && c <= 'z'){
			k.push_back(c - 'a');
		}
		if (c >= 'A' && c <= 'Z') {
			k.push_back(c - 'A');
		}
	}

	// Iteruji pres vstup a (de)sifruji klicem
	for (auto & c : input) {
		char out = c + (way * k[p]);

		if (out > 'Z') {
			out -= ('Z' - 'A') - 1;
		}
		else if (out < 'A') {
			out += ('Z' - 'A') + 1;
		}

		cout << out;
		p = (p + 1) % k.size();
	}
	cout << endl;
}


int kasiski(const string &input) {
	// Hledam jenom trigramy
	vector<size_t> distances(input.length() - 2, 0);
	set<string> trigrams;

	for (size_t i = 0; i < input.length() - 3; ++i) {
		string trigram = input.substr(i, 3);

		// Overim, ze jsem ho uz nehledal
		if (trigrams.find(trigram) != trigrams.end()) {
			continue;
		}
		trigrams.insert(trigram);

		size_t pos = input.find(trigram, i + 1);

		while (pos != string::npos) {
			++distances[pos - i];

			pos = input.find(trigram, pos + 1);
		}
	}

	vector<size_t> lengths(input.length() - 2, 0);

	for (size_t i = 0; i < distances.size(); ++i) {
		if (distances[i]) {
			for (auto & f : factors(i)) {
				++lengths[f];
			}
		}
	}

	size_t maxOccurrences = 0;
	size_t passwordLength;
	for (int i = lengths.size() - 1; i >= 3; --i) {  // preskocim delky 0, 1 a 2
		if (lengths[i]) {
			if (lengths[i] > maxOccurrences) {
				maxOccurrences = lengths[i];
				passwordLength = i;
			}
		}
	}

	log("kasiski", passwordLength);

	return passwordLength;
}

float friedman(const string &input) {
	// Delka retezce
	const size_t SIZE = input.size();

	// Provedu Friedmanuv test
	vector<size_t> friedmanOccurrences(LETTERS, 0);

	for (auto & c : input) {
		++friedmanOccurrences[c - 'A'];
	}

	unsigned long long friedmanSum = 0;
	for (auto & f : friedmanOccurrences) {
		friedmanSum += f * (f - 1);
	}

	float friedman = (1.0 * friedmanSum) / (SIZE * (SIZE - 1));
	friedman = (INDEX_OF_COINCIDENCE - (1.0 / LETTERS)) / (friedman - (1.0 / LETTERS));
	log("friedman", friedman);

	return friedman;
}


string coincidence(const string &input, size_t maxLength) {
	// Delka retezce
	const size_t SIZE = input.size();

	maxLength = maxLength != 0 ? maxLength : SIZE;
	log("maximalni delka hesla", maxLength);

	// Provedu postup z prednasky
	float deviation = numeric_limits<float>::infinity();
	size_t passwordLength;

	vector<vector<size_t>> occurrences;
	vector<unsigned long long> sum;

	// Zkousim ruzne delky hesla od 1
	for (size_t length = 1; length < maxLength; ++length) {
		// Rozsirim vektory o dalsi polozku
		occurrences.push_back(vector<size_t>(LETTERS, 0));
		sum.push_back(0);

		// Zaznamenam si pocet vyskytu jednotlivych pismen na ruznych offsetech
		for (size_t i = 0; i < SIZE; ++i) {
			++occurrences[i % length][input[i] - 'A'];
		}

		// Vypocitam index koincidence
		float tmpDeviation = 0;
		for (size_t i = 0; i < length; ++i) {
			for (size_t l = 0; l < LETTERS; ++l) {
				sum[i] += occurrences[i][l] * (occurrences[i][l] - 1);
			}

			// NOTE SIZE / length je mirne nepresne
			float coincidence = (1.0 * sum[i]) / ((SIZE / length) * ((SIZE / length) - 1));
			tmpDeviation += fabs(INDEX_OF_COINCIDENCE - coincidence);
		}

		// Zapamatuji si nejmensi prumernou odchylku
		float avgDeviation = tmpDeviation / length;
		if (avgDeviation < deviation) {
			deviation = avgDeviation;
			passwordLength = length;
		}

		// Vynuluji mezivysledky
		for (auto & o : occurrences) {
			for (auto & x : o) {
				x = 0;
			}
		}

		for (auto & s : sum) {
			s = 0;
		}
	}
	log("Delka hesla", passwordLength);




	//////////////////////////////////////////////////////////////////////////
	// Se znalosti delky hesla zkusim hadat heslo
	// https://en.wikipedia.org/wiki/Index_of_coincidence
	string password(passwordLength, 'a');

	occurrences.resize(1);

	// Heslo hadam znak po zanku
	for (size_t offset = 0; offset < passwordLength; ++offset) {
		// Vymazu docasna data
		for (auto & x : occurrences[0]) {
			x = 0;
		}

		// Ziskam pocet vyskytu jednotlivych pismen
		for (size_t i = offset; i < SIZE; i += passwordLength) {
			++occurrences[0][input[i] - 'A'];
		}

		// Zkousim postupne vsechny pismena
		float maxDeviation = numeric_limits<float>::infinity();

		for (char key = 0; key < LETTERS; ++key) {
			float sum = 0.0;

			// Sectu vyskyty
			for (size_t l = 0; l < LETTERS; ++l) {
				// sum[0] += occurrences[0][l] * (occurrences[0][l] - 1);
				sum += FREQUENCY[l] * (1.0 * occurrences[0][(l + key) % LETTERS] / (SIZE / passwordLength));
			}

			// NOTE SIZE / passwordLength je mirne nepresne
			float deviation = fabs(INDEX_OF_COINCIDENCE - sum);

			// Zapamatuji si nejvetsi pravdepodobnost
			if (deviation < maxDeviation) {
				maxDeviation = deviation;
				password[offset] = key + 'a';
			}
		}
	}

	log("password", password);

	// Zkusim odhalit falešně dlouhá hesla
	// abcabcabcabc => abc
	vector<int>sizes = factors(passwordLength);
	for (auto & size : sizes) {
		string tmp = password.substr(0, size);
		
		size_t pos = size;
		for (size_t i = 0; i < passwordLength / size;) {
			if (tmp == password.substr(pos, size)) {
				pos += size;

				if (pos >= passwordLength) {
					log("nove heslo", tmp);
					return tmp;
				}
			}
			else {
				break;
			}
		}
	}

	return password;
}