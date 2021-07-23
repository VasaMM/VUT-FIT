## HEADER ##################
#	Makefile
#	Reseni IJC-DU1, 23.3.2016
#	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
#	Prelozeno: gcc 4.9
############################

CC= gcc
CFLAGS= -O2 -std=c99 -Wall -pedantic


## All
all: steg-decode primes primes-inline clean


## Primes
primes: primes.o eratosthenes.o error.o 
	$(CC) $^ -o primes

primes.o: primes.c eratosthenes.h bit_array.h error.h
	$(CC) $(CFLAGS) -c $< -o $@

eratosthenes.o: eratosthenes.c eratosthenes.h bit_array.h error.h
	$(CC) $(CFLAGS) -c $< -o $@

error.o: error.c error.h
	$(CC) $(CFLAGS) -c $< -o $@
	
	
## Primes-inline
primes-inline: primes-inline.o eratosthenes.o error.o 
	$(CC) $^ -o primes-inline
	
primes-inline.o: primes.c eratosthenes.h bit_array.h error.h
	$(CC) $(CFLAGS) -c $< -o $@ -DUSE_INLINE


## Steg-decode
steg-decode: steg-decode.o  ppm.o eratosthenes.o error.o
	$(CC) $^ -o steg-decode

steg-decode.o: steg-decode.c eratosthenes.h error.h ppm.h bit_array.h
	$(CC) $(CFLAGS) -c $< -o $@

ppm.o: ppm.c ppm.h error.h
	$(CC) $(CFLAGS) -c $< -o $@

	
## clean
clean:
	rm -f *.o
	

## zip
zip: all
	rm -f *.zip
	zip xmarti76.zip *.c *.h Makefile