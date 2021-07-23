# # # # # # # # # # # # # # # # # # #
# AUTOR: Václav Martinka (xmarti76) #
# Makefile pro bms (BMS)            #
# 07. 12. 2020 FIT VUT Brno         #
# # # # # # # # # # # # # # # # # # #


CXX=g++-9
CXXFLAGS=-std=c++2a -pedantic -Wall -o3 -D NODEBUG

# Merlin
ifeq "$(shell command -v g++-9.2 > /dev/null ; echo $$?)" "0"
	CXX=g++-9.2
endif

SOURCES=main.cpp Coder.cpp


all: bms

bms: $(SOURCES:.cpp=.o)
	$(CXX) $(CXXFLAGS) $(SOURCES:.cpp=.o) $(SOURCES_PEER:.cpp=.o) -o bms


test: bms
	./bms -d


## CLEAN
clean:
	rm -f *.o


## Pack
pack:
	rm -f xmarti76.zip
	zip -v -r xmarti76.zip *.cpp *.hpp Makefile

