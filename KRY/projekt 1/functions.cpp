/**
 * @file functions.cpp
 * @version 1.0
 * @author Vaclav Martinka
 * @date 03. 04. 2021
 * @brief Pomocne funkce pro _Projekt 1 (KRY)_, VUT Brno
 */

#include <cstring>
#include <string>
#include <algorithm>
#include <iostream>

#include "functions.hpp"

using namespace std;



Mode parseArg(const std::string &arg) {
	if (arg == "--help" || arg == "-h") {
		return HELP;
	}
	else if (arg == "--decode" || arg == "-d") {
		return DECODE;
	}
	else if (arg == "--encode" || arg == "-e") {
		return ENCODE;
	}
	else if (arg == "--attack" || arg == "-a") {
		return ATTACK;
	}
	else if (arg == "--solve" || arg == "-s") {
		return SOLVE;
	}

	return UNKNOWN;
}


std::vector<int> factors(const int n) {
	std::vector<int> out;

	for (auto i = 2; i < n; ++i) {
		if (n % i == 0) {
			out.push_back(i);
		}
	}
	
	out.push_back(n);

	return out;
}


string readInput() {
	string tmp, input;

	// nactu vstup
	while (getline(cin, tmp)) {
		input += tmp;
	}
	// input = "MRGFNIATXZQVFFNUXFFYBTCETYXIIXGZKACJLRGKQYEIXOYYAUAPXYIJLHPRGVTSFPAYNNYURZOPHXWYXLFRNUTZBRFKAHFWFZESYUWZMOLLBSBZBJHFPLXKHVIVMZTZHUIWAETIUEDFGLXDIEXIYJIUXPNNEIXABVCINTVCIEZYYDAZGZIWTYXJIKTRZLMFFKALGZNVKZXIIMXUUNAPGVXFUSMISKHVYVOCRVXRIWTYXZOIRFNUXZNXLDUDPZGVHVOWMOYJERLAUGLVTUXTHRBUQZTYTXORNKBASFFXGHQVDSHUYJSYHDYUWYXYYKHVTUCDACAHXSEVGJIEFZGLXRSBXSYKOEPPNYAKTUACEFYILFWEAHCIAUALLZNXMVCKLRRHGFNXMOYUESKPM";

	// Prevedu na velka pismena
	transform(input.begin(), input.end(), input.begin(), ::toupper);

	// Odstranim jine znaky
    input.erase(remove_if(input.begin(), input.end(), [](unsigned char x){return x < 'A' || x > 'Z';}), input.end());

	log("input", input);

	return input;
}

