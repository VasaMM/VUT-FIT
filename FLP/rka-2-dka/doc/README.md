# rka-2-dka
- **Autor:** Václav Martinka
- **Login:** xmarti76
- **Datum:** 31. 03. 2020
- **Předmět:** FLP - FIT VUT Brno
- **Zadání:** Funkcionální projekt rka-2-dka (Převod rozšířených nedeterministických konečných automatů na deterministické)


## Struktura:
Projekt je členěn do několika adresářů:
 - _src_ - obsahuje zdrojové soubory v jazyce Haskell
     - _main.hs_ - zpracuje argumenty z příkazové řádky a podle toho volá potřebné funkce
     - _DataFsm.hs_ - definice nových datových typů
     - _ListFuncs.hs_ - pomocné funkce pro práci se seznamy
     - _Parse.hs_ - Parsovací funkce pro převod _Stringu_ na datový typ _FSM_
     - _Functions.hs_ - Funkce pro determinizaci konečného automatu a její pomocné funkce
 - _doc_ - Dokumentace v souboru _README.md_
 - _test_ - Testy pro ověření fungování algoritmu. Vstupem je soubor _*.in_, výstupem _*.out_. Výstup z programu nemusí přesně odpovídat, protože může dojít k přejmenování stavů!
     - _error_1_ - Test na záporné číslo stavu
     - _error_2_ - Test na nečíselný stav
     - _error_3_ - Test na symbol mimo rozsah a-z
     - _error_4_ - Test na chybějící řádek
     - _error_5_ - Test na neúplně definovaný přechod
     - _test_example_ - Vstup ze zadání
     - _testy_empty_ - Zcela prázdný vstup


## Překlad
Program se překládá pomocí _Makefile_
 - `make` popř. `make all` - vytvoří spustitelný soubor _rka-2-dka_
 - `make test` - spustí testy
 - `make clean` - smaže dočasné soubory
 - `make cleanLogs` - smaže výstupy testů
 - `make cleanAll` - smaže dočasné soubory i výslednou binárku
 - `make zip` - vytvoří archiv

 
## Spuštění
Program očekává jeden nebo dva argumenty. Je-li spuštěn s jiným počtem argumentů, vypíše nápovědu a ukončí se.

Použití `rka-2-dka (-i|-t|-h|--help) <FILE>` kde:
 - `-h`, `--help` - zobrazí nápovědu a ukončí se
 - `-i` - zpracuje vstup a vypíše ho. Během toho dochází k validaci a převodu na vnitřní reprezentaci, ale výstup by měl přesně odpovídat vstupu
 - `-t` - převede automat na vstupu na deterministický
 - `FILE` - nepovinný argument se jménem vstupního souboru. Je-li prázdný, očekává se vstup na _stdin_


## Testy
Ve složce _test_ je nachystáno několik testů. Skript _testAll.sh_ se stará o spuštění všech testů a jejich jednoduché vyhodnocení. Testy začínající na `error_` by měli vést na chybu a korektní ukončení programu. Naopak testy `test_` nesmí skončit chybou a jejich výsledek by měl odpovídat stejnojmennému souboru s příponou `.out`. Shoda nemusí být přesná, protože nemusí odpovídat jména stavů, proto skript ani neprovádí porovnání souborů, pouze uloží výstup do soubor `.log`.
