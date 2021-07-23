# Autor: Václav Martinka
# Login: xmarti76
# Datum: 31. 03. 2020
# Předmět: FLP - FIT VUT Brno
# Zadání: Funkcionální projekt rka-2-dka (Převod rozšířených nedeterministických konečných automatů na deterministické)
# Popis: Makefile

all:
	ghc -Wall src/*.hs -o rka-2-dka

test: FORCE
	./testAll.sh

clean:
	rm -f src/*.hi src/*.o

cleanLogs:
	rm -f test/*.log

cleanAll: clean cleanLogs
	rm -f rka-2-dka

FORCE: ;

zip:
	rm -f flp-fun-xmarti76.zip
	zip -r flp-fun-xmarti76 doc src/*.hs test/*.in test/*.out Makefile testAll.sh