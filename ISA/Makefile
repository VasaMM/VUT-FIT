# # # # # # # # # # # # # # # # # # #
# AUTOR: Václav Martinka (xmarti76) #
# Makefile pro POP3 client (ISA)    #
# 20. 11. 2017 FIT VUT              #
# # # # # # # # # # # # # # # # # # #

CC=g++
CFLAGS=-std=c++11 -pedantic -Wall
DEBUG=

all: popcl

## POP3 klient
popcl: functions.o client.o options.o main.o
	$(CC) $(CFLAGS) functions.o client.o options.o main.o -o popcl -lcrypto -lssl

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

functions.o: functions.cpp functions.hpp
	$(CC) $(CFLAGS) -c functions.cpp -o functions.o

client.o: client.cpp client.hpp
	$(CC) $(CFLAGS) -c client.cpp -o client.o

options.o: options.cpp options.hpp
	$(CC) $(CFLAGS) -c options.cpp -o options.o



## CLEAN
clean:
	rm -f *.o
clean_all:
	rm -f *.o popcl
clear:
	make clean
clear_all:
	make clean_all