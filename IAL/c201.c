
/* c201.c *********************************************************************}
{* Tema: Jednosmerny linearni seznam
**
**                     Navrh a referencni implementace: Petr Prikryl, rijen 1994
**                                          Upravy: Andrea Nemcova listopad 1996
**                                                   Petr Prikryl, listopad 1997
**                                Prepracovane zadani: Petr Prikryl, brezen 1998
**                                  Prepis do jazyka C: Martin Tucek, rijen 2004
**                                            Upravy: Bohuslav Krena, rijen 2016
**
** Implementujte abstraktni datovy typ jednosmerny linearni seznam.
** Uzitecnym obsahem prvku seznamu je cele cislo typu int.
** Seznam bude jako datova abstrakce reprezentovan promennou typu tList.
** Definici konstant a typu naleznete v hlavickovem souboru c201.h.
**
** Vasim ukolem je implementovat nasledujici operace, ktere spolu s vyse
** uvedenou datovou casti abstrakce tvori abstraktni datovy typ tList:
**
**      InitList ...... inicializace seznamu pred prvnim pouzitim,
**      DisposeList ... zruseni vsech prvku seznamu,
**      InsertFirst ... vlozeni prvku na zacatek seznamu,
**      First ......... nastaveni aktivity na prvni prvek,
**      CopyFirst ..... vraci hodnotu prvniho prvku,
**      DeleteFirst ... zrusi prvni prvek seznamu,
**      PostDelete .... rusi prvek za aktivnim prvkem,
**      PostInsert .... vlozi novy prvek za aktivni prvek seznamu,
**      Copy .......... vraci hodnotu aktivniho prvku,
**      Actualize ..... prepise obsah aktivniho prvku novou hodnotou,
**      Succ .......... posune aktivitu na dalsi prvek seznamu,
**      Active ........ zjistuje aktivitu seznamu.
**
** Pri implementaci funkci nevolejte zadnou z funkci implementovanych v ramci
** tohoto prikladu, neni-li u dane funkce explicitne uvedeno neco jineho.
**
** Nemusite osetrovat situaci, kdy misto legalniho ukazatele na seznam
** preda nekdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodne komentujte!
**
** Terminologicka poznamka: Jazyk C nepouziva pojem procedura.
** Proto zde pouzivame pojem funkce i pro operace, ktere by byly
** v algoritmickem jazyce Pascalovskeho typu implemenovany jako
** procedury (v jazyce C proceduram odpovidaji funkce vracejici typ void).
**/

#include "c201.h"

int solved;
int errflg;

void Error() {
/*
** Vytiskne upozorneni na to, ze doslo k chybe.
** Tato funkce bude volana z nekterych dale implementovanych operaci.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globalni promenna -- priznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L pred jeho prvnim pouzitim (tzn. zadna
** z nasledujicich funkci nebude volana nad neinicializovanym seznamem).
** Tato inicializace se nikdy nebude provadet nad jiz inicializovanym
** seznamem, a proto tuto moznost neosetrujte. Vzdy predpokladejte,
** ze neinicializovane promenne maji nedefinovanou hodnotu.
**/
	L->Act = NULL;
	L->First = NULL;
}

void DisposeList (tList *L) {
/*
** Zrusi vsechny prvky seznamu L a uvede seznam L do stavu, v jakem se nachazel
** po inicializaci. Veskera pamet pouzivana prvky seznamu L bude korektne
** uvolnena volanim operace free.
***/
	//Check empty list
	if(!L->First)
		return;

	struct tElem *remove;
	struct tElem *next = L->First;

	//Clear
	do {
		remove = next;
		next = remove->ptr;
		free(remove);
	} while(next);

	L->Act = NULL;
	L->First = NULL;
}

void InsertFirst (tList *L, int val) {
/*
** Vlozi prvek s hodnotou val na zacatek seznamu L.
** V pripade, ze neni dostatek pameti pro novy prvek pri operaci malloc,
** vola funkci Error().
**/
	struct tElem *tmpPtr = NULL;

	if((tmpPtr = malloc(sizeof(struct tElem))) == NULL) {
		Error();
		return;
	}

	tmpPtr->ptr = L->First;
	L->First = tmpPtr;
	L->First->data = val;
}

void First (tList *L) {
/*
** Nastavi aktivitu seznamu L na jeho prvni prvek.
** Funkci implementujte jako jediny prikaz, aniz byste testovali,
** zda je seznam L prazdny.
**/
	L->Act = L->First;
}

void CopyFirst (tList *L, int *val) {
/*
** Prostrednictvim parametru val vrati hodnotu prvniho prvku seznamu L.
** Pokud je seznam L prazdny, vola funkci Error().
**/
	if(!L->First) {
		Error();
		return;
	}

	*val = L->First->data;
}

void DeleteFirst (tList *L) {
/*
** Zrusi prvni prvek seznamu L a uvolni jim pouzivanou pamet.
** Pokud byl ruseny prvek aktivni, aktivita seznamu se ztraci.
** Pokud byl seznam L prazdny, nic se nedeje.
**/
	if(L->First) {
		struct tElem *tmpPtr = L->First->ptr;

		if(L->Act == L->First) {
			L->Act = NULL;
		}

		free(L->First);
		L->First = tmpPtr;
	}
}

void PostDelete (tList *L) {
/*
** Zrusi prvek seznamu L za aktivnim prvkem a uvolni jim pouzivanou pamet.
** Pokud neni seznam L aktivni nebo pokud je aktivni posledni prvek seznamu L,
** nic se nedeje.
**/
	if(!L->Act || !L->Act->ptr)
		return;

	struct tElem *tmpPtr = L->Act->ptr->ptr;

	free(L->Act->ptr);
	L->Act->ptr = tmpPtr;
}

void PostInsert (tList *L, int val) {
/*
** Vlozi prvek s hodnotou val za aktivni prvek seznamu L.
** Pokud nebyl seznam L aktivni, nic se nedeje!
** V pripade, ze neni dostatek pameti pro novy prvek pri operaci malloc,
** zavola funkci Error().
**/
	if(L->Act) {
		struct tElem *tmpPtr = NULL;

		if((tmpPtr = malloc(sizeof(struct tElem))) == NULL) {
			Error();
			return;
		}

		tmpPtr->data = val;
		tmpPtr->ptr = L->Act->ptr;

		L->Act->ptr = tmpPtr;
	}
}

void Copy (tList *L, int *val) {
/*
** Prostrednictvim parametru val vrati hodnotu aktivniho prvku seznamu L.
** Pokud seznam neni aktivni, zavola funkci Error().
**/
	if(!L->Act) {
		Error();
		return;
	}

	*val = L->Act->data;
}

void Actualize (tList *L, int val) {
/*
** Prepise data aktivniho prvku seznamu L hodnotou val.
** Pokud seznam L neni aktivni, nedela nic!
**/
	if(L->Act)
		L->Act->data = val;
}

void Succ (tList *L) {
/*
** Posune aktivitu na nasledujici prvek seznamu L.
** Vsimnete si, ze touto operaci se muze aktivni seznam stat neaktivnim.
** Pokud neni predany seznam L aktivni, nedela funkce nic.
**/
	if(L->Act)
		L->Act = L->Act->ptr;
}

int Active (tList *L) {
/*
** Je-li seznam L aktivni, vraci nenulovou hodnotu, jinak vraci 0.
** Tuto funkci je vhodne implementovat jednim prikazem return.
**/
	return (!!L->Act);
}

/* Konec c201.c */
