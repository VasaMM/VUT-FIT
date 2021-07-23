/**
 * @file Coder.cpp
 * @author Václav Matinka
 * @date 07. 12. 2020 (19:15)
 */

#include <iostream>
#include "Coder.hpp"


using namespace std;


const string bits2str(const Bits bits, size_t length) {
	if (!length) {
		length = sizeof(Bits) * 8;
	}

	string output = "";
	for (Bits i = length; i > 0; --i) {
		output += bitAtPos(bits, i - 1) ? "1" : "0";
	}
	return output;
}



Coder::Coder(const size_t stages, const Bits upper, const Bits lower) {
	// TODO kontrola na maximalni velikost upper a lower, neni zivotne dulezita

	_size = stages;
	_upper = upper;
	_lower = lower;


	// Maximalni velikost registru
	if (stages > (sizeof(Bits) * 8 -1)) {
		throw runtime_error("Velikost vnitrniho registru je prilis velka. Maximalni je " + to_string(static_cast<long long unsigned int>(sizeof(Bits) * 8 - 1)));
	}


	// Iteruji pres vsechny mozne stavy a generuji trellis diagram
	for (Bits state = 0; state < Bits(1 << stages); ++state) {
		vector<TwoBits> newStates(2);

		// Nastavim state jako aktualni stav
		_data = state;

		// Ulozim si vysledek (mam 0 na vstupu)
		newStates[BIT_0] = (xorSum(upper & _data) << 1) | xorSum(lower & _data);


		// Nastavim 1 na vstup
		_data |= 1 << (size());
		newStates[BIT_1] = (xorSum(upper & _data) << 1) | xorSum(lower & _data);
		
		trellisDiagram().push_back(newStates);
	}

	// Kontrolni vypis
	#ifdef DEBUG
	for (Bits i = 0; i < (Bits(1) << stages); ++i) {
		cout << "from " << bits2str(i, stages) << " by 0 to " <<  bits2str(Coder::put0(i),         stages) << " output is " << twoBits2Str(trellisDiagram(i, BIT_0)) << endl;
		cout << "from " << bits2str(i, stages) << " by 1 to " <<  bits2str(Coder::put1(i, stages), stages) << " output is " << twoBits2Str(trellisDiagram(i, BIT_1)) << endl;
	}
	#endif


	// Musim vynulovat data
	_data = 0;
}



const string Coder::str() {
	string line1, line2, line3;

	size_t i = size() + 1;
	while (true) {
		--i;
		line1 += bitAtPos(_upper, i) ? "v" : " ";
		line2 += bitAtPos(_data,  i) ? "1" : "0";
		line3 += bitAtPos(_lower, i) ? "^" : " ";
		
		if (! i) {break;}
	}
	
	line2 += "  " + to_string(xorSum(_upper & _data)) + to_string(xorSum(_lower & _data));
	

	return line1 + "  .\n" + line2 + "\n" + line3 + "   .\n";
}



const TwoBits Coder::load(const Bit bit) {
	// Posunu buffer	
	_data >>= 1;

	TwoBits output = trellisDiagram(_data, bit > 0 ? BIT_1 : BIT_0);

	// Ulozim vstup na prvni pozici
	_data |= bit << (size());

	return output;
}



const Bit Coder::xorSum(const Bits set) {
	// Vystup
	Bit output = 0;

	for (size_t i = 0; i <= size(); ++i) {
		// Provedu xor
		output ^= bitAtPos(set, i) > 0;
	}

	return output;
}



const Price Coder::hammnigDistance(const Bits data_a, const Bits data_b) {
	Bits tmp = data_a ^ data_b;

	uint output = 0;

	while(tmp) {
		output += tmp & 1;
		tmp >>= 1;
	}

	return output;
}