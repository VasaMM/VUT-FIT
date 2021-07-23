/**
 * @file ecc.cpp
 * @author Vaclav Martinka (xmarti76)
 * @brief Implementace ecc.hpp
 * @date 2021-05-02
 * 
 * @copyright CC-BY 4.0
 * 
 */

#include <gmpxx.h>
#include <iostream>
#include "ecc.hpp"

using namespace std;


bool Curve::onCurve(Point p) {
	mpz_class l = modulo(p.y() * p.y());
	mpz_class r = modulo(p.x() * p.x() * p.x() + _a * p.x() + _b);
	mpz_class c = 4 * _a * _a * _a + 27 * _b * _b;

	return l == r && c != 0;
}


Point Curve::add(Point p, Point q) {
	Point r;      
	
	if (p == r) {return q;}
	if (q == r) {return p;}
	
	if (p == q) {
		if (p.y() != 0) {
			mpz_class l = modulo(modulo(3 * p.x() * p.x() + _a) * inverse(2 * p.y()));
			r.x(modulo(l * l - 2 * p.x()));
			r.y(modulo(l * (p.x() - r.x()) - p.y()));
		}
	}
	else {
		if (q.x() - p.x() != 0) {
			mpz_class l = modulo(modulo(q.y() - p.y()) * inverse(q.x() - p.x()));
			r.x(modulo(l * l - p.x() - q.x()));
			r.y(modulo(l * (p.x() - r.x()) - p.y()));
		}
	}
		
	return r;
}
