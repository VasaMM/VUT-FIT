
/* c402.c: ********************************************************************}
{* Tema: Nerekurzivni implementace operaci nad BVS
**                                     Implementace: Petr Prikryl, prosinec 1994
**                                           Upravy: Petr Prikryl, listopad 1997
**                                                     Petr Prikryl, kveten 1998
**			  	                        Prevod do jazyka C: Martin Tucek, srpen 2005
**                                         Upravy: Bohuslav Krena, listopad 2009
**                                         Upravy: Karel Masarik, rijen 2013
**                                         Upravy: Radek Hranicky, rijen 2014
**                                         Upravy: Radek Hranicky, listopad 2015
**                                         Upravy: Radek Hranicky, rijen 2016
**
** S vyuzitim dynamickeho pridelovani pameti, implementujte NEREKURZIVNE
** nasledujici operace nad binarnim vyhledavacim stromem (predpona BT znamena
** Binary Tree a je u identifikatoru uvedena kvuli mozne kolizi s ostatnimi
** priklady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivni vlozeni noveho uzlu do stromu
**     BTPreorder ...... nerekurzivni pruchod typu pre-order
**     BTInorder ....... nerekurzivni pruchod typu in-order
**     BTPostorder ..... nerekurzivni pruchod typu post-order
**     BTDisposeTree ... zrus vsechny uzly stromu
**
** U vsech funkci, ktere vyuzivaji nektery z pruchodu stromem, implementujte
** pomocnou funkci pro nalezeni nejlevejsiho uzlu v podstromu.
**
** Presne definice typu naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na nej je typu tBTNodePtr. Jeden uzel obsahuje polozku int Cont,
** ktera soucasne slouzi jako uzitecny obsah i jako vyhledavaci klic
** a ukazatele na levy a pravy podstrom (LPtr a RPtr).
**
** Priklad slouzi zejmena k procviceni nerekurzivnich zapisu algoritmu
** nad stromy. Nez zacnete tento priklad resit, prostudujte si dukladne
** principy prevodu rekurzivnich algoritmu na nerekurzivni. Programovani
** je predevsim inzenyrska disciplina, kde opetne objevovani Ameriky nema
** misto. Pokud se Vam zda, ze by neco slo zapsat optimalneji, promyslete
** si vsechny detaily Vaseho reseni. Povsimnete si typickeho umisteni akci
** pro ruzne typy pruchodu. Zamyslete se nad modifikaci resenych algoritmu
** napriklad pro vypocet poctu uzlu stromu, poctu listu stromu, vysky stromu
** nebo pro vytvoreni zrcadloveho obrazu stromu (pouze poprehazovani ukazatelu
** bez vytvareni novych uzlu a ruseni starych).
**
** Pri pruchodech stromem pouzijte ke zpracovani uzlu funkci BTWorkOut().
** Pro zjednoduseni prace mate predem pripraveny zasobniky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro praci
** s pomocnymi zasobniky neupravujte
** Pozor! Je treba spravne rozlisovat, kdy pouzit dereferencni operator *
** (typicky pri modifikaci) a kdy budeme pracovat pouze se samotnym ukazatelem
** (napr. pri vyhledavani). V tomto prikladu vam napovi prototypy funkci.
** Pokud pracujeme s ukazatelem na ukazatel, pouzijeme dereferenci.
**/

#include "c402.h"
int solved;

void BTWorkOut (tBTNodePtr Ptr)		{
/*   ---------
** Pomocna funkce, kterou budete volat pri pruchodech stromem pro zpracovani
** uzlu urceneho ukazatelem Ptr. Tuto funkci neupravujte.
**/

	if (Ptr==NULL)
    printf("Chyba: Funkce BTWorkOut byla volana s NULL argumentem!\n");
  else
    printf("Vypis hodnoty daneho uzlu> %d\n",Ptr->Cont);
}

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zasobnik hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP (tStackP *S)
/*   ------
** Inicializace zasobniku.
**/
{
	S->top = 0;
}

void SPushP (tStackP *S, tBTNodePtr ptr)
/*   ------
** Vlozi hodnotu na vrchol zasobniku.
**/
{
                 /* Pri implementaci v poli muze dojit k preteceni zasobniku. */
  if (S->top==MAXSTACK)
    printf("Chyba: Doslo k preteceni zasobniku s ukazateli!\n");
  else {
		S->top++;
		S->a[S->top]=ptr;
	}
}

tBTNodePtr STopPopP (tStackP *S)
/*         --------
** Odstrani prvek z vrcholu zasobniku a soucasne vrati jeho hodnotu.
**/
{
                            /* Operace nad prazdnym zasobnikem zpusobi chybu. */
	if (S->top==0)  {
		printf("Chyba: Doslo k podteceni zasobniku s ukazateli!\n");
		return(NULL);
	}
	else {
		return (S->a[S->top--]);
	}
}

bool SEmptyP (tStackP *S)
/*   -------
** Je-li zasobnik prazdny, vrati hodnotu true.
**/
{
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zasobnik hotnot typu bool. Tyto funkce neupravujte.
*/

void SInitB (tStackB *S) {
/*   ------
** Inicializace zasobniku.
**/

	S->top = 0;
}

void SPushB (tStackB *S,bool val) {
/*   ------
** Vlozi hodnotu na vrchol zasobniku.
**/
                 /* Pri implementaci v poli muze dojit k preteceni zasobniku. */
	if (S->top==MAXSTACK)
		printf("Chyba: Doslo k preteceni zasobniku pro boolean!\n");
	else {
		S->top++;
		S->a[S->top]=val;
	}
}

bool STopPopB (tStackB *S) {
/*   --------
** Odstrani prvek z vrcholu zasobniku a soucasne vrati jeho hodnotu.
**/
                            /* Operace nad prazdnym zasobnikem zpusobi chybu. */
	if (S->top==0) {
		printf("Chyba: Doslo k podteceni zasobniku pro boolean!\n");
		return(NULL);
	}
	else {
		return(S->a[S->top--]);
	}
}

bool SEmptyB (tStackB *S) {
/*   -------
** Je-li zasobnik prazdny, vrati hodnotu true.
**/
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Nasleduje jadro domaci ulohy - funkce, ktere mate implementovat.
*/

void BTInit (tBTNodePtr *RootPtr)	{
/*   ------
** Provede inicializaci binarniho vyhledavaciho stromu.
**
** Inicializaci smi programator volat pouze pred prvnim pouzitim binarniho
** stromu, protoze neuvolnuje uzly neprazdneho stromu (a ani to delat nemuze,
** protoze pred inicializaci jsou hodnoty nedefinovane, tedy libovolne).
** Ke zruseni binarniho stromu slouzi procedura BTDisposeTree.
**
** Vsimnete si, ze zde se poprve v hlavicce objevuje typ ukazatel na ukazatel,
** proto je treba pri praci s RootPtr pouzit dereferencni operator *.
**/
	*RootPtr = NULL;
}

void BTInsert (tBTNodePtr *RootPtr, int Content) {
/*   --------
** Vlozi do stromu novy uzel s hodnotou Content.
**
** Z pohledu vkladani chapejte vytvareny strom jako binarni vyhledavaci strom,
** kde uzly s hodnotou mensi nez ma otec lezi v levem podstromu a uzly vetsi
** lezi vpravo. Pokud vkladany uzel jiz existuje, neprovadi se nic (dana hodnota
** se ve stromu muze vyskytnout nejvyse jednou). Pokud se vytvari novy uzel,
** vznika vzdy jako list stromu. Funkci implementujte nerekurzivne.
**/
	tBTNodePtr tmpPtr   = NULL;
	tBTNodePtr *prevPtr = RootPtr;


	while(*prevPtr) {		// Pokud je RootPtr prazdny, while se preskoci
		// Hodnota je mensi -> jdu vlevo
		if(Content < (*prevPtr)->Cont) {
			prevPtr = &(*prevPtr)->LPtr;
		}
		// Hodnota je vetsi -> jdu vpravo
		else if(Content > (*prevPtr)->Cont) {
			prevPtr = &(*prevPtr)->RPtr;
		}
		// Nasel jsem hodnotu -> koncim
		else
			return;
	}

	//Jsem na konci -> vytvorim list
	if((tmpPtr = malloc(sizeof(**RootPtr))) == NULL) {
		fprintf(stderr, "ERROR: Allocation of memory was unsuccessful!\n");
		return;
	}
	*prevPtr = tmpPtr;
	tmpPtr->Cont = Content;
	tmpPtr->LPtr = tmpPtr->RPtr = NULL;
}

/*                                  PREORDER                                  */

void Leftmost_Preorder (tBTNodePtr ptr, tStackP *Stack) {
/*   -----------------
** Jde po leve vetvi podstromu, dokud nenarazi na jeho nejlevejsi uzel.
**
** Pri pruchodu Preorder navstivene uzly zpracujeme volanim funkce BTWorkOut()
** a ukazatele na ne si ulozime do zasobniku.
**/
	while(ptr) {
		BTWorkOut(ptr);
		SPushP(Stack, ptr);
		ptr = ptr->LPtr;
	}
}

void BTPreorder (tBTNodePtr RootPtr) {
/*   ----------
** Pruchod stromem typu preorder implementovany nerekurzivne s vyuzitim funkce
** Leftmost_Preorder a zasobniku ukazatelu. Zpracovani jednoho uzlu stromu
** realizujte jako volani funkce BTWorkOut().
**/
	// Prazdny strom
	if(!RootPtr)
		return;

	// Inicializace zasobniku
	tStackP *stack = NULL;
	if((stack = malloc(sizeof(*stack))) == NULL) {
		fprintf(stderr, "ERROR: Allocation of memory was unsuccessful!\n");
		return;
	}
	SInitP(stack);
	if(!SEmptyP(stack)) {
		fprintf(stderr, "ERROR: Some problem with stack!\n");
		return;
	}

	tBTNodePtr tmpPtr = RootPtr;
	while(1) {
		// Jdu vlevo
		Leftmost_Preorder(tmpPtr, stack);
		// Jsem na konci
		if(SEmptyP(stack)) {
			break;
		}
		// Jdu vpravo
		tmpPtr = STopPopP(stack);
		if(tmpPtr->RPtr) {
			tmpPtr = tmpPtr->RPtr;
			continue;
		}
		// Jsem na konci
		if(SEmptyP(stack)) {
			break;
		}
		tmpPtr = STopPopP(stack);
		// Dosel jsem do korene stromu
		if(tmpPtr == RootPtr) {
			tmpPtr = RootPtr->RPtr;
			continue;
		}
		// Jdu vpravo
		tmpPtr = tmpPtr->RPtr;
	}
	// Uklizim po sobe
	free(stack);
}


/*                                  INORDER                                   */

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)		{
/*   ----------------
** Jde po leve vetvi podstromu, dokud nenarazi na jeho nejlevejsi uzel.
**
** Pri pruchodu Inorder ukladame ukazatele na vsechny navstivene uzly do
** zasobniku.
**/
	while(ptr) {
		SPushP(Stack, ptr);
		ptr = ptr->LPtr;
	}
}

void BTInorder (tBTNodePtr RootPtr)	{
/*   ---------
** Pruchod stromem typu inorder implementovany nerekurzivne s vyuzitim funkce
** Leftmost_Inorder a zasobniku ukazatelu. Zpracovani jednoho uzlu stromu
** realizujte jako volani funkce BTWorkOut().
**/
	// Prazdny strom
	if(!RootPtr)
		return;

	// Inicializace zasobniku
	tStackP *stack = NULL;
	if((stack = malloc(sizeof(*stack))) == NULL) {
		fprintf(stderr, "ERROR: Allocation of memory was unsuccessful!\n");
		return;
	}
	SInitP(stack);
	if(!SEmptyP(stack)) {
		fprintf(stderr, "ERROR: Some problem with stack!\n");
		return;
	}

	tBTNodePtr tmpPtr = RootPtr;
	while(1) {
		// Jdu vlevo
		Leftmost_Inorder(tmpPtr, stack);
		// Jsem na konci
		if(SEmptyP(stack)) {
			break;
		}
		// Jdu vpravo
		tmpPtr = STopPopP(stack);
		BTWorkOut(tmpPtr);
		if(tmpPtr->RPtr) {
			tmpPtr = tmpPtr->RPtr;
			continue;
		}
		// Jsem na konci
		if(SEmptyP(stack)) {
			break;
		}
		tmpPtr = STopPopP(stack);
		// Jdu vpravo
		BTWorkOut(tmpPtr);
		if(tmpPtr == RootPtr) {
			tmpPtr = RootPtr->RPtr;
			continue;
		}
		tmpPtr = tmpPtr->RPtr;
	}
	// Uklizim po sobe
	free(stack);
}

/*                                 POSTORDER                                  */

void Leftmost_Postorder (tBTNodePtr ptr, tStackP *StackP, tStackB *StackB) {
/*           --------
** Jde po leve vetvi podstromu, dokud nenarazi na jeho nejlevejsi uzel.
**
** Pri pruchodu Postorder ukladame ukazatele na navstivene uzly do zasobniku
** a soucasne do zasobniku bool hodnot ukladame informaci, zda byl uzel
** navstiven poprve a ze se tedy jeste nema zpracovavat.
**/
	while(ptr) {
		SPushP(StackP, ptr);
		SPushB(StackB, FALSE);
		ptr = ptr->LPtr;
	}
}

void BTPostorder (tBTNodePtr RootPtr)	{
/*           -----------
** Pruchod stromem typu postorder implementovany nerekurzivne s vyuzitim funkce
** Leftmost_Postorder, zasobniku ukazatelu a zasobniku hotdnot typu bool.
** Zpracovani jednoho uzlu stromu realizujte jako volani funkce BTWorkOut().
**/
	// Prazdny strom
	if(!RootPtr)
		return;

	// Inicializace zasobniku
	tStackP *stackP = NULL;
	         stackP = malloc(sizeof(*stackP));
	tStackB *stackB = NULL;
	         stackB = malloc(sizeof(*stackB));

	if(!stackP || !stackB) {
		fprintf(stderr, "ERROR: Allocation of memory was unsuccessful!\n");
		return;
	}
	SInitP(stackP);
	SInitB(stackB);
	if(!SEmptyP(stackP) || !SEmptyB(stackB)) {
		fprintf(stderr, "ERROR: Some problem with stack!\n");
		return;
	}

	tBTNodePtr tmpPtr  = RootPtr;
	bool       tmpBool = false;
	while(1) {
		// Jdu vlevo
		if(!tmpBool)
			Leftmost_Postorder(tmpPtr, stackP, stackB);
		// Jsem na konci
		if(SEmptyP(stackP) || SEmptyB(stackB)) {
			break;
		}

		tmpPtr  = STopPopP(stackP);
		tmpBool = STopPopB(stackB);
		if(tmpPtr->RPtr && !tmpBool) {
			SPushP(stackP, tmpPtr);
			SPushB(stackB, TRUE);
			tmpPtr = tmpPtr->RPtr;
			continue;
		}
		else {
			BTWorkOut(tmpPtr);
		}

		if(SEmptyP(stackP) || SEmptyB(stackB)) {
			break;
		}

		tmpPtr = STopPopP(stackP);
		tmpBool = STopPopB(stackB);

		// Kontrola na koren stromu
		if(tmpPtr == RootPtr) {
			if(!SEmptyB(stackB) || !SEmptyP(stackP)) {
				fprintf(stderr, "ERROR: Some problem with stack!\n");
				return;
			}

			SPushP(stackP, RootPtr);
			SPushB(stackB, TRUE);
			tmpPtr = RootPtr->RPtr;
			continue;
		}


		if(tmpBool) {
			BTWorkOut(tmpPtr);

			tmpPtr = STopPopP(stackP);
			tmpBool = STopPopB(stackB);

			SPushP(stackP, tmpPtr);
			SPushB(stackB, TRUE);
		}
		else {
			SPushP(stackP, tmpPtr);
			SPushB(stackB, TRUE);
		}
		tmpPtr = tmpPtr->RPtr;
	}
	// Uklizim po sobe
	free(stackP);
	free(stackB);
}


void BTDisposeTree (tBTNodePtr *RootPtr) {
/*   -------------
** Zrusi vsechny uzly stromu a korektne uvolni jimi zabranou pamet.
**
** Funkci implementujte nerekurzivne s vyuzitim zasobniku ukazatelu.
**/
	// Prazdny strom
	if(!*RootPtr)
		return;

	// Inicializace zasobniku
	tStackP *stackP = NULL;
	         stackP = malloc(sizeof(*stackP));
	tStackB *stackB = NULL;
	         stackB = malloc(sizeof(*stackB));

	if(!stackP || !stackB) {
		fprintf(stderr, "ERROR: Allocation of memory was unsuccessful!\n");
		return;
	}
	SInitP(stackP);
	SInitB(stackB);
	if(!SEmptyP(stackP) || !SEmptyB(stackB)) {
		fprintf(stderr, "ERROR: Some problem with stack!\n");
		return;
	}

	tBTNodePtr tmpPtr  = *RootPtr;
	bool       tmpBool = false;
	while(1) {
		// Jdu vlevo
		if(!tmpBool) {
			while(tmpPtr) {
				SPushP(stackP, tmpPtr);
				SPushB(stackB, FALSE);
				tmpPtr = tmpPtr->LPtr;
			}
		}
		// Jsem na konci
		if(SEmptyP(stackP) || SEmptyB(stackB)) {
			free(*RootPtr);
			*RootPtr = NULL;
			break;
		}

		tmpPtr  = STopPopP(stackP);
		tmpBool = STopPopB(stackB);
		if(tmpPtr->RPtr && !tmpBool) {
			SPushP(stackP, tmpPtr);
			SPushB(stackB, TRUE);
			tmpPtr = tmpPtr->RPtr;
			continue;
		}
		else {
			free(tmpPtr);
		}

		if(SEmptyP(stackP) || SEmptyB(stackB)) {
			//free(*RootPtr);
			*RootPtr = NULL;
			break;
		}

		tmpPtr  = STopPopP(stackP);
		tmpBool = STopPopB(stackB);

		// Kontrola na koren stromu
		if(tmpPtr == *RootPtr) {
			if(!SEmptyB(stackB) || !SEmptyP(stackP)) {
				fprintf(stderr, "ERROR: Some problem with stack!\n");
				return;
			}

			SPushP(stackP, *RootPtr);
			SPushB(stackB, TRUE);
			tmpPtr = (*RootPtr)->RPtr;
			continue;
		}


		if(tmpBool) {
			free(tmpPtr);

			tmpPtr = STopPopP(stackP);
			tmpBool = STopPopB(stackB);

			SPushP(stackP, tmpPtr);
			SPushB(stackB, TRUE);
		}
		else {
			SPushP(stackP, tmpPtr);
			SPushB(stackB, TRUE);
		}
		tmpPtr = tmpPtr->RPtr;
	}
	// Uklizim po sobe
	free(stackP);
	free(stackB);
}

/* konec c402.c */

