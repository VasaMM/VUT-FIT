
/* ******************************* c202.c *********************************** */
/*  Predmet: Algoritmy (IAL) - FIT VUT v Brne                                 */
/*  Ukol: c202 - Zasobnik znaku v poli                                        */
/*  Referencni implementace: Petr Prikryl, 1994                               */
/*  Vytvoril: Vaclav Topinka, zari 2005                                       */
/*  Upravy: Bohuslav Krena, rijen 2016                                        */
/* ************************************************************************** */
/*
** Implementujte datovy typ zasobnik znaku ve statickem poli. ADT zasobnik je
** reprezentovan zaznamem typu tStack. Staticke pole 'arr' je indexovano
** do maximalni hodnoty STACK_SIZE. Polozka zaznamu 'top' ukazuje na prvek
** na vrcholu zasobniku. Prazdnemu zasobniku odpovida hodnota top == -1,
** zasobniku s jednim prvkem hodnota 0, atd. Presnou definici typu neleznete
** v hlavickovem souboru c202.h.
**
** Implementujte nasledujici funkce:
**
**    stackInit .... inicializace zasobniku
**    stackEmpty ... test na prazdnost zasobniku
**    stackFull .... test na zaplnenost zasobniku
**    stackTop ..... precte hodnotu z vrcholu zasobniku
**    stackPop ..... odstrani prvek z vrcholu zasobniku
**    stackPush .... vlozi prvku do zasobniku
**
** Sve reseni ucelne komentujte!
**
** Terminologicka poznamka: Jazyk C nepouziva pojem procedura.
** Proto zde pouzivame pojem funkce i pro operace, ktere by byly
** v algoritmickem jazyce Pascalovskeho typu implemenovany jako
** procedury (v jazyce C proceduram odpovidaji funkce vracejici typ void).
**
**/

#include "c202.h"

int STACK_SIZE = MAX_STACK;
int solved;

void stackError ( int error_code ){
/*   ----------
** Vytiskne upozorneni, ze doslo k chybe pri praci se zasobnikem.
**
** TUTO FUNKCI, PROSIME, NEUPRAVUJTE!
*/
	static const char* SERR_STRINGS[MAX_SERR+1] = {"Unknown error","Stack error: INIT","Stack error: PUSH","Stack error: TOP"};
	if ( error_code <= 0 || error_code > MAX_SERR )
		error_code = 0;
	printf ( "%s\n", SERR_STRINGS[error_code] );
	err_flag = 1;
}

void stackInit ( tStack* s ) {
/*   ---------
** Provede inicializaci zasobniku - nastavi vrchol zasobniku.
** Hodnoty ve statickem poli neupravujte - po inicializaci zasobniku
** jsou nedefinovane.
**
** V pripade, ze funkce dostane jako parametr s == NULL,
** volejte funkci stackError(SERR_INIT). U ostatnich funkci pro zjednoduseni
** predpokladejte, ze tato situace nenastane.
*/
	if(!s) {
		stackError(SERR_INIT);
		return;
	}

	s->top = -1;
}

int stackEmpty ( const tStack* s ) {
/*  ----------
** Vraci nenulovou hodnotu, pokud je zasobnik prazdny, jinak vraci hodnotu 0.
** Funkci implementujte jako jediny prikaz. Vyvarujte se zejmena konstrukce
** typu "if ( true ) b=true else b=false".
*/
	return (s->top == -1);
}

int stackFull ( const tStack* s ) {
/*  ---------
** Vraci nenulovou hodnotu, je-li zasobnik plny, jinak vraci hodnotu 0.
** Dejte si zde pozor na castou programatorskou chybu "o jednicku"
** a dobre se zamyslete, kdy je zasobnik plny, jestlize muze obsahovat
** nejvyse STACK_SIZE prkvu a prvni prvek je vlozen na pozici 0.
**
** Funkci implementujte jako jediny prikaz.
*/
	return (s->top + 1 >= STACK_SIZE);
}

void stackTop ( const tStack* s, char* c ) {
/*   --------
** Vraci znak z vrcholu zasobniku prostrednictvim parametru c.
** Tato operace ale prvek z vrcholu zasobniku neodstranuje.
** Volani operace Top pri prazdnem zasobniku je nekorektni
** a osetrete ho volanim funkce stackError(SERR_TOP).
**
** Pro overeni, zda je zasobnik prazdny, pouzijte drive definovanou
** funkci stackEmpty.
*/
	if(stackEmpty(s)) {
		stackError(SERR_TOP);
		return;
	}

	*c = s->arr[s->top];
}


void stackPop ( tStack* s ) {
/*   --------
** Odstrani prvek z vrcholu zasobniku. Pro overeni, zda je zasobnik prazdny,
** pouzijte drive definovanou funkci stackEmpty.
**
** Vyvolani operace Pop pri prazdnem zasobniku je sice podezrele a muze
** signalizovat chybu v algoritmu, ve kterem je zasobnik pouzit, ale funkci
** pro osetreni chyby zde nevolejte (muzeme zasobnik ponechat prazdny).
** Spise nez volani chyby by se zde hodilo vypsat varovani, coz vsak pro
** jednoduchost nedelame.
**
*/
	if(stackEmpty(s))
		return;

	s->top --;
}


void stackPush ( tStack* s, char c ) {
/*   ---------
** Vlozi znak na vrchol zasobniku. Pokus o vlozeni prvku do plneho zasobniku
** je nekorektni a osetrete ho volanim procedury stackError(SERR_PUSH).
**
** Pro overeni, zda je zasobnik plny, pouzijte drive definovanou
** funkci stackFull.
*/
	if(stackFull(s)) {
		stackError(SERR_PUSH);
		return;
	}

	s->top ++;
	s->arr[s->top] = c;
}
/* Konec c202.c */
