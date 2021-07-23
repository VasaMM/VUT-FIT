NAME=kry
CC=g++
CFLAGS= -pedantic -Wall -Wextra -std=c++17
# https://stackoverflow.com/questions/29046724/undefined-reference-to-imp-gmpz-init-building-gmp-program-on-cygwin
CFLAGS_END= -lgmpxx -lgmp
DEFINES=
publicKey=(0x52910a011565810be90d03a299cb55851bab33236b7459b21db82b9f5c1874fe, 0xe3d03339f660528d511c2b1865bcdfd105490ffc4c597233dd2b2504ca42a562)


.PHONY: clean all decipher pack


all: $(NAME)

$(NAME): main.o ecc.o
	@$(CC) $(CFLAGS) $(DEFINES) main.o ecc.o -o $(NAME) $(CFLAGS_END)

# Obecne generujici pravidlo
%.o: %.cpp
	@$(CC) $(CFLAGS) $(DEFINES) -c $< -o $@ $(CFLAGS_END)

decipher: $(NAME)
	@./$(NAME) "$(publicKey)"

clean:
	rm -f *.o
	
pack:
	rm -f xmarti76.zip
	zip -v -r xmarti76.zip *.cpp *.hpp Makefile