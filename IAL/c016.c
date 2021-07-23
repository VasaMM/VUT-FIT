
/* c016.c: **********************************************************}
{* Tema:  Tabulka s Rozptylenymi Polozkami
**                      Prvni implementace: Petr Prikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Upravy: Karel Masarik, rijen 2014
**                              Radek Hranicky, rijen 2014
**                              Radek Hranicky, listopad 2015
**                              Radek Hranicky, rijen 2016
**
** Vytvorete abstraktni datovy typ
** TRP (Tabulka s Rozptylenymi Polozkami = Hash table)
** s explicitne retezenymi synonymy. Tabulka je implementovana polem
** linearnich seznamu synonym.
**
** Implementujte nasledujici procedury a funkce.
**
**  HTInit ....... inicializuje tabulku pred prvnim pouzitim
**  HTInsert ..... vlozeni prvku
**  HTSearch ..... zjisteni pritomnosti prvku v tabulce
**  HTDelete ..... zruseni prvku
**  HTRead ....... precteni hodnoty prvku
**  HTClearAll ... zruseni obsahu cele tabulky (inicializace tabulky
**                 pote, co jiz byla pouzita)
**
** Definici typu naleznete v souboru c016.h.
**
** Tabulka je reprezentovana datovou strukturou typu tHTable,
** ktera se sklada z ukazatelu na polozky, jez obsahuji slozky
** klice 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na dalsi synonymum 'ptrnext'. Pri implementaci funkci
** uvazujte maximalni rozmer pole HTSIZE.
**
** U vsech procedur vyuzivejte rozptylovou funkci hashCode.  Povsimnete si
** zpusobu predavani parametru a zamyslete se nad tim, zda je mozne parametry
** predavat jinym zpusobem (hodnotou/odkazem) a v pripade, ze jsou obe
** moznosti funkcne pripustne, jake jsou vyhody ci nevyhody toho ci onoho
** zpusobu.
**
** V prikladech jsou pouzity polozky, kde klicem je retezec, ke kteremu
** je pridan obsah - realne cislo.
*/

#include "c016.h"
#include <stdio.h>

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovaci funkce - jejim ukolem je zpracovat zadany klic a pridelit
** mu index v rozmezi 0..HTSize-1.  V idealnim pripade by melo dojit
** k rovnomernemu rozptyleni techto klicu po cele tabulce.  V ramci
** pokusu se muzete zamyslet nad kvalitou teto funkce.  (Funkce nebyla
** volena s ohledem na maximalni kvalitu vysledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitne zretezenymi synonymy.  Tato procedura
** se vola pouze pred prvnim pouzitim tabulky.
*/

void htInit ( tHTable* ptrht ) {
	if(ptrht) {
		for(unsigned int i = 0 ; i < MAX_HTSIZE ; i++)
			(*ptrht)[i] = NULL;
	}
}

/* TRP s explicitne zretezenymi synonymy.
** Vyhledani prvku v TRP ptrht podle zadaneho klice key.  Pokud je
** dany prvek nalezen, vraci se ukazatel na dany prvek. Pokud prvek nalezen neni,
** vraci se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
	if(ptrht) {
		int hash = hashCode(key);
		tHTItem *tmpPtr = (*ptrht)[hash];

		if(tmpPtr) {
			do {
				if(!strcmp(key, tmpPtr->key))
					return tmpPtr;
				tmpPtr = tmpPtr->ptrnext;
			} while(tmpPtr);
		}
	}
	return NULL;
}

/*
** TRP s explicitne zretezenymi synonymy.
** Tato procedura vklada do tabulky ptrht polozku s klicem key a s daty
** data. Protoze jde o vyhledavaci tabulku, nemuze byt prvek se stejnym
** klicem ulozen v tabulce vice nez jedenkrat.  Pokud se vklada prvek,
** jehoz klic se jiz v tabulce nachazi, aktualizujte jeho datovou cast.
**
** Vyuzijte drive vytvorenou funkci htSearch.  Pri vkladani noveho
** prvku do seznamu synonym pouzijte co nejefektivnejsi zpusob,
** tedy provedte.vlozeni prvku na zacatek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {
	if(ptrht) {
		int hash = hashCode(key);
		tHTItem *tmpPtr = NULL;

		if((*ptrht)[hash]) {		//Dany hash uz neco obsahuje
			if((tmpPtr = htSearch(ptrht, key))) {		//Tabulka dany key uz obsahuje
				tmpPtr->data = data;
			}
			else {
				if((tmpPtr = malloc(sizeof(tHTItem))) == NULL) {
					//fprintf(stderr, "ERROR\n");
					return;
				}
				tmpPtr->data = data;
				tmpPtr->key = key;
				tmpPtr->ptrnext = (*ptrht)[hash];
				(*ptrht)[hash] = tmpPtr;
			}
		}
		else {
			if((tmpPtr = malloc(sizeof(tHTItem))) == NULL) {
				//fprintf(stderr, "ERROR\n");
				return;
			}
			tmpPtr->data = data;
			tmpPtr->key = key;
			tmpPtr->ptrnext = NULL;
			(*ptrht)[hash] = tmpPtr;
		}
	}
}

/*
** TRP s explicitne zretezenymi synonymy.
** Tato funkce zjistuje hodnotu datove casti polozky zadane klicem.
** Pokud je polozka nalezena, vraci funkce ukazatel na polozku
** Pokud polozka nalezena nebyla, vraci se funkcni hodnota NULL
**
** Vyuzijte drive vytvorenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {

	tHTItem *tmpPtr = NULL;

	if(ptrht) {
		tmpPtr = htSearch(ptrht, key);

		if(tmpPtr)
			return &tmpPtr->data;
	}

	return NULL;
}

/*
** TRP s explicitne zretezenymi synonymy.
** Tato procedura vyjme polozku s klicem key z tabulky
** ptrht.  Uvolnenou polozku korektne zruste.  Pokud polozka s uvedenym
** klicem neexistuje, delejte, jako kdyby se nic nestalo (tj. nedelejte
** nic).
**
** V tomto pripade NEVYUZIVEJTE drive vytvorenou funkci HTSearch.
*/
void htDelete ( tHTable* ptrht, tKey key ) {
	if(ptrht) {
		int hash = hashCode(key);
		tHTItem *tmpPrevPtr = NULL;
		tHTItem *tmpNextPtr = (*ptrht)[hash];

		if(tmpNextPtr) {
			do {		//Hledam
				if(!strcmp(key, tmpNextPtr->key)) {		//Mazu
					if(tmpNextPtr == (*ptrht)[hash]) {
						(*ptrht)[hash] = tmpNextPtr->ptrnext;
						free(tmpNextPtr);
						return;
					}
					else if(tmpPrevPtr) {
						tmpPrevPtr->ptrnext = tmpNextPtr->ptrnext;
						free(tmpNextPtr);
						return;
					}
					else {		//Nema predchudce
						free(tmpNextPtr);
						(*ptrht)[hash] = NULL;
						return;
					}
				}
				tmpPrevPtr = tmpNextPtr;
				tmpNextPtr = tmpNextPtr->ptrnext;
			} while(tmpNextPtr);
		}
	}
}

/* TRP s explicitne zretezenymi synonymy.
** Tato procedura zrusi vsechny polozky tabulky, korektne uvolni prostor,
** ktery tyto polozky zabiraly, a uvede tabulku do pocatecniho stavu.
*/
void htClearAll ( tHTable* ptrht ) {
	tHTItem *tmpPtr;
	tHTItem *tmpNextPtr;
	unsigned int i = 0;

	if(ptrht) {
		while(i < MAX_HTSIZE) {
			tmpNextPtr = (*ptrht)[i];
			while(tmpNextPtr) {
				tmpPtr = tmpNextPtr;
				tmpNextPtr = tmpPtr->ptrnext;

				free(tmpPtr);
			}

			(*ptrht)[i] = NULL;
			i++;
		}
	}
}
