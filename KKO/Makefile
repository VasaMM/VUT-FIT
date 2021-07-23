# # # # # # # # # # # # # # # # # # #
# AUTOR: Václav Martinka (xmarti76) #
# Makefile pro huff_codec (KKO)     #
# 07. 04. 2019 FIT VUT Brno         #
# # # # # # # # # # # # # # # # # # #

DEBUG=-D NODEBUG

CXX=g++
CXXFLAGS=-std=c++17 -pedantic -Wall -o3 -march=native $(DEBUG)
LDFLAGS=

EXECUTABLE=huff_codec
SOURCES=huff_codec.cpp Info.cpp Options.cpp File.cpp Huffman.cpp HuffmanTree.cpp Header.cpp Bytes.cpp Preprocessor.cpp

all: $(SOURCES:.cpp=.o)
	$(CXX) $(LDFLAGS) $(SOURCES:.cpp=.o) -o $(EXECUTABLE)


# Dokumentace
doxygen:
	mkdir doc -p
	doxygen Doxyfile


## CLEAN
clean:
	rm -f *.o


## Pack
pack: all
	rm -f xmarti76.zip
	cp doc/xmarti76.pdf dokumentace.pdf
	zip -v -r kko_xmarti76.zip *.cpp *.hpp Makefile dokumentace.pdf Doxyfile test.sh
	rm dokumentace.pdf


test: all
	./test.sh

measure: all
	./measure.sh