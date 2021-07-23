/**
 * @file main.cpp
 * @author Vaclav Martinka (xmarti76)
 * @brief Druhy projekt do predmetu KRY - 2020 FIT VUTBR
 * @date 2021-05-02
 * 
 * @copyright CC-BY 4.0
 * 
 */
#include <gmpxx.h>
#include <iostream>
#include "ecc.hpp"

using namespace std;


int main(int argc, char const *argv[]) {
	mpz_class result(0);

	if (argc == 2) {
		string input = argv[1];

		size_t fromX, toX, fromY, toY;
		fromX = input.find("(0x");
		toX   = input.find(',');
		fromY = input.find("0x", toX);
		toY   = input.find(")");

		if (fromX != string::npos && toX != string::npos && fromY != string::npos && toY != string::npos) {
			Point base(
				mpz_class("6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296", 16),
				mpz_class("4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5", 16)
			);

			Point publicKey(
				mpz_class(input.substr(fromX + 3, toX - 3), 16),         // 3 je delka "(0x"
				mpz_class(input.substr(fromY + 2, toY - fromY - 2), 16)  // 2 je delka "0x"
			);

			Curve curve(
				mpz_class(-0x3),
				mpz_class("5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b", 16),
				mpz_class("ffffffff00000001000000000000000000000000ffffffffffffffffffffffff", 16)
			);

			if (curve.onCurve(base) && curve.onCurve(publicKey)) {
				Point r;

				// Postupne pricitam, nez se doberu stejneho klice
				for (mpz_class d = 1; d < curve.p(); ++d) {
					r = curve.add(base, r);
					
					if (! curve.onCurve(r)) {
						break;
					}

					if (r == publicKey) {
						result = d;
						break;
					}
				}
			}
		}
	}

	cout << result << endl;
	return 0;
}
