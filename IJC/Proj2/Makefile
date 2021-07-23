### HEADER #################
#	Makefile
#	Reseni IJC-DU2, 26.4.2016
#	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
############################


CC= gcc
CFLAGS= -std=c99 -pedantic -Wall -Wextra

CPP			= g++
CPPFLAGS	= -std=c++11 -pedantic -Wall

.PHONY: all
.PHONY: zip
.PHONY: clear

## All
all: tail tail2 libhtable.so libhtable.a wordcount wordcount-dynamic 


## Tails
tail: tail.o error.o 
	$(CC) $^ -o tail

tail.o: tail.c tail.h error.h
	$(CC) $(CFLAGS) -c $< -o $@
	
error.o: error.c error.h
	$(CC) $(CFLAGS) -c $< -o $@
	
tail2: tail2.cc
	$(CPP) $(CPPFLAGS) -o $@ $^
	
	
## Wordcount
wordcount: wordcount.c htable.h libhtable.a
	$(CC) $(CFLAGS) -o $@ $^

wordcount-dynamic: wordcount.c htable.h libhtable.so
	$(CC) $(CFLAGS) -L. -o $@ $^ -lhtable
	

## Libhtable
libhtable.so: libhtable.o
	$(CC) $(CFLAGS) -shared -o $@ $^

libhtable.a: libhtable.o
	ar -cvq $@ $^

libhtable.o: libhtable.c htable.h
	$(CC) $(CFLAGS) -fPIC -o $@ -c $<


## Clean
clean:
	rm -f *.o *.a *.so
	rm -f tail tail2 wordcount wordcount-dynamic
	

## Zip
zip:
	rm -f *.zip
	zip xmarti76.zip *.c *.cc *.h Makefile