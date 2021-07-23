# # # # # # # # # # # # # # # # # # #
# AUTOR: Václav Martinka (xmarti76) #
# Makefile pro ZPO - uloha 1        #
# 05. 03. 2019 FIT VUT              #
# # # # # # # # # # # # # # # # # # #

CC=g++-8.3
CFLAGS=-std=c++17 -pedantic -Wall
DEBUG=-D NODEBUG

all: mt05.cpp
	$(CC) $(CFLAGS) $(DEBUG) -I. -I/usr/local/include -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs mt05.cpp -o mt05 


## CLEAN
clean:
	rm mt05


## TESTY
test: all
	./mt05 garden.png 0.05 7

ref: all
	./ref garden.png 0.05 7
