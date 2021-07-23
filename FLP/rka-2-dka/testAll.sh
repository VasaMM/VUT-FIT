#!/bin/bash

# Autor: Václav Martinka
# Login: xmarti76
# Datum: 01. 04. 2020
# Předmět: FLP - FIT VUT Brno
# Zadání: Funkcionální projekt rka-2-dka (Převod rozšířených nedeterministických konečných automatů na deterministické)
# Popis: Skript pro spouštění testů



# Zkompiluji
make all 1> /dev/null


# Smazu predchozi logy
rm -f *.log


# Otestuji chybne vstupy
echo "### TESTY NA CHYBY ###"
errors_in=`find test/error_*.in`
error_desc=("Zaporne cislo stavu" "Neciselny stav" "Symbol mimo rozsah a-z" "Chybejici radek" "Neuplne definovany prechod" "Neexistujici stav v pravidlech")
i=0

for err in $errors_in
do
    echo ${error_desc[i]}
    ./rka-2-dka -i $err 1> /dev/null
    exit_status=$?

    if [ $exit_status -eq 0 ]; then
        echo "CHYBA - nezachyceno"
    else
        echo "OK"
    fi
    echo
    ((i=i+1))
done


# Otestuji validni vstupy
echo
echo "### TESTY NA VALIDNI VSTUPY ###"
tests_in=`find test/test_*.in`

for test in $tests_in
do
    echo $test
    ./rka-2-dka -t $test > `echo $test | sed -e 's/.in$/.log/'`
    exit_status=$?

    if [ $exit_status -ne 0 ]; then
        echo "Prevod selhal"
    else
        echo "OK"
    fi
    echo
done