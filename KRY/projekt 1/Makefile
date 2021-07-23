###########################
## @file functions.hpp
## @version 1.0
## @author Vaclav Martinka
## @date 03. 04. 2021
## @brief Makefile pro _Projekt 1 (KRY)_, VUT Brno
## 

CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -o3 -D NODEBUG
TARGET = kry
DEPS = functions.hpp
OBJ = functions.o main.o

all: $(TARGET)


%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o
	
pack:
	rm -f xmarti76.zip
	zip -v -r xmarti76.zip *.cpp *.hpp Makefile readme.md
