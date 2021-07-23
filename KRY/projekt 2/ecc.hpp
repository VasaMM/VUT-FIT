/**
 * @file ecc.hpp
 * @author Vaclav Martinka (xmarti76)
 * @brief Pomocne tridy pro vypocty nad ECC
 * @date 02. 05. 2021 (13:35)opyright CC-BY 4.0
 * 
 */
#pragma once

#include <gmpxx.h>

/// Trida popisujici jeden bod
class Point {
private:
	mpz_class _x;  ///< Souradnice x
	mpz_class _y;  ///< Souradnice y

public:
	/// Konstruktor nuloveho bodu
	Point() {
		_x = 0;
		_y = 0;
	}
	

	/**
	 * @brief Konstruktor bodu se souradnici [x, y]
	 * 
	 * @param x Souradnice x
	 * @param y Souradnice y
	 */
	Point(mpz_class x, mpz_class y) {
		_x = x;
		_y = y;
	}


	/**
	 * @brief Operator pro porovnani bodu => musi mit stejne souradnice
	 * 
	 * @param other Druhy bod
	 * @return true Body jsou stejne
	 * @return false Body jsou ruzne
	 */
	inline bool operator== (const Point &other) const {
		return _x == other._x && _y == other._y;
	}


	/// @brief Ziska x souradnici bodu
	/// @return mpz_class& Souradnice x
	mpz_class x() {return _x;}


	/// @brief Nastavi x souradnici bodu
	/// @param val Nova hodnota
	void x(mpz_class val) {_x = val;}
	

	/// @brief Ziska x souradnici bodu
	/// @return mpz_class& Souradnice x
	mpz_class y() {return _y;}


	/// @brief Nastavi x souradnici bodu
	/// @param val Nova hodnota
	void y(mpz_class val) {_y = val;}
};


/// Trida popisujici eliptickou krivku
class Curve {
private:
	mpz_class _a;  ///< Parametr a
	mpz_class _b;  ///< Parametr b
	mpz_class _p;  ///< Fp

public:
	/**
	 * @brief Kontruktor krivky `y^2 = x^3 + ax + b mod p`
	 * 
	 * @param a Parametr a
	 * @param b Parametr b
	 * @param p Fp
	 */
	Curve(mpz_class a, mpz_class b, mpz_class p) {
		_a = a;
		_b = b;
		_p = p;
	}

	/**
	 * @brief Zjisti, zda je bod na krivce
	 * 
	 * @param p Bod
	 * @return true Bod lezi na krivce 
	 * @return false Bod lezi mimo krivku
	 */
	bool onCurve(Point p);

	/**
	 * @brief Vypocte modulo
	 * 
	 * @param x Vstupni cislo
	 * @return mpz_class& x modulo p
	 */
	mpz_class modulo(mpz_class x) {
		mpz_mod(x.get_mpz_t(), x.get_mpz_t(), _p.get_mpz_t());
		return x;
	}
	
  
	/**
	 * @brief Vypocte multiplikativni inverzi
	 * 
	 * @param x Vstupni cislo
	 * @return mpz_class Vysledek
	 */
	mpz_class inverse(mpz_class x) {
		mpz_invert(
			x.get_mpz_t(),
			x.get_mpz_t(),
			_p.get_mpz_t()
		);

		return x;
	}


	/**
	 * @brief Secte dva body na krivce
	 * 
	 * @param p Prvni bod
	 * @param q Druhy bod
	 * @return Point Vysledny bod
	 */
	Point add(Point p, Point q);


	/**
	 * @brief Pricte bod sam k sobe
	 * 
	 * @param p Bod
	 * @return Point Výsledny bod
	 */
	Point doubleMe(Point p) {
		return add(p, p);
	}

	
	/// @brief Ziska parametr a
	/// @return mpz_class Parametr a
	mpz_class a() {return _a;}
	
	
	/// @brief Ziska parametr b
	/// @return mpz_class Parametr b
	mpz_class b() {return _b;}


	/// @brief Ziska hodnotu Fp
	/// @return mpz_class Hodnotu Fp
	mpz_class p() {return _p;}
};
