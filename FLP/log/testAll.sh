#!/bin/bash

# Autor: Václav Martinka
# Login: xmarti76
# Datum: 01. 04. 2020
# Předmět: FLP - FIT VUT Brno
# Zadání: Logicky projekt  - Kostra grafu
# Popis: Skript pro spouštění testů



# Zkompiluji
make all 1> /dev/null


# Smazu predchozi logy
rm -f *.log


# Spustim testy
echo
echo "### Spoustim testy ###"
tests_in=`find test/test_*.in`

for test in $tests_in
do
    echo $test

    cat $test | ./flp20-log > `echo $test | sed -e 's/.in$/.log/'`
    exit_status=$?

    if [ $exit_status -ne 0 ]; then
        echo "Test selhal"
    else
        echo "OK"
    fi
    echo
done