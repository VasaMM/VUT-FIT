/** HEADER *****************
	error.c
	Reseni IJC-DU1, 23.3.2016
	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
	Prelozeno: gcc 4.9.3

	Chybove funkce
****************************/


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "error.h"


void warning_msg(const char *fmt, ...) {

	va_list args;   //seznam argumentu
	va_start(args, fmt);   //naplnim argumenty od fmt

	vfprintf(stderr, "CHYBA: ", NULL);
	vfprintf(stderr, fmt, args);   //vytisknu argumenty

	va_end(args);
}


void fatal_error(const char *fmt, ...) {

	va_list args;   //seznam argumentu
	va_start(args, fmt);   //naplnim argumenty od fmt

	vfprintf(stderr, "CHYBA: ", NULL);
	vfprintf(stderr, fmt, args);   //vytisknu argumenty

	va_end(args);

	exit(1);
}
