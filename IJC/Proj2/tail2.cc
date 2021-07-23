/** HEADER *****************
	main.c
  Reseni IJC-DU2, 25.4.2016
  Autor: Vaclav Martinka, FIT - 1. rocnik BIT
  Prelozeno: g++ 4.9.3

  Jako POSIX tail...
****************************/

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


///Struktura s nastavenim
typedef struct {
	unsigned long n;
	string file_name;
	istream *input;
}options;



/************************
	Cte postupne radky z input
	uklada je do last_lines (pamatuje si jen n poslednich radku)
	Nakonec vytiskne last_lines
*/
void tail(istream &input, unsigned long n) {
	unsigned long i = 0;
	unsigned long lines = 0;
	string one_line;
	vector<string> last_lines(n);

	while(getline(input,one_line)) {
		lines ++;
		last_lines[i] = one_line;

		i++;
		if(i == n) {
			i = 0;
		}
	}

	if(lines < n) {
		i = 0;
		n = lines;
	}

	for(unsigned long j = 0 ; j < n ; j++) {
		cout << last_lines[i] << "\n";
		i ++;
		if(i == n) {
			i = 0;
		}
	}
}




/************************
	Precte argumenty programu a zapise je do struktury options.
*/
void read_args(int *argc, char *argv[], options *options) {
	long tmp = 0;

	for(int i = 1 ; i < *argc ; i++) {
		if(argv[i][0] == '-') {
				if(string(argv[i]) == "-n") {
				i++;
				if(i > *argc - 1) {
					cerr << "Empty number of '-n'!\n";
				}
				tmp = stol(argv[i], nullptr, 10);
				///Jak ochranit?
				options->n = (unsigned long)abs(tmp);
				}
				continue;
				//dalsi pripadne argumenty
		}

		else if(options->file_name == "") {
				options->file_name = string(argv[i]);
				continue;
		}
		else
				cerr << "Some bad argument!\n" << argv[i];
	}
}


/** MAIN */
int main(int argc, char *argv[]) {
	ios::sync_with_stdio(false);
	
	options options = {10, "", &cin};
	ifstream input_file;

	read_args(&argc, argv, &options);
	if(options.n == 0) return 0;

	if(options.file_name != "") {
		input_file.open(options.file_name, ios::in);
		options.input = &input_file;
			if(!input_file) cerr << "Can't open the file" << options.file_name <<"!\n";
	}

	tail(*options.input, options.n);

	if(options.file_name != "")
		input_file.close();

	return 0;
}
