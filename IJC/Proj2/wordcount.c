/** HEADER *****************
	wordcoun.c
	Reseni IJC-DU2, 4.4.2016
	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
	Prelozeno: gcc 4.9

	!!! popis
****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "htable.h"
#include "error.h"



/** MAIN */
int main(void) {
	htab_t *table = htab_init(12289); //http://planetmath.org/goodhashtableprimes
	//              htab_init2(12289, &hash_function);

	read_words(table);
	//htab_remove(table, "ahoj");
	htab_print(table);

	htab_free(table);

	return 0;
}


/************
	Nacte a rovnou prida slova ze stdin
	Max. delka slova je MAX_LENGTH, zbytek preskoci
*/
void read_words(htab_t *t) {
	char word[MAX_LENGTH + 1] = {0};
	int err = 0, erros = 0;
	char tmp = 0;

	while(1) {
		err = scanf("%" STRINGIFY(MAX_LENGTH) "s%c", word, &tmp);
		if(err == EOF) {
			return;
		}
		if(!isspace(tmp)) {	//retezec neskocil whitespace
			erros ++;
			word[MAX_LENGTH] = 0;
			if(erros == 1) {	//Varuji jen poprve
				fprintf(stderr, "ERROR: Prekrocen maximalni pocet znaku (%i) ve slove %s", MAX_LENGTH, word);
			}
			while(!isspace(err = getchar()) || err == EOF) {
				fputc(err, stderr);
			}
			fputc('\n', stderr);
		}

		htab_lookup_add(t, word);
	}
}
