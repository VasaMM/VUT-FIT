# Kostra grafu
- **Autor:** Václav Martinka
- **Login:** xmarti76
- **Datum:** 31. 03. 2020
- **Předmět:** FLP - FIT VUT Brno
- **Zadání:** Logický projekt - Kostra grafu


## Struktura:
Projekt je členěn do několika adresářů:
 - _src_ - obsahuje zdrojové soubory v jazyce Prolog
     - _main.pl_ - obsahuje kompletní implementaci
 - _doc_ - Dokumentace v souboru _README.md_
 - _test_ - Testy pro ověření fungování algoritmu. Vstupem je soubor _*.in_
     - _test_1_ - Jednoduchý graf se třemi uzly a třemi hranami
     - _test_2_ - Graf pouze s jednou kostrou, dvojnásobná definice jedné z hran (nesmí mít vliv na výsledek)
     - _test_3_ - Nesouvislý graf - může vést k chybě programu
     - _test_4_ - Prázdný soubor - může vést k chybě programu
     - _test_5_ - Složitější graf se čtyřmi uzly
     - _test_6_ - Složitý graf s šesti uzly
     - _test_7_ - Jednoduchý graf, hrany definovány dvakrát s různým pořadím uzlů - "A B" i "B A" (nesmí mít vliv na výsledek)
     - _test_8_ - Nesouvislý graf s dvěma uzly a dvěma hranami - dvojice "A A" a "B B" - může vést k chybě programu
     - _test_9_ - Graf který je zároveň i kostrou


## Překlad
Program se překládá pomocí _Makefile_
 - `make` popř. `make all` - vytvoří spustitelný soubor _flp20-log_
 - `make test` - spustí testy
 - `make cleanLogs` - smaže výstupy testů
 - `make zip` - vytvoří archiv

 
## Spuštění
Program po spuštění čeká a vstup. Jakmile je vstup zadán, zpracuje ho. Nevalidní řádky (prázdné nebo s jinám počtem uzlů než dva) jsou ignorovány. Zbylé řádky vstupu jsou kontrolovány, zda jsou názvy uzlu validní (pouze jedno písmeno z rozsahu _A_ až _Z_) a chybné jsou opět ignorovány.

Zbylé řádky jsou převedeny na seznam struktur `edge`, přičemž jak uzly v rámci struktury, tak struktury v rámci seznamu jsou seřazeny podle abecedy, čímž jsou zároveň odstraněny duplicity.



## Popis algoritmu
Kostra grafu je tvořena postupně počínaje prvním uzlem. Ten je odstraněn ze seznamu všech uzlů (může být zvolen libovolný, v algoritmu se vybírá abecedně první). Nyní se hledá taková hrana, která spojuje aktuální kostru grafu s jedním doposud nezpracovaným uzlem. Pokud taková hrana neexistuje, nelze vytvořit kostru grafu.

Pokud je nalezena, připojí se ke kostře grafu a nově přidaný uzel se odstraní ze seznamu doposud nepřipojených uzlů a celý postup se opakuje, dokud není seznam nezpracovaných uzlů prázdný. 


## Testy
Ve složce _test_ je nachystáno devět testů. Skript _testAll.sh_ se stará o spuštění všech testů. Výstup je uložen do souboru `.log`. Některé z testů mohou končit _chybou_ pokud je vstupní test nevalidní.
