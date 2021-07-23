/** HEADER *****************
	main.c
  Reseni IJC-DU2, 25.4.2016
  Autor: Vaclav Martinka, FIT - 1. rocnik BIT
  Prelozeno: gcc 4.9

  Jako POSIX tail...
****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "tail.h"
#include "error.h"



/************************
	Alokuje pamet pro n radku o maximalni delce max_length
	V pripade chyby vraci NULL
*/
char **create_line_buffer(unsigned long n) {
	char **output = 0;

	output = (char **)malloc(n * sizeof(char *));
		if(!output) return NULL;

	for(unsigned i = 0 ; i < n ; i++) {
		output[i] = (char*)malloc(MAX_LENGTH * sizeof(char));
			if(!output[i]) return NULL;
		output[i][0] = 0;
		output[i][MAX_LENGTH - 2] = '\n';		//Predposledni byte nastaven na '\n' -> kontrola preteceni
	}

	return output;
}



/************************
	Cte postupne radky z input
	uklada je do line_buffer_name (pamatuje si jen n poslednich radku)
	cte jen prvnich MAX_LENGTH znaku (zbytek ignoruje)
	Vraci index prvniho radku v poli <0-n>
*/
unsigned long read_lines(char **line_buffer_name, FILE *input, unsigned long n) {
	unsigned long i = 0;
	int tmp, erros = 0;

	while(1) {
		fgets(line_buffer_name[i], MAX_LENGTH + 1, input);
			if(line_buffer_name[i][MAX_LENGTH - 2] != '\n') {			//Nacteno MAX_LENGTH znaku fgetc
				line_buffer_name[i][MAX_LENGTH - 2] = '\n';
				line_buffer_name[i][MAX_LENGTH - 1] = 0;
				while((tmp = fgetc(input)) != '\n') {		//Ignoruji dalsi znaky az do konce radku
					if(tmp == EOF)
						break;
				}
				erros ++;
				if(erros == 1) {		//Tisknu pouze prvni vyskyt preteceni
					warning_msg("I found line, which is longer than %d (%c%c%c%c%c ...)",
						MAX_LENGTH - 2, line_buffer_name[i][0], line_buffer_name[i][1],
						line_buffer_name[i][2], line_buffer_name[i][3], line_buffer_name[i][4]);
				}
			}

		if(feof(input)) break;
		i++;
		if(i == n) {
			i = 0;
		}
	}

	return i;
}



/************************
	Postupne tiskne radky z line_buffer_name
	Tiskne radky od index, celkem n radku
*/
void print_lines(char **line_buffer, unsigned long n, unsigned long index) {

	for(unsigned long i = 0 ; i < n ; i++){
		printf("%s", line_buffer[index]);
		index++;
		if(index == n) {
			index = 0;
		}
	}
}



/************************
	Smaze cely buffer
*/
void destroy_line_buffer(char **line_buffer, unsigned long n) {

	for(unsigned i = 0 ; i < n ; i++) {
		free(line_buffer[i]);
	}

	free(line_buffer);
}



/************************
	Precte argumenty programu a zapise je do struktury options.
*/
void read_args(int *argc, char *argv[], options *options) {

	long tmp = 0;
	char *err=0;

	for(int i = 1 ; i < *argc ; i++) {
    if(argv[i][0] == '-') {
			if(!strcmp(argv[i],"-n")) {
				i++;
				if(i > *argc - 1) {
					fatal_error("Empty number of '-n'!");
				}
				tmp = strtoul(argv[i], &err, 10);
				if(err == argv[i] || *err != '\0')
					fatal_error("Invalid %i. argument %s, integer expected!", i - 1, argv[i]);

				options->n = (unsigned long)abs(tmp);
			}
			continue;
    }
    else if(options->file_name == 0) {
			options->file_name = argv[i];
			continue;
    }
    else
			fatal_error("Some bad argument! '%s'", argv[i]);
	}
}



/************************
	Otevre soubor z options a do options ulozi jeho adresu
*/
void open_file(options *options) {
	if(options->file_name != 0) {
		options->input = fopen(options->file_name, "r");

		if(!options->input) {
			perror(NULL);
			fatal_error("Opening of file %s was unsuccessful!", options->file_name);
		}
	}
}



/************************
	Zavre soubor z options
*/
void close_file(options *options) {
	if(options->file_name != 0)
		fclose(options->input);
}



/** MAIN */
int main(int argc, char *argv[]) {
	options options = {10, 0, stdin};

	read_args(&argc, argv, &options);
		if(options.n == 0) return 0;
	open_file(&options);

	char **last_lines = create_line_buffer(options.n);
		if(!last_lines) {
			close_file(&options);
			fatal_error("Unable to allocate memory!");
		}

	unsigned long lines = read_lines(last_lines, options.input, options.n);
	print_lines(last_lines, options.n, lines);

	destroy_line_buffer(last_lines, options.n);
	close_file(&options);

	return 0;
}
