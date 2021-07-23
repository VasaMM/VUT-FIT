/** HEADER *****************
	ppm.h
	Reseni IJC-DU1, 23.3.2016
	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
	Prelozeno: gcc 4.9.3

	Struktura pro praci s obrázky .ppm
****************************/


#ifndef ppm_header
#define ppm_header

typedef struct ppm {
	unsigned xsize;
	unsigned ysize;
	char data[];	// RGB bajty, celkem 3 * xsize * ysize
}struct_ppm;

	struct ppm *ppm_read(const char *filename);

	int ppm_write(struct ppm *p, const char *filename);

#endif

