/*******************************
 ** Autor: Vaclav Martinka    **
 **    ID: xmarti76           **
 ** Nazev: Proj1              **
 *******************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>   // INT_MAX = 2147483647
#include <stdbool.h>
//#include <time.h>


///Definice globalnich promennych
char vstup[101];    //Definuji si pole pro 100-znakovy string (posledni znak \0) = 1 vstupni slovo
int cislo;
int pocet_nul;

int letopocet[] = {1997, 11, 01};           //Pole pro snadne ulozeni letopoctu, pro zacatek ho zaplnim 1. listopadem , musim deklarovat mimo funkci kvuli pristupu z funkce main!
int format_data[] = {0,0,0,0,0,0,0,0,0,0};  //Testuji, zda se muze jednat o letopocet -> musi byt 1111211211
int poradi_dne = 0;
bool date_wrong_format = false;             //Neexistujici datum

bool nepovoleny_znak = false;

const int rok_min = 1583;
const int rok_max = 5000;


///Chybova hlaseni
void chyba (int x) {
  printf("\nNejspis doslo k nejake chybe!\n   ");

  if (x == 1)
    printf("Pri kontrole prvocisla nebyla splnena zadna podminka.");
  else if (x == 2)
    printf("Pri tisku radku doslo k chybe.\n");

  return;
}


///Otestovani palindromu
char *is_palindrome (int pocet_znaku) {
  int prectenych_znaku = 0;
  int kontrolovat_znaku;

  if (pocet_znaku == 1)                     //Pokud se jedna o jeden znak
    return " (palindrome)";                 //tak je to palindrom
  else                                      //Jinak si vydelim pocet znaku 2 abych vedel, kolik jich musim porovnat
    kontrolovat_znaku = pocet_znaku / 2;    //Muzu si dovolit ignorovat desetinou cast, jelikoz tak ignoruji pouze prostredni znak -> je spolecny pro obe poloviny slova

  do {
    if (vstup[prectenych_znaku] != vstup[pocet_znaku - 1])  //porovnam prvni a posledni pismenko
      return "";                                            //kdyz jsou ruzne, vracim false

    prectenych_znaku ++;
    pocet_znaku --;
    kontrolovat_znaku --;
  } while (kontrolovat_znaku != 0);   //Delam to, dokud nejsou znaky rozdilne, nebo neporovnam vsechny

  return " (palindrome)";             //Pokud dojdu az sem, vracim true
}


///Otestovani prvocisla
char *is_prime (void) {             //Typ je char - vracim rovnou " (prime)" nebo "" nebo " (err)"
  if (cislo < 2)                    //Prvocisla >= 2
    return "";                      //Proto kdyz je mensi, ukoncim zjistovani
  if ((cislo == 2) || (cislo == 3)) //na cisla 2 a 3 tato metoda nefunguje...
    return " (prime)";
  if (cislo % 2 == 0)               //Suda cisla nemuzou byt prvocislem
    return "";                      // -> kontroluji jen licha cisla
  else {
    for (long long int delitel = 3 ; (delitel * delitel) <= cislo ; delitel += 2) { //Testovani tak, ze cislo postupne delim intervalem <3; odmocnina z cisla>
      if (cislo % delitel == 0)     //Pokud narazim na modulo 0 -> neni prvocislo!
        return "";
    }
    return " (prime)";              //Pokud ne -> je to prvocislo
  }

  chyba(1);   //Kdyby cokoli, napisu chybu...
  return "";
}


///Otestovani letopoctu
int is_date (int pocet_znaku) { //Typ je int, vraci 2 pro date a 0 pro word + vytvori datum ve spravnem formatu
  bool prestupny_rok = false;   //Prestupny rok
  int tvar_data[] = {0,0,0};    //Kontrola na spravny tvar - pole
  int i = 0;                    //Promenne pro postupne cteni
  int j = 0;
//  int rok;                      //Promenna pro ulozeni roku v

  do {
    if (format_data[i] == 1)        //Postupne zjistuji pocet cislic mezi oddelovaci
      tvar_data[j] ++;              //Povolene tvary: 4-1/2-1/2 (YYYY-MM-DD)
    else if (format_data[i] == 2)   //                1/2-1/2-4 (DD-MM-YYYY)
      j ++;
    else
      return 0;                     //Kdyby cokoli, vratim "word"

    i ++;
  } while (i != pocet_znaku);


///Prevod prvni pozice = rok / den
  if (tvar_data[0] == 4) {
    if (((letopocet[0] = atoi(&vstup[0])) < rok_min) || (letopocet[0] > rok_max)) {   //Prevedu vstupni data na cisla, ulozim do pole a zkontroluji spravnost (rozsah 1000 - 3000)
		  date_wrong_format = true;
      return 0;
    }
  }
  else if ((tvar_data[0] == 2) || (tvar_data[0] == 1)) {
    if (((letopocet[2] = atoi(&vstup[0])) > 31) || (letopocet[2] == 0)) {       //Rozsah 1 - 31, dalsi kontrola pozdeji
		  date_wrong_format = true;
      return 0;
    }
  }
  else
    return 0;

///Prevod druhe pozice = mesic
  if ((tvar_data[1] == 1) || (tvar_data[1] == 2)) {
    if (((letopocet[1] = atoi(&vstup[tvar_data[0] + 1])) > 12) || (letopocet[1] == 0)) {    //Rozsah 1 - 12
			date_wrong_format = true;
      return 0;
    }
  }
  else
    return 0;

///Prevod treti pozice = den/rok
  if (tvar_data[2] == 4) {
    if (((letopocet[0] = atoi(&vstup[tvar_data[0] + tvar_data[1] + 2])) < rok_min) || (letopocet[0] > rok_max)) {   //Prevedu vstupni data na cisla, ulozim do pole a zkontroluji spravnost (rozsah 1000 - 3000)
      date_wrong_format = true;
      return 0;
    }
  }
  else if ((tvar_data[2] == 2) || (tvar_data[2] == 1)) {
    if (((letopocet[2] = atoi(&vstup[tvar_data[0] + tvar_data[1] + 2])) > 31) || (letopocet[2] == 0)) {       //Rozsah 1 - 31, dalsi kontrola pozdeji
      date_wrong_format = true;
      return 0;
    }
  }
  else
    return 0;



///Kontrola realnosti data
  if ((((letopocet[0] % 4) == 0) && ((letopocet[0] % 100) != 0)) || ((letopocet[0] % 400) == 0))  //Kontrola prestupniho roku (kvuli unoru 29)
    prestupny_rok = true;

  if ((letopocet[1] == 4 || letopocet[1] == 6 || letopocet[1] == 9 || letopocet[1] == 11) && (letopocet[2] == 31)) {     //Kontrola max 30 dnu v mesicich 4, 6, 9 a 11
    date_wrong_format = true;
    return 0;
  }
  if ((letopocet[1] == 2) && (prestupny_rok == false) && (letopocet[2] > 28)) { //Kontrola unoru
    date_wrong_format = true;
    return 0;
  }
  else if ((letopocet[1] == 2) && prestupny_rok && (letopocet[2] > 29)) {
    date_wrong_format = true;
    return 0;
  }

//mktime - nevyuzivam
/*  if (letopocet[0] > 1970 && letopocet[0] < 2038)
  {
  time_str.tm_year  = letopocet[0] - 1900;  //Nastavim rok
  time_str.tm_mon   = letopocet[1] - 1;     //Nastavim mesic
  time_str.tm_mday  = letopocet[2];         //Nastavim den
  time_str.tm_sec   = 1;
  time_str.tm_min   = 0;
  time_str.tm_hour  = 0;
  time_str.tm_isdst = 0;

  mktime(&time_str);                                                  //Spocitam pocet vterin od 1970...
  (void)strftime(poradi_dne, sizeof(poradi_dne), "%u", &time_str);    //Prevedu na poradi
  printf(" -- %i\n", poradi_dne);
  poradi_dne --;
  den_slovne[3] = ' ';                                                //Pripisu mezeru za slovo
  den_slovne[4] = '\0';                                               //Pro jistotu zakoncim retezec...
  }*/

///Spocitam poradi dne v tydnu (https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week)
  int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

  if (letopocet[1] < 3)
    poradi_dne = (letopocet[0] - 1 + (letopocet[0] - 1) / 4 - (letopocet[0] - 1) / 100
                 + (letopocet[0] - 1) / 400 + t[letopocet[1] - 1] + letopocet[2]) % 7;

  else
    poradi_dne = (letopocet[0] + letopocet[0] / 4 - letopocet[0] / 100 + letopocet[0] / 400
                 + t[letopocet[1] - 1] + letopocet[2]) % 7;

  return 1;       //Vracim 1 pro funkcni date
}


///Definice funkce na napsani jednoho radku
int pis_radek (void) {
  bool pouze_cislice = true;  //Kontrola cislic, na zacatku true, pokud narazim na "necislo" tak false
  char *den_slovne[] = {"Sun ", "Mon ", "Tue ", "Wed ", "Thu ", "Fri ", "Sat "};
  int  znak_navic;
  int  prectenych_oddelovacu = 0;
  int  prectenych_znaku = 0;
              pocet_nul = 0;
  bool kontrolovat_nuly = true;

  while (vstup[prectenych_znaku] != '\0') {   //Cteni slova po znacich
    if (iscntrl(vstup[prectenych_znaku])) {   //Jde o ridici znak
      nepovoleny_znak = true;                 //Ukoncit program
      return 0;
    }

    if (!isdigit(vstup[prectenych_znaku]))    //Neni cislo -> word
      pouze_cislice = false;
    else if (prectenych_znaku < 10)           //Je cislo -> zapisu si 1, pouze prvnich 10 cisel!
      format_data[prectenych_znaku] = 1;
    if (!isalnum(vstup[prectenych_znaku]) && prectenych_znaku < 10) {     //Je oddelovac -> zapisu si 2, pouze prvnich 10 cisel!
      format_data[prectenych_znaku] = 2;
      prectenych_oddelovacu ++;
    }
    if (pouze_cislice && kontrolovat_nuly && vstup[prectenych_znaku] == '0')
      pocet_nul ++;
    else
      kontrolovat_nuly = false;

    prectenych_znaku ++;
  }

  ///Pokud splnim podminku, je mozne, ze se jedna o datum, pokud ano -> napisu radek rovnou -> je to jednodussi
  if ((prectenych_oddelovacu == 2 && (prectenych_znaku == 8 || prectenych_znaku == 9 || prectenych_znaku == 10)) && (is_date(prectenych_znaku) == 1)) {
    printf("date: %s%d-%d-%d\n", den_slovne[poradi_dne], letopocet[0], letopocet[1], letopocet[2]);   //Vytisknu radek
    return 1;
  }

  ///Pokud je znaku vice jak 100 - vypisu je
  if (prectenych_znaku == 100 && !isspace(znak_navic = getchar ())) {   //Pokud je znaku 100 a 101. neni mezera
    if (iscntrl(znak_navic)) {                  //Zkontroluji nepovoleny znak
      nepovoleny_znak = true;
      return 0;
    }
    else                                        //Pokud ho muzu vytisknout
      for (int i = 0 ; i != 100 ; i ++)         //Vytisknu prvnich 100 znaku
        putchar(vstup[i]);

    putchar(znak_navic);                        //Potom i 101. znak

    while (!isspace(znak_navic = getchar ())) { //A zbytek znaku az po bili znak
      if (iscntrl(znak_navic)) {                //Pokud je netisknutelny, ukoncim to.
        nepovoleny_znak = true;
        return 0;
      }
      else
        putchar(znak_navic);
    }

    printf(" (Word is longer than 100 chars! Palindrome wasn't detected)\n");
    return 1;
  }


  if (pouze_cislice) {            //Pokud je to cislo, nastavim "number" a overim prime
    if (prectenych_znaku == pocet_nul)
      pocet_nul --;
    if (atof(vstup) > INT_MAX)    //Kdyz je cislo vetsi nez INT_MAX neprovadim detekci (proto float)
      printf("number: %s (Bigger than INT_MAX)\n", &vstup[pocet_nul]);
    else {
      cislo = atoi(vstup);
      printf("number: %s%s\n", &vstup[pocet_nul], is_prime());
    }
  }

  else {         //Jinak je to slovo, nastavim "word" a overim palindrom
    if (date_wrong_format)
      printf("word: %s (wrong format of day)\n", vstup);
    else
      printf("word: %s%s\n", vstup, is_palindrome(prectenych_znaku));
  }

  return 1;      //Vratim 1 jako potvrzeni vypsani
}


///Hlavni funkce
int main (int argc, char *argv[]) {
  vstup[0] = '\0';                //Na zacatek je pole prazdne
  (void) argv;                    //Kvuli hlaseni gcc o nepouziti...

///Pokud je program spusten s parametry
  if (argc != 1) {
    printf(
      "Projekt 1\n"
      "Autor: V. Martinka (xmarti76)\n\n"
      "Program cte data ze stdin a rozhodne, zda se jedna o cislo, datum nebo slovo\n"
      "  - Maximalni delka vstupu je 100 znaku (delsi vstup nebude zpracovan, pouze vypsan)\n\n\n"
      "  # Cislo je oznaceno num:\n"
      "    - Musi byt pouze z mnoziny prirozenych cisel (mohou zacinat libovolnym poctem 0)\n"
      "    - U cisel mensich nez %d se provadi detekce na prvocislo\n"
      "  # Datum je oznacen date:\n"
      "    - Muze byt ve formatu DD-MM-YYYY nebo YYYY-MM-DD\n"
      "    - 0 ve dnech/mesicich nejsou povinne a jako oddelovac muze slouzit libovolny znak, ktery neni cislo nebo pismeno\n"
      "    - Jedna-li se o realny datum v rozsahu 1-1-%d az 31-12-%d bude dopocitan den v tydnu\n"
      "    - V opacnem pripade je datum oznacen jako neplatny\n"
      "  # Slovo je oznaceno word:\n"
      "    - Vse, co nesplni predchozi podminky je slovo\n"
      "    - Provadi se kontrola na palindrom, zalezi na velikosti pismen\n"
      "    - Je mozne, ze diakritika a specialni znaky nebudou fungovat korektne, zalezi na kodovani vaseho systemu!\n\n"
      "  * Program se ukonci, pokud narazi na EOF (Ctrl + D pro Linux, Ctrl + Z pro Windows)\n\n"
      "  ! V pripade, ze vstup obsahuje nejaky ridici znak, je program ukoncen. !\n", INT_MAX, rok_min, rok_max);

    return 0;
  }


///Pokud je program spusten bez parametru
  while (scanf("%100s", vstup) != EOF) {            //Ctu vstup, max 100 znaku a zjistuji, zda uz nejsem na konci (Ctrl+D)
    if (pis_radek() != 1)
      chyba(2);
    if (nepovoleny_znak){
      printf("     - Nalezen nepovoleny znak!\n");

    return 0;
    }
  }
  return 0;
}
