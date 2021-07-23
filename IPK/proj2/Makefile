# # # # # # # # # # # # # # # # # # #
# AUTOR: Václav Martinka (xmarti76) #
# Makefile pro IPK                  #
# 21. 04. 2017 FIT VUT              #
# # # # # # # # # # # # # # # # # # #

CC=g++
CFLAGS=-std=c++11 -pedantic -Wall

all: trace

## Tracerout
trace: functions.o main.o
	$(CC) $(CFLAGS) -lrt functions.o main.o -o trace

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

functions.o: functions.cpp functions.hpp
	$(CC) $(CFLAGS) -c functions.cpp -o functions.o



## CLEAN
clean:
	rm -f *.o
clean_all:
	rm -f *.o trace
clear:
	make clean
clear_all:
	make clean_all

## Pack
pack:
	tar -czf xmarti76.tgz *.cpp *.hpp Makefile *.1
#TODO pridat dokumentaci
zip:
	make pack
