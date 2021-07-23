/** HEADER *****************
	primes.c
	Reseni IJC-DU1, 23.3.2016
	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
	Prelozeno: gcc 4.9.3

	Tisk poslednich 10 prvocisel mensich nez 202M
****************************/


#include <stdio.h>
#include <stdlib.h>

#include "eratosthenes.h"
#include "bit_array.h"


int main(void) {
	ba_create(array,202000001); //Vytvorim pole pro ukladani prvocisel i s 0

	Eratosthenes(array);	//spocitam prvocisla
	
	unsigned long primes[10] = {0};
	unsigned long i = ba_size(array) - 1;
	for(unsigned j = 10 ; j > 0; i-- ) {
		if(!ba_get_bit(array,i)) {	//Prvocisla ulozena jako 0 !!!
		primes[j-1] = i;
		j--;
		}
	}

	//Tisknu prvocisla
	for(i = 0; i < 10 ; i++ ) {
		printf("%lu\n", primes[i]);
	}

	return 0;
}

