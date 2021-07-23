/** HEADER *****************
	error.h
	Reseni IJC-DU1, 23.3.2016
	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
	Prelozeno: gcc 4.9.3
****************************/


#ifndef error_header
#define error_header

	void warning_msg(const char *fmt, ...);

	void fatal_error(const char *fmt, ...);

#endif
