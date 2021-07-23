#** MAKEFILE *****************
#
#	Reseni IOS-proj2, 2.5.2016
#	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
#
#****************************/

NAME = proj2
CC = gcc
CFLAGS = -pthread -std=gnu99 -Wall -Wextra -Werror -pedantic


.PHONY: all
.PHONY: zip


## all
all: $(NAME)
	$(CC) $(CFLAGS) -lrt $(NAME).c -o $(NAME)
	

## zip
zip:
	rm -f *.zip
	zip xmarti76.zip *.c Makefile
