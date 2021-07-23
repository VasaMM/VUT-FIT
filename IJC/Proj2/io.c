/** HEADER *****************
	io.c
	Reseni IJC-DU2, 4.4.2016
	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
	Prelozeno: gcc 4.9
****************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/***************
	Nacte jedno slovo ze souboru f do zadaneho pole znaku
	Vraci delku slova (cte jen 'max' znaku, zbytek preskoci
	Pri konci souboru vraci EOF
*/
int get_word(char *s, int max, FILE *f) {
	int i, tmp_char = fgetc(f);

	//ignorace prazdnych znaku na vstupu + kontrola konce
	while(!isspace(tmp_char)) {
		tmp_char = fgetc(f);
		if(tmp_char == EOF) return EOF;
	}

	for(i = 0 ; i < max ; i++) {
		if(isspace(tmp_char) || tmp_char == EOF) {
			s[i]=0;
			return i;
		}
		s[i] = tmp_char; ///??? Netisknutelne znaky
		tmp_char = fgetc(f);
	}

	if((i == max - 1) && s[max]) { //ignoruji prebytecne znaky
		s[max] = 0;
		while(!isspace(tmp_char = fgetc(f)) || tmp_char == EOF) {}
	}

return i;
}
