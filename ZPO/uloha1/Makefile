# # # # # # # # # # # # # # # # # # #
# AUTOR: Václav Martinka (xmarti76) #
# Makefile pro ZPO - uloha 1        #
# 05. 03. 2019 FIT VUT              #
# # # # # # # # # # # # # # # # # # #

CC=g++-8.3
CFLAGS=-std=c++17 -pedantic -Wall
DEBUG=-D NODEBUG

all:
	$(CC) $(CFLAGS) $(DEBUG) -I. -I/usr/local/include -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs mt01.cpp -o mt01 


## CLEAN
# clean:
# 	rm -f *.o
# clean_all:
# 	rm -f *.o seasoner pic2vec xmarti76.zip
# clear:
# 	make clean
# clear_all:
# 	make clean_all


## Pack
# pack: all
# 	rm -f xmarti76.zip
# 	cp doc/xmarti76.pdf dokumentace.pdf
# 	zip -v -r xmarti76.zip *.cpp *.hpp seasoner Makefile dokumentace.pdf Doxyfile test.sh
# 	rm dokumentace.pdf

# zip:
# 	make pack


## TESTY
test: all
	./mt01 lena.png 3 gauss 0.1
