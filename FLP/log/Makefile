
# Autor: Václav Martinka
# Login: xmarti76
# Datum: 31. 03. 2020
# Předmět: FLP - FIT VUT Brno
# Zadání: Funkcionální projekt rka-2-dka (Převod rozšířených nedeterministických konečných automatů na deterministické)
# Popis: Makefile

all: src/main.pl
	swipl -q -g main -o flp20-log -c src/main.pl

test: FORCE
	./testAll.sh

# clean:
# 	rm -f src/*.hi src/*.o

cleanLogs:
	rm -f test/*.log

# cleanAll: clean cleanLogs
# 	rm -f rka-2-dka

FORCE: ;

zip:
	rm -f flp-log-xmarti76.zip
	zip -r flp-log-xmarti76 doc src/*.pl test/*.in Makefile testAll.sh