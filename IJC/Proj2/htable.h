/** HEADER *****************
	wordcount.h
	Reseni IJC-DU2, 25.4.2016
	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
	Prelozeno: gcc 4.9
****************************/

#ifndef wordcount_header
#define wordcount_header

	struct htab_listitem{
		char* key;                   // ukazatel na dynamicky alokovany retezec
		unsigned data;               // pocet vyskytu
		struct htab_listitem *next;  // dalsi zaznam
	};
	//typedef struct htab_listitem htab_listitem;


	typedef struct{
		unsigned htab_size;                                // velikost pole
		unsigned (*hash_fun_ptr)(const char *, unsigned);  // implicitnì obsahuje &hash_function
		unsigned n;                                        // aktuální poèet záznamù
		struct htab_listitem *ptr[];                       // ukazatel na tabulku
	}htab_t;



/*Hashovaci funkce
		Parametry string a velikost tabulky */
	unsigned hash_function(const char *str, unsigned htab_size);


/*Inicializace htab_t
		Parametry: size
		Je pouzita implicitni hashovaci funkce
		V pripade chyby vraci NULL */
	htab_t *htab_init(unsigned htab_size);


/*Inicializace htab_t
		Parametry: size a ukazatel na hashovaci funkci
		Neni-li druhy argument zadan, je pouzita implicitni hashovaci funkce
		V pripade chyby vraci NULL */
	htab_t *htab_init2(unsigned htab_size, unsigned (*hashfn)(const char *, unsigned));


/*Smaze htab_t
		Parametry: ukazatel na htab_t */
	void htab_free(htab_t *t);


//Smaze vsechny polozky v tabulce
	void htab_clear(htab_t *t);


//Vyhleda a smaze konkretni polozku v tabulce
	void htab_remove(htab_t *t, char* key);


/*V tabulce t vyhleda zaznam odpovidajici retezci key
		- pokud jej nalezne, vrati ukazatel na zaznam
		- pokud nenalezne vraci NULL */
	struct htab_listitem *htab_lookup(htab_t *t, const char *key);


/*V tabulce t prida zaznam odpovidajici retezci key
		- vraci ukazatel, v pripade chyby NULL */
	struct htab_listitem *htab_add(htab_t *t, const char *key);


/*V tabulce t vyhleda zaznam odpovidajici retezci key
		- pokud jej nalezne, vrati ukazatel na zaznam
		- pokud nenalezne, automaticky prida zaznam a vrati ukazatel
		- chyba vraci NULL */
	struct htab_listitem *htab_lookup_add(htab_t *t, const char *key);


//Vola zadanou funkci pro kazdy prvek tabulky, obsah tabulky nemeni.
	void htab_foreach(htab_t *t, void (*func)(const char*, unsigned));



/// --- DALSI ---
/*Vytiskne obsah htab_t t
		Testovací fce */
	void htab_print_all(htab_t *t);


/*Vytiskne vysledky pocitani
		slovo (klic)	(\t)	pocet (data)
*/
	void htab_print(htab_t *t);


/*Nacte a rovnou prida slova ze stdin
	Max. delka slova je MAX_LENGTH */
#define MAX_LENGTH 128		//127 + 1 ('\0')
#define STRINGIFY(x) STRINGIFY2(x)	//from http://stackoverflow.com/questions/3301294/scanf-field-lengths-using-a-variable-macro-c-c
#define STRINGIFY2(x) #x

	void read_words(htab_t *t);

#endif
