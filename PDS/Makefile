# # # # # # # # # # # # # # # # # # #
# AUTOR: Václav Martinka (xmarti76) #
# Makefile pro pds18-peer (PDS)     #
# 22. 04. 2019 FIT VUT Brno         #
# # # # # # # # # # # # # # # # # # #


CXX=g++-7
CXXFLAGS=-std=c++17 -pedantic -Wall -fopenmp -o3 -D NODEBUG
LDFLAGS=

EXECUTABLE_PEER=pds18-peer
EXECUTABLE_NODE=pds18-node
EXECUTABLE_RPC=pds18-rpc
SOURCES=Info.cpp Communicator.cpp parser.cpp Options.cpp
SOURCES_PEER=pds18-peer.cpp Peer.cpp
SOURCES_NODE=pds18-node.cpp Node.cpp
SOURCES_RPC=pds18-rpc.cpp Info.cpp


all: pds18-peer pds18-node pds18-rpc

pds18-peer: $(SOURCES:.cpp=.o) $(SOURCES_PEER:.cpp=.o)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(SOURCES:.cpp=.o) $(SOURCES_PEER:.cpp=.o) -o $(EXECUTABLE_PEER)

pds18-node: $(SOURCES:.cpp=.o) $(SOURCES_NODE:.cpp=.o)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(SOURCES:.cpp=.o) $(SOURCES_NODE:.cpp=.o) -o $(EXECUTABLE_NODE)

pds18-rpc: $(SOURCES_RPC:.cpp=.o)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(SOURCES_RPC:.cpp=.o) -o $(EXECUTABLE_RPC)


## Dokumentace
doxygen:
	mkdir doc -p
	doxygen Doxyfile



## CLEAN
clean:
	rm -f *.o


## Pack
pack:
	rm -f xmarti76.zip
	cp doc/xmarti76.pdf dokumentace.pdf
	zip -v -r xmarti76.zip *.cpp *.hpp Makefile dokumentace.pdf Doxyfile readme
	rm dokumentace.pdf

