/** HEADER *****************
	ppm.c
	Reseni IJC-DU1, 23.3.2016
	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
	Prelozeno: gcc 4.9.3

	Prace s obrazky .ppm
****************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ppm.h"
#include "error.h"


///Nacteni obrazku do struktury
struct ppm *ppm_read(const char *filename){
	//Otevru soubor
	FILE *picture = NULL;
	if((picture = fopen(filename, "rb")) == NULL) {
		warning_msg("Nepodarilo se otevrit soubor. Existuje?\n");
		return NULL;
	}

	//Ctu hlavicku
	char p6[3];
	unsigned xsize, ysize, colors;
	if((fscanf(picture, "%2s %u %u %u\n", p6, &xsize, &ysize, &colors) != 4) || !(strcmp("p6", p6))) {
		warning_msg("Pri cteni hlavicky souboru doslo k chybe. Spravny soubor?\n");
		fclose(picture);
	}

	//Alokuji strukturu
	struct ppm *pic = NULL;
	if((pic = malloc(sizeof(struct ppm) + 3 * xsize * ysize)) == NULL) {
		warning_msg("Nedostatek pameti!\n");
		fclose(picture);
	}

	//Vyplnim hlavicku struktury
	pic->xsize = xsize;
	pic->ysize = ysize;

	//Nactu obrazek do struktury
	if(fread(pic->data, 1, 3 * xsize * ysize, picture) != 3 * xsize * ysize) {
		warning_msg("Nepodarilo se precist cely soubor. Je poskozeny?\n");
		free(pic);
		fclose(picture);
	}

	//Zavru soubor
	fclose(picture);

	//Vratim strukturu
	return pic;
}


///Zapise strukturu do souboru
int ppm_write(struct ppm *p, const char *filename){
	//Otevru soubor
	FILE *picture = NULL;
	if((picture = fopen(filename, "wb")) == NULL) {
		warning_msg("Nepodarilo se vytvorit soubor. Nedostatecna prava?\n");
		return -1;
	}

	//Zapisu hlavicku
	if(fprintf(picture, "P6\n %i %i\n 255\n", p->xsize, p->ysize) == EOF) {
		warning_msg("Nepodarilo se zapsat hlavicku. Malo mista?\n");
		fclose(picture);
		return -1;
	}

	//Zapisu obsah
	unsigned pic_size = 3 * p->xsize * p->ysize;
	if(fwrite(p->data, 1, pic_size, picture) != pic_size) { ///??? p->data + 1  PROC?
		warning_msg("Nepodarilo se zapsat cely soubor. Malo mista?\n");
		fclose(picture);
		return -1;
	}

	//Zavru soubor
	fclose(picture);

	return 0;
}
