/** HEADER *****************
	steg_decode.c
	Reseni IJC-DU1, 23.3.2016
	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
	Prelozeno: gcc 4.9.3

	Ctu sifrovane zpravy v ppm
****************************/


#include <stdio.h>
#include <stdlib.h>

#include "ppm.h"
#include "bit_array.h"
#include "eratosthenes.h"
#include "error.h"


int main(int argc, char *argv[]) {
	//Spatny pocet argumentu
	if(argc != 2) {
		fatal_error("Spatny pocet argumentu. Pouzijte pouze jeden argument - nazev souboru, ktery chcete dekodovat.");
	}

	//Nactu soubor
	struct ppm *picture = ppm_read(argv[1]);
	if(picture == NULL) {
		fatal_error("Nepodarilo se nacist obrazek.");
	}

	//Pripravuji si promenne a pamet pro cteni zpravy
	unsigned long pic_size = 3 * picture->xsize * picture->ysize;
	char *hidden_message;
	if((hidden_message = malloc((pic_size/8)+2)) == NULL) {
		free(picture);
		fatal_error("Nedostatek pameti!\n");
	}

	///Vytvorim pole prvocisel
	unsigned long *primes;
	if((primes = malloc((pic_size/8)+2)) == NULL){
    free(picture);
    fatal_error("Nedostatek pameti!\n");
	}
	for(unsigned i = 0 ; i < ((pic_size/8)+2)/sizeof(unsigned long) ; i++ ) {
		primes[i]=0;
	}

	primes[0] = pic_size;

	Eratosthenes(primes);

	unsigned index = 0;
	for(unsigned i = 0 ; i < ba_size(primes) ; i++ ) {
		if(!ba_get_bit(primes, i)) {
			DU1_SET_BIT(hidden_message, index, (picture->data[i] & 1));
			index++;
		}
	}

	printf("%s\n", hidden_message);

	free(picture);

	return 0;
}
