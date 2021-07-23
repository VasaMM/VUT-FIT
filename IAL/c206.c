
/* c206.c **********************************************************}
{* Tema: Dvousmerne vazany linearni seznam
**
**                   Navrh a referencni implementace: Bohuslav Krena, rijen 2001
**                            Prepracovane do jazyka C: Martin Tucek, rijen 2004
**                                            Upravy: Bohuslav Krena, rijen 2016
**
** Implementujte abstraktni datovy typ dvousmerne vazany linearni seznam.
** Uzitecnym obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datova abstrakce reprezentovan promennou
** typu tDLList (DL znamena Double-Linked a slouzi pro odliseni
** jmen konstant, typu a funkci od jmen u jednosmerne vazaneho linearniho
** seznamu). Definici konstant a typu naleznete v hlavickovem souboru c206.h.
**
** Vasim ukolem je implementovat nasledujici operace, ktere spolu
** s vyse uvedenou datovou casti abstrakce tvori abstraktni datovy typ
** obousmerne vazany linearni seznam:
**
**      DLInitList ...... inicializace seznamu pred prvnim pouzitim,
**      DLDisposeList ... zruseni vsech prvku seznamu,
**      DLInsertFirst ... vlozeni prvku na zacatek seznamu,
**      DLInsertLast .... vlozeni prvku na konec seznamu,
**      DLFirst ......... nastaveni aktivity na prvni prvek,
**      DLLast .......... nastaveni aktivity na posledni prvek,
**      DLCopyFirst ..... vraci hodnotu prvniho prvku,
**      DLCopyLast ...... vraci hodnotu posledniho prvku,
**      DLDeleteFirst ... zrusi prvni prvek seznamu,
**      DLDeleteLast .... zrusi posledni prvek seznamu,
**      DLPostDelete .... rusi prvek za aktivnim prvkem,
**      DLPreDelete ..... rusi prvek pred aktivnim prvkem,
**      DLPostInsert .... vlozi novy prvek za aktivni prvek seznamu,
**      DLPreInsert ..... vlozi novy prvek pred aktivni prvek seznamu,
**      DLCopy .......... vraci hodnotu aktivniho prvku,
**      DLActualize ..... prepise obsah aktivniho prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dalsi prvek seznamu,
**      DLPred .......... posune aktivitu na predchozi prvek seznamu,
**      DLActive ........ zjistuje aktivitu seznamu.
**
** Pri implementaci jednotlivych funkci nevolejte zadnou z funkci
** implementovanych v ramci tohoto prikladu, neni-li u funkce
** explicitne uvedeno neco jineho.
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

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozorneni na to, ze doslo k chybe.
** Tato funkce bude volana z nekterych dale implementovanych operaci.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globalni promenna -- priznak osetreni chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L pred jeho prvnim pouzitim (tzn. zadna
** z nasledujicich funkci nebude volana nad neinicializovanym seznamem).
** Tato inicializace se nikdy nebude provadet nad jiz inicializovanym
** seznamem, a proto tuto moznost neosetrujte. Vzdy predpokladejte,
** ze neinicializovane promenne maji nedefinovanou hodnotu.
**/
	L->Act = NULL;
	L->First = NULL;
	L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zrusi vsechny prvky seznamu L a uvede seznam do stavu, v jakem
** se nachazel po inicializaci. Rusene prvky seznamu budou korektne
** uvolneny volanim operace free.
**/
	//Kontrola na to, zda neni seznam prazdny
	if(!L->First)
		return;

	struct tDLElem *remove;
	struct tDLElem *next = L->First;

	do {
		remove = next;
		next = remove->rptr;
		free(remove);
	} while(next);

	L->Act = NULL;
	L->First = NULL;
	L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlozi novy prvek na zacatek seznamu L.
** V pripade, ze neni dostatek pameti pro novy prvek pri operaci malloc,
** vola funkci DLError().
**/
	struct tDLElem *tmpPtr = NULL;

	if((tmpPtr = malloc(sizeof(struct tDLElem))) == NULL) {
		DLError();
		return;
	}

	//No empty list
	if(L->First) {
		tmpPtr->rptr = L->First;
		L->First->lptr = tmpPtr;
	}
	else {
		tmpPtr->rptr = NULL;
		L->Last = tmpPtr;
	}

	tmpPtr->lptr = NULL;

	L->First = tmpPtr;
	tmpPtr->data = val;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlozi novy prvek na konec seznamu L (symetricka operace k DLInsertFirst).
** V pripade, ze neni dostatek pameti pro novy prvek pri operaci malloc,
** vola funkci DLError().
**/
	struct tDLElem *tmpPtr = NULL;

	if((tmpPtr = malloc(sizeof(struct tDLElem))) == NULL) {
		DLError();
		return;
	}

	//No empty list
	if(L->Last) {
		tmpPtr->lptr = L->Last;
		L->Last->rptr = tmpPtr;
	}
	else {
		tmpPtr->lptr = NULL;
		L->First = tmpPtr;
	}

	tmpPtr->rptr = NULL;

	L->Last = tmpPtr;
	tmpPtr->data = val;
}

void DLFirst (tDLList *L) {
/*
** Nastavi aktivitu na prvni prvek seznamu L.
** Funkci implementujte jako jediny prikaz (nepocitame-li return),
** aniz byste testovali, zda je seznam L prazdny.
**/
	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastavi aktivitu na posledni prvek seznamu L.
** Funkci implementujte jako jediny prikaz (nepocitame-li return),
** aniz byste testovali, zda je seznam L prazdny.
**/
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostrednictvim parametru val vrati hodnotu prvniho prvku seznamu L.
** Pokud je seznam L prazdny, vola funkci DLError().
**/
	if(!L->First) {
		DLError();
		return;
	}

	*val = L->First->data;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostrednictvim parametru val vrati hodnotu posledniho prvku seznamu L.
** Pokud je seznam L prazdny, vola funkci DLError().
**/
	if(!L->Last) {
		DLError();
		return;
	}

	*val = L->Last->data;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zrusi prvni prvek seznamu L. Pokud byl prvni prvek aktivni, aktivita
** se ztraci. Pokud byl seznam L prazdny, nic se nedeje.
**/
	if(!L->First)
		return;

	if(L->First == L->Last) {
		free(L->First);
		L->First = L->Last = L->Act = NULL;
		return;
	}

	struct tDLElem *tmpPtr = L->First->rptr;

	if(L->Act == L->First) {
		L->Act = NULL;
	}

	free(L->First);
	L->First = tmpPtr;
	if(L->First)
		L->First->lptr = NULL;
}

void DLDeleteLast (tDLList *L) {
/*
** Zrusi posledni prvek seznamu L. Pokud byl posledni prvek aktivni,
** aktivita seznamu se ztraci. Pokud byl seznam L prazdny, nic se nedeje.
**/
	if(!L->Last)
		return;

	if(L->First == L->Last) {
		free(L->First);
		L->First = L->Last = L->Act = NULL;
		return;
	}

	struct tDLElem *tmpPtr = L->Last->lptr;

	if(L->Act == L->Last) {
		L->Act = NULL;
	}

	free(L->Last);
	L->Last = tmpPtr;
	if(L->Last)
		L->Last->rptr = NULL;
}

void DLPostDelete (tDLList *L) {
/*
** Zrusi prvek seznamu L za aktivnim prvkem.
** Pokud je seznam L neaktivni nebo pokud je aktivni prvek
** poslednim prvkem seznamu, nic se nedeje.
**/
	if(!L->Act || !L->Act->rptr)
		return;

	struct tDLElem *tmpPtr = L->Act->rptr->rptr;

	free(L->Act->rptr);
	L->Act->rptr = tmpPtr;
	if(tmpPtr)
		tmpPtr->lptr = L->Act;
	else
		L->Last = L->Act;
}

void DLPreDelete (tDLList *L) {
/*
** Zrusi prvek pred aktivnim prvkem seznamu L .
** Pokud je seznam L neaktivni nebo pokud je aktivni prvek
** prvnim prvkem seznamu, nic se nedeje.
**/
	if(!L->Act || !L->Act->lptr)
		return;

	struct tDLElem *tmpPtr = L->Act->lptr->lptr;

	free(L->Act->lptr);
	L->Act->lptr = tmpPtr;
	if(tmpPtr)
		tmpPtr->rptr = L->Act;
	else
		L->First = L->Act;
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlozi prvek za aktivni prvek seznamu L.
** Pokud nebyl seznam L aktivni, nic se nedeje.
** V pripade, ze neni dostatek pameti pro novy prvek pri operaci malloc,
** vola funkci DLError().
**/
	if(L->Act) {
		struct tDLElem *tmpPtr = NULL;

		if((tmpPtr = malloc(sizeof(struct tDLElem))) == NULL) {
			DLError();
			return;
		}

		else if(L->Act == L->Last)
			L->Last = tmpPtr;

		tmpPtr->data = val;
		tmpPtr->rptr = L->Act->rptr;
		tmpPtr->lptr = L->Act;

		if(L->Act->rptr)
			L->Act->rptr->lptr = tmpPtr;
		L->Act->rptr = tmpPtr;
	}
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlozi prvek pred aktivni prvek seznamu L.
** Pokud nebyl seznam L aktivni, nic se nedeje.
** V pripade, ze neni dostatek pameti pro novy prvek pri operaci malloc,
** vola funkci DLError().
**/
	if(L->Act) {
		struct tDLElem *tmpPtr = NULL;

		if((tmpPtr = malloc(sizeof(struct tDLElem))) == NULL) {
			DLError();
			return;
		}

		if(L->Act == L->First)
			L->First = tmpPtr;

		tmpPtr->data = val;
		tmpPtr->lptr = L->Act->lptr;
		tmpPtr->rptr = L->Act;

		if(L->Act->lptr)
			L->Act->lptr->rptr = tmpPtr;
		L->Act->lptr = tmpPtr;
	}
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostrednictvim parametru val vrati hodnotu aktivniho prvku seznamu L.
** Pokud seznam L neni aktivni, vola funkci DLError ().
**/
	if(!L->Act) {
		DLError();
		return;
	}

	*val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Prepise obsah aktivniho prvku seznamu L.
** Pokud seznam L neni aktivni, nedela nic.
**/
	if(L->Act)
		L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na nasledujici prvek seznamu L.
** Neni-li seznam aktivni, nedela nic.
** Vsimnete si, ze pri aktivite na poslednim prvku se seznam stane neaktivnim.
**/
	if(L->Act)
		L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na predchozi prvek seznamu L.
** Neni-li seznam aktivni, nedela nic.
** Vsimnete si, ze pri aktivite na prvnim prvku se seznam stane neaktivnim.
**/
	if(L->Act)
		L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivni, vraci nenulovou hodnotu, jinak vraci 0.
** Funkci je vhodne implementovat jednim prikazem return.
**/
	return (!!L->Act);
}

/* Konec c206.c*/
