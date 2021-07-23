# # # # # # # # # #
# AUTOR: xmarti76 #
# client pro IPK  #
# 14. 03. 2017    #
# # # # # # # # # #

CC=g++
CFLAGS=-std=c++11 -pedantic -Wall

all: server client

## SERVER
server: server_main.o server.o functions.o client.o
	$(CC) $(CFLAGS) server_main.o server.o functions.o client.o -o ftrestd

server_main.o: server_main.cpp
	$(CC) $(CFLAGS) -c server_main.cpp -o server_main.o

server.o: server.cpp server.hpp
	$(CC) $(CFLAGS) -c server.cpp -o server.o


## CLIENT
client: client_main.o client.o functions.o server.o
	$(CC) $(CFLAGS) client_main.o client.o functions.o server.o -o ftrest

client_main.o: client_main.cpp
	$(CC) $(CFLAGS) -c client_main.cpp -o client_main.o

client.o: client.cpp client.hpp
	$(CC) $(CFLAGS) -c client.cpp -o client.o



## CLIENT + SERVER
functions.o: functions.cpp functions.hpp
	$(CC) $(CFLAGS) -c functions.cpp -o functions.o


## CLEAN
clean:
	rm -f *.o
clean_all:
	rm -f *.o server client
clear:
	make clean
clear_all:
	make clean_all

## Pack
pack:
	tar -czf xmarti76.tgz *.cpp *.hpp Makefile *.md
zip:
	make pack