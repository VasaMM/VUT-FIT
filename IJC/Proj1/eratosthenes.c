/** HEADER *****************
	eratosthenes.c
	Reseni IJC-DU1, 23.3.2016
	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
	Prelozeno: gcc 4.9.3

  Algoritmus Eratostenovo sito
****************************/


//#include <math.h>

#include "bit_array.h"
#include "eratosthenes.h"


///!Pozor! prvocisla oznaceny 0 !!!
void Eratosthenes(bit_array_t pole) {

	//Cisla 0 a 1 nejsou prvocisly
	ba_set_bit(pole, 0, 1);
	ba_set_bit(pole, 1, 1);

	//Suda cisla nemuzou byt prvocisly (krome 2)
	for(unsigned long i = 4 ; i < ba_size(pole) ; i += 2) {
		ba_set_bit(pole, i, 1);
	}

	///Kontroluji jen licha cisla - PREDELAT?
	for(unsigned long i = 3 ; i <= ba_size(pole)/2 ; i += 2) {
			if(ba_get_bit(pole,i)) {	//Pokud narazime na 1, uz nemusime skrtat nasobky (uz jsou vyskrtane)
				continue;
			}
	    for(unsigned long n = 2; (n * i) < ba_size(pole); n++) {
			ba_set_bit(pole, (n * i), 1);	//skrtame nasobky lichych cisel - nemohou byt prvocisly
	    }
	}
}

