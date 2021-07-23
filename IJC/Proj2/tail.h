/** HEADER *****************
	tail.h
  Reseni IJC-DU2, 25.4.2016
  Autor: Vaclav Martinka, FIT - 1. rocnik BIT
  Prelozeno: gcc 4.9
****************************/

#ifndef tail_header
#define tail_header

#define MAX_LENGTH 514 //(512 + 1 + 1) +1 kvuli kontrole preteceni +1 kvuli '\0'

typedef struct {
	unsigned long n;
	char *file_name;
	FILE *input;
}options;


//Precte argumenty programu a zapise je do struktury options.
	void read_args(int *argc, char *argv[], options *options);


//Otevre soubor z options a do options ulozi jeho adresu
	void open_file(options *options);


//Zavre soubor z options
	void close_file(options *options);


/*Alokuje pamet pro n radku o maximalni delce max_length
	V pripade chyby vraci NULL */
	char **create_line_buffer(unsigned long n);


//Smaze cely buffer
	void destroy_line_buffer(char **line_buffer, unsigned long n);


/*Cte postupne radky z input
	uklada je do line_buffer_name (pamatuje si jen n poslednich radku)
	cte jen prvnich MAX_LENGTH znaku (zbytek ignoruje)
	Vraci index prvniho radku v poli <0-n> */
	unsigned long read_lines(char **line_buffer, FILE *input, unsigned long n);


/*Postupne tiskne radky z line_buffer_name
	Tiskne radky od index, celkem n radku */
	void print_lines(char **line_buffer, unsigned long n, unsigned long index);

#endif
