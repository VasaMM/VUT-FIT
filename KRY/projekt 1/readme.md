# Projekt 1 (KRY)
* Autor: Václav Martinka
* Login: xmarti76

## 1. Úvod
Úkolem bylo vyřešit problém hledání hesla k anglickému textu šifrovaném pomocí Viegenerovi šifry.

## 2. Postup
Celkem jsem použil tři přístupy. Friedmanův a Kasiského test vyžadovalo zadání, k odhadu délky a obsahu hesla jsem použil index Koincidence.

### 2.1 Friedmanův test
Má implementace vychází z odkazované stránky wikipedie (https://en.wikipedia.org/wiki/Vigenère_cipher#Friedman_test).Tvoří ji dva jednoduché cykly. Prvním získám četnost výskytu jednotlivých písmen abecedy ve vstupním textu. Druhý cyklus pak vypočte průměrný index koincidence pro daný text. Tyto hodnoty jsou dosazeny do vzorce z Wikipedie a vytištěny do terminálu jako výsledek.

### 2.2 Kasiského test
Tento postup hledá stejné podřetězce v textu a vyhodnocuje jejich vzdálenost. Ve své implementaci jsem se omezil pouze na trigramy. Hlavní cyklus postupně vytváří trigramy a hledá je ve zbytku textu. Každý trigram hledá pouze jednou. U každého nalezeného si uloží jeho vzdálenost. Ze všech těchto vzdáleností jsou vypočteny faktory a následně vybrán ten nejčastější (přičemž se ignorují hodnoty menší než 3). Pokud je jich více, bere se ten větší.

### 2.3 Index Koincidence
Slouží k finálnímu odhadu délky hesla i jeho znění. Hledání je omezeno na desetinásobek větší hodnoty z předchozích dvou testů. To by mělo být dostatečné i v případě, kdy jsou odhadnuté výsledky nepřesné, a zároveň to zabraňuje příliš dlouhému běhu programu na dlouhých textech, kde je obtížné určit heslo.

Postupně iteruji přes všechny délky hesla, až do limitu a spočítám si počet výskytů jednotlivých písmen v podřetězcích (dle přednášek). Tedy pro heslo délky jedno, je to celý text. Heslo délky dva má dva podřetězce písmen na sudých /lichých pozicích, řetězec délky tři pak má tři podřetězce z písmen na indexu 0, 3, 6, ... respektive 1, 4, 7, ... a 2, 5, 8, ...

Z těchto hodnot lze vypočíst index koincidence pro jednotlivé podřetězce. Z nich spočítám průměr a poznačím si ho -> jedná se o index koincidence pro heslo dané délky. Na závěr vyberu ten, který je nejblíže hodnotě 0.065, která dle přednášek odpovídá angličtině.

Díky znalosti délky hesla a frekvenční pravděpodobnosti písmen v angličtině, lze postupně odhadnout heslo. Opět je vstup rozdělen na podřetězce a každý počítán zvlášť. Postupně jsou vyzkoušeny všechny možné posuny v abecedě (25 posunů) a je vybrán ten, který je nejbližší hodnotě 0.065. Tímto způsobem je rychle odhadnuto možné heslo, které je předáno na výstup. _Odhadnutá délka hesla_ ve výstupu je pak délka tohoto stringu.

## 3. Překlad
Program se překládá pomocí nástroje make.
 * `make`nebo `make kry` vytvoří spustitelný program _kry_
 * `make clean` smaže dočasné soubory
 * `make pack` vytvoří zip archiv s projektem

## 4. Použití
Program lze spustit bez argumentů, pak očekává na vstupu _stdin_ zašifrovaný text, který analyzuje, spustí na něm všechny tři výše popsané testy a vypíše výsledek ve tvaru: `friedman(float);kasisky(int);odhadnutaDelkaHesla(int);heslo(string)`.
Kromě toho ho lze spustit s několika argumenty:
 * `-h`, `--help` Zobrazí nápovědu
 * `-d KLIC`, `--decode KLIC` Dešifruje text ze _stdin_ pomocí klíče `KLIC`
 * `-e KLIC`, `--encode KLIC` Zašifruje text ze _stdin_ pomocí klíče `KLIC`
 * `-a`, `--attack` stejné chování, jako při spuštění bez argumentů
 * `-s`, `--solve` obdobně jako _attack_, ale kromě hesla vypíše i dešifrovaný text

## 5. Závěr
Program splňuje všechny požadavky zadání. Po zběžném testování se ukazuje, že Friedmanův test poskytuje o poznání méně přesné výsledky než Kasisckého test. Oba mají problém s dlouhými hesly. Naproti tomu index koincidence poskytuje překvapivě kvalitní výsledky i pro dlouhá hesla.
