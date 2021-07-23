/**
 * @file main.cpp
 * @author Vaclav Martinka
 * @date 07. 12. 2020 (19:15)
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "Coder.hpp"

using namespace std;

// Vychozi koder ze zadani
#define DEF_SIZE 5
#define DEF_UPPER 0b110101
#define DEF_LOWER 0b101110

// Rezimy koderu
using CoderMode = enum CoderMode_e {
    UNDEFINED,
    DECODE,
    ENCODE
};

// Struktura pro zaznamenani jednoho kroku dekodovani
using decodeState = struct decodeState_s {
    Bits bits;    // Aktualni stav
    size_t prev;  // Ukazatel (index) na predchozi stav
    Price price;  // Cena aktualniho stavu
    Bit bit;      // Bit (1/0) ktery vedl na aktualni stav
};

/**
 * @brief Kontrola na platnost znaku
 * @param c Vstupni znak
 * @return `true` pokud je znak v rozsahu [0-9a-zA-Z]
 */
inline const bool isASCII(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

/**
 * @brief Funkce pro dekodovani zpravy
 * 
 * @param coder Inicializovany koder pro dekodovani
 * @param input Vstupni string, muze obsahovat pouze znaky '0' a '1', vse ostatni je ignorovano
 * @return Dekodovana zprava
 */
const string decode(Coder &coder, const string &input);

/**
 * @brief Funkce pro kodovani zpravy
 * 
 * @param coder Inicializovany koder pro dekodovani
 * @param input Vstupni string, muze obsahovat pouze znaky v rozsahu [0-9a-zA-Z], vse ostatni je ignorovano
 * @return Dekodovana zprava
 */
const string encode(Coder &coder, const string &input);

const string HELP =
    "Projekt do BMS 2020\n"
    "Autor: Vaclav Martinka (xmarti76)\n"
    "\n"
    "Pouziti:\n"
    "-d, --decode         Dekoduje vstup, ten muze obsahovat pouze znaky '1' a '0'\n"
    "-e, --encode         Zakoduje vstup, ten muze obsahovat pouze znaky a-z, A-Z a 0-9\n"
    "-h, --help           Zobrazi tuto napovedu\n"
    "-c, --coder (X,Y,Z)  Umozni specifikovat jiny koder s kodovym pomerem 1/2.\n"
    "                     X ... počet opoždovacích bloků\n"
    "                     Y ... horní zpětná vazba kódována binárně, zapsána dekadicky\n"
    "                     Z ... dolní zpětná vazba kódována binárně, zapsána dekadicky\n"
    "                     Výchozí je --coder (5,53,46)\n"
    "\n"
    "--decode a --encode nelze pouzit zaroven\n";



int main(int argc, char const *argv[]) {
    CoderMode coderMode = UNDEFINED;

    size_t coderSize = DEF_SIZE;
    Bits coderUpper = DEF_UPPER, coderLower = DEF_LOWER;

    if (argc < 2) {
        throw runtime_error("Nebyli zadany zadne argumenty! Pouzijte ./bms --help pro zobrazeni napovedy.");
    }

    for (auto i = 1; i < argc; ++i) {
        // Kodovani
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            cout << HELP << endl;
            return 0;
        }
        // Kodovani
        else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--encode") == 0) {
            if (coderMode != UNDEFINED) {
                throw runtime_error("Parametry --encode a --decode nelze kombinovat! Viz ./bms --help.");
            }
            coderMode = ENCODE;
        }
        // Dekodovani
        else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decode") == 0) {
            if (coderMode != UNDEFINED) {
                throw runtime_error("Parametry --encode a --decode nelze kombinovat! Viz ./bms --help.");
            }
            coderMode = DECODE;
        }
        // Vlastni koder
        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--coder") == 0) {
            if (++i == argc) {
                throw runtime_error("Byl zadan paramter --coder bez hodnoty! Pouziti: --coder (X,Y,Z), viz ./bms --help.");
            }

            smatch matches;

            string input = string(argv[i]);  // Nemuzu pouzit docasny string
            regex_match(input, matches, regex("^\\((\\d+),(\\d+),(\\d+)\\)$"));

            // V matches se krome cisel vyskytuje i cely string, proto 4
            if (matches.size() != 4) {
                throw runtime_error("Spatny format pro argument '--c'! Pouziti: --coder (X,Y,Z), viz ./bms --help.");
            }
            coderSize = stoull(matches[1]);
            coderUpper = stoull(matches[2]);
            coderLower = stoull(matches[3]);
        } else {
            throw runtime_error("Neznamy argument '" + string(argv[i]) + "'!");
        }
    }

    // Inicializuji koder
    Coder coder(coderSize, coderUpper, coderLower);

    // Nactu data ze vstupu uzivatele
    string userInput;
    string tmp;
    while (getline(cin, tmp)) {
        userInput += tmp;
    }

    switch (coderMode) {
        case ENCODE:
            cout << encode(coder, userInput) << endl;
            return 0;

        case DECODE:
            cout << decode(coder, userInput) << endl;
            return 0;

        default:
            break;
    }

    return 1;
}

/**
 * @brief Najde konkretni stav ve vectoru stavu, hledani probiha az od zadaneho indexu
 * 
 * @param states Vector stavu
 * @param startPointer Vychozi index
 * @param stateBits Obsah stavu (data), podle ktereho probiha porovnani
 * @param index Po ukonceni hledani obsahuje index prvku (byl-li nalezen)
 * @return True, pokud takovy stav existuje, jinak false
 */
const bool findState(const vector<decodeState> &states, const size_t startPointer, const Bits stateBits, size_t &index) {
    for (index = startPointer; index < states.size(); ++index) {
        if (states[index].bits == stateBits) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Koduje zadany retezec
 * @param coder Reference na inicializovany koder
 * @param inputString Vstup ke kodovani. Muze obsahovat pouze a-z, A-Z, 0-9.
 * @return const string Kodovany vstup
 */
const string encode(Coder &coder, const string &inputString) {
    // Vystupni string
    string output = "";

    const size_t dataLen = inputString.size();

    // Iteruji pres vsechny znaky
    uint16_t input = inputString[dataLen - 1];
    for (size_t i = 1; i <= dataLen; ++i) {
        // Musim zaroven nacist i dalsi byte
        input |= inputString[dataLen - i - 1] << 8;

        // Kontrola na ASCII
        if (!isASCII(input & 0xff)) {
            cerr << "Invalid char '" << string(1, input) << "'" << endl;
            input = input >> 8;
            continue;
        }

        // Postupne projdu vsechny bity
        for (auto i = 0; i < 8; ++i) {
            // Nactu prvni bit do koderu
            output = twoBits2Str(coder.load(input & 1)) + output;
            // Posunu bity
            input = input >> 1;
        }
    }

    // Vycisteni koderu
    for (size_t i = 0; i < coder.size(); ++i) {
        // Nacitam uz jen nuly
        output = twoBits2Str(coder.load(0)) + output;
    }

    return output;
}

/**
 * @brief Dekoduje zadany retezec
 * @param coder Reference na inicializovany koder
 * @param inputString Vstupni string k dekodovani. Můze obsahovat pouze '0' a '1'.
 * @return const string Dekodovany vstup
 */
const string decode(Coder &coder, const string &inputString) {
    // Predpriprava dat
    const char *data = inputString.data();

    // Buffer se vsemi stavy
    vector<decodeState> states;
    // Vlozim vychozi stav
    states.push_back({Bits(0), 0, Price(0), Bit(0)});

    // Ukazatele do _state_ na zacatek stavu aktualni/pristi iterace
    size_t actualStepPointer = 0, nextStepPointer;

    size_t pointer = inputString.size() - 2;
    while (true) {
        // Nacitam tak dlouho, dokud nenajdu dve '1'/'0'
        int counter = 1;
        Bits input = 0;

        while (counter >= 0) {
            char c = data[pointer];
            switch (c) {
                case '0':
                    --counter;
                    ++pointer;
                    break;

                case '1':
                    input |= BIT_1 << counter;
                    --counter;
                    ++pointer;
                    break;

                default:
                    cerr << "Invalid char '" << c << "'" << endl;
                    break;
            }
        }

        // Vsechny od ted pridane stavy spadaji do pristi iterace
        nextStepPointer = states.size();

        // Iteruji pres vsechny stavy z minule iterace
        for (; actualStepPointer < nextStepPointer; ++actualStepPointer) {
            // Ukazatel je citelnejsi
            decodeState *s = &states[actualStepPointer];

            // Jdu pres 0
            Bits tmpState = Coder::put0(s->bits);
            Price price = Coder::hammnigDistance(coder.trellisDiagram(s->bits, BIT_0), input);

            // Zjistim, zda jsem uz dany stav nevytvoril
            size_t index;
            if (findState(states, nextStepPointer, tmpState, index)) {
                // Stav existuje, zjistim, zda ma mensi cenu
                if (states[index].price > s->price + price) {
                    states[index].price = s->price + price;
                    states[index].prev = actualStepPointer;
                }
            } else {
                states.push_back({tmpState, actualStepPointer, s->price + price, BIT_0});
            }

            // Jdu pres 1
            tmpState = Coder::put1(s->bits, coder.size());
            price = Coder::hammnigDistance(coder.trellisDiagram(s->bits, BIT_1), input);

            // Zjistim, zda jsem uz dany stav nevytvoril
            if (findState(states, nextStepPointer, Coder::put1(s->bits, coder.size()), index)) {
                // Stav existuje, zjistim, zda ma mensi cenu
                if (states[index].price > s->price + price) {
                    states[index].price = s->price + price;
                    states[index].prev = actualStepPointer;
                }
            } else {
                states.push_back({tmpState, actualStepPointer, s->price + price, BIT_1});
            }
        }

        if (pointer == 2) {
            break;
        } else if (pointer < 4) {
            throw runtime_error("Na vstupu prebyva nebo chybi jeden symbol!");
        }
        pointer -= 4;
    }

    // Posledni stavy jsou od actualStepPointer
    // Najdu ten s nejnizsi cenou => vysledna cesta
    Price minimalPrice = states[actualStepPointer].price;
    size_t way = actualStepPointer;

    for (; actualStepPointer < states.size(); ++actualStepPointer) {
        // Co delat, kdyz existuji dve se stejnou cenou?
        if (states[actualStepPointer].price < minimalPrice) {
            minimalPrice = states[actualStepPointer].price;
            way = actualStepPointer;
        }
    }

    // Nyni mam cestu a muzu ji prevest zpet na bity a puvodni ASCII znaky
    string output = "";
    char c = 0;
    uint8_t offset = 0b10000000;   // Ukazatel na ukatualni pozici v jednom byte
    size_t ignore = coder.size();  // Musim ignorovat x prvnich znaku = vzniknou vycistenim zasobniku koderu

    // Dokud nedojdu na prvni prvek
    while (way) {
        // Ignoruji prvni znaky
        if (ignore) {
            ignore = ignore > 1 ? ignore - 1 : 0;
        } else {
            // Nactu bit a zapisu ho
            c |= states[way].bit ? offset : 0;

            offset >>= 1;
            // Nacetl jsem byte, muzu ho zapsat na vystup
            if (!offset) {
                offset = 0b10000000;
                output += c;
                c = 0;
            }
        }

        way = states[way].prev;
    }

    // Pocet prijatych bitu neni delitelny 8 -> zprava nebyla uplna
    if (offset != 0b10000000) {
        output += c;

        cerr << "Prijaty pocet bitu neni nasobkem osmy, zprava muze byt poskozena." << endl;
    }

    return output;
}