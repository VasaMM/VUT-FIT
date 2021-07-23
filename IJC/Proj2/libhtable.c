/** HEADER *****************
	libtable.c
	Reseni IJC-DU2, 27.4.2016
	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
	Prelozeno: gcc 4.9
****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "htable.h"
#include "error.h"


/**********
Hashovaci funkce
	Parametry string a velikost tabulky
	Opsana ze zadani
*/
unsigned hash_function(const char *str, unsigned htab_size) {
	unsigned h=0;
	const unsigned char *p;

	for(p = (const unsigned char*)str ; *p != '\0' ; p++) {
		h = 65599*h + *p;
	}

	return h % htab_size;
}



/***********
Inicializace htab_t
	Parametry: size
	Je pouzita implicitni hashovaci funkce
	V pripade chyby vraci NULL
*/
htab_t *htab_init(unsigned htab_size) {
	htab_t *output = 0;

	output = malloc(sizeof(htab_t) + htab_size * sizeof(struct htab_listitem));
		if(!output) return NULL;

	output->htab_size = htab_size;
	output->hash_fun_ptr = &hash_function;
	output->n = 0;

	for(unsigned i = 0 ; i < htab_size ; i++) {
		output->ptr[i] = 0;
	}

	return output;
}



/***********
Inicializace htab_t
	Parametry: size a ukazatel na hashovaci funkci
	Neni-li druhy argument zadan, je pouzita implicitni hashovaci funkce
	V pripade chyby vraci NULL
*/
htab_t *htab_init2(unsigned htab_size, unsigned (*hashfn)(const char *, unsigned)) {
	htab_t *output = 0;

	output = malloc(sizeof(htab_t) + htab_size * sizeof(struct htab_listitem));
		if(!output) return NULL;

	output->htab_size = htab_size;
	output->hash_fun_ptr = hashfn;
	output->n = 0;

	for(unsigned i = 0 ; i < htab_size ; i++) {
		output->ptr[i] = 0;
	}

	return output;
}



/************
	Vyhleda a smaze konkretni polozku v tabulce
*/
void htab_remove(htab_t *t, char* key) {
	struct htab_listitem *next_item, *tmp;

	for(unsigned i = 0 ; i < t->htab_size ; i++) {
		if(!(next_item = t->ptr[i])) continue;	//preskoci zaznam, pokud je prazdny

			if(!(strcmp(next_item->key, key))) {	//nasel jsem, mazu
				//t->n -= next_item->data;
				t->n --;
				free(next_item->key);
				free(next_item);
				t->ptr[i] = 0;
				return;
			}

		while(next_item) {	//jinak projdu zbytek tabulky
			tmp = next_item;
			next_item = next_item->next;

			if(next_item && !(strcmp(next_item->key, key))) {	//pokud existuje next a nasel jsem, mazu
				tmp->next = next_item->next;
				//t->n -= next_item->data;
				t->n --;
				free(next_item->key);
				free(next_item);
				return;
			}
		}
	}
}



/************
	Smaze vsechny polozky v tabulce
*/
void htab_clear(htab_t *t) {
	struct htab_listitem *next_item, *tmp;

	for(unsigned i = 0 ; i < t->htab_size ; i++) {
		if(!t->ptr[i]) continue;	//prazdny radek tabulky preskoci

		if(!(next_item = t->ptr[i]->next)) {
			t->ptr[i]->data = 0;
			if(t->ptr[i]->key)
				free(t->ptr[i]->key);
			t->ptr[i]->key = 0;
			continue;
		}


		while(next_item) {
			tmp = next_item;
			next_item = next_item->next;

			free(tmp->key);
			free(tmp);
		}
	}

	t->n = 0;
}



/************
Smaze htab_t
	Parametry: ukazatel na htab_t
*/
void htab_free(htab_t *t) {
	htab_clear(t);
	free(t);
}



/*************
V tabulce t vyhleda zaznam odpovidajici retezci key
	- pokud jej nalezne, vrati ukazatel na zaznam
	- pokud nenalezne vraci NULL
*/
struct htab_listitem *htab_lookup(htab_t *t, const char *key) {
	struct htab_listitem *output;

	if(!(output = t->ptr[t->hash_fun_ptr(key, t->htab_size)])) return NULL;	//preskoci zaznam, pokud je prazdny

	if(!(strcmp(output->key, key))) {	//nasel jsem
		output->data++;	//pocet vyskytu +1
		//t->n ++; // Pocet zaznamu
		return output;
	}

	while(output) {	//jinak projdu zbytek tabulky
		output = output->next;

		if(output && !(strcmp(output->key, key))) {	//pokud existuje next a nasel jsem
			output->data++;	//pocet vyskytu +1
			//t->n ++; // Pocet zaznamu
			return output;
		}
	}

	//Nenasel
	return NULL;
}



/*************
V tabulce t prida zaznam odpovidajici retezci key
	- vraci ukazatel, v pripade chyby NULL
*/
struct htab_listitem *htab_add(htab_t *t, const char *key) {
	struct htab_listitem *output;

	output = t->ptr[t->hash_fun_ptr(key, t->htab_size)];

	if(!output) {	//zaznam je prazdny
		output = (t->ptr[t->hash_fun_ptr(key, t->htab_size)] = malloc(sizeof(struct htab_listitem)));
			if(!output) return NULL;
	}
	else {
		while(1) {	//dojdu na konec
			if(!output->next) {
				break;
			}
			output = output->next;
		}
		//Pridam zaznam
		output->next = malloc(sizeof(struct htab_listitem));
			if(!output->next) return NULL;

		output = output->next;
	}

	output->next = 0;
	output->data = 1;	// Pocet vyskytu
	t->n ++; // Pocet zaznamu

	output->key  = malloc((strlen(key) + 1) * sizeof(char));	//Funguje i s /0???
		if(!output->key) return NULL;
	strcpy(output->key, key);

	return output;
}



/*************
V tabulce t vyhleda zaznam odpovidajici retezci key
	- pokud jej nalezne, vrati ukazatel na zaznam
	- pokud nenalezne, automaticky prida zaznam a vrati ukazatel
	- chyba vraci NULL
*/
struct htab_listitem *htab_lookup_add(htab_t *t, const char *key) {
	struct htab_listitem *output = htab_lookup(t, key);

	if(output) {
		return output;
	}

	output = htab_add(t, key);

	return output;
}






/*Vytiskne vysledky pocitani
		slovo (klic)	(\t)	pocet (data)
*/
void htab_print(htab_t *t) {
	struct htab_listitem *tmp;

	for(unsigned i = 0 ; i < t->htab_size ; i++) {
		if(!(tmp = t->ptr[i])) continue;	//preskocim prazdny radek

		while(tmp) {
			printf("%s\t%i\n", tmp->key, tmp->data);
			tmp = tmp->next;
		}
	}
}



/**************
	Vola zadanou funkci pro kazdy prvek tabulky, obsah tabulky nemeni.
		const char* ... key
		unsigned ... data
*/
void htab_foreach(htab_t *t, void (*func)(const char*, unsigned)) {
	struct htab_listitem *tmp;

	for(unsigned i = 0 ; i < t->htab_size ; i++) {
		if(!(tmp = t->ptr[i])) continue;	//preskocim prazdny radek

		while(tmp) {
			func(tmp->key, tmp->data);
			tmp = tmp->next;
		}
	}
}
