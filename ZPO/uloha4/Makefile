# # # # # # # # # # # # # # # # # # #
# AUTOR: Václav Martinka (xmarti76) #
# Makefile pro ZPO - uloha 1        #
# 05. 03. 2019 FIT VUT              #
# # # # # # # # # # # # # # # # # # #

CC=g++-8.3
CFLAGS=-std=c++17 -pedantic -Wall
DEBUG=-D NODEBUG

all: mt04.cpp
	$(CC) $(CFLAGS) $(DEBUG) -I. -I/usr/local/include -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs mt04.cpp -o mt04 


## CLEAN
clean:
	rm mt04


## TESTY
test: all
	./mt04 garden.png 0.4 31 2.0

# Chyba v binarce
# https://fitator.cz/search/?q=v+%3D+sqrt%28v%C3%BDkon%3D0.2%29+%2A+gaussian%28sigma%3D1.0%29&tag=zpo&s=date&t=desc
ref: all
	./ref garden.png 0.4 31 2.0
