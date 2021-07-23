/*Sablona pro projekty do predmetu IPA, tema graficky editor
*Autor: Tomas Goldmann, igoldmann@fit.vutbr.cz
*
*LOGIN STUDENTA: xmarti76
*/


#include "opencv2/core/core.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <intrin.h>
#include <inttypes.h>
#include <windows.h>
#include "ipa_tool.h"

#define WIN_WIDTH 800.0
#define PROJECT_NAME "IPA - graficky editor 2017"
#define PROJECT_NAME_WIN_IN "IPA - graficky editor 2017-IN"
#define PROJECT_NAME_WIN_OUT "IPA - graficky editor 2017-OUT"

#define USE_ASSEMBLER true

using namespace cv;
using namespace std;


typedef int(*Ipa_algorithm)(unsigned char *input_data, unsigned char *output_data, unsigned int width, unsigned int height, int argc, char** argv);

int IPA_algorithm_c(unsigned char *input_data, unsigned char *output_data, unsigned int width, unsigned int height, int argc, char** argv);


int main(int argc, char** argv) {
	unsigned __int64 cycles_start = 0;
	if (argc > 3)
	{
		cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
	}

	Mat output, window_img,image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	if (!image.data)    
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	//koeficient pro prijatelne vykresleniz
	float q = WIN_WIDTH / image.cols;
	
	//vytvoreni vystupniho obrazu	
	image.copyTo(output);

	cv::resize(image, window_img, cv::Size(q*image.cols, q*image.rows));

	namedWindow(PROJECT_NAME_WIN_IN, WINDOW_AUTOSIZE);
	imshow(PROJECT_NAME_WIN_IN, window_img);

	HINSTANCE hInstLibrary = LoadLibrary("Student_DLL.dll");

	if (!hInstLibrary)
	{
		std::cout << "DLL Failed To Load!" << std::endl;
		return EXIT_FAILURE;
	}
	else
	{
		//Algoritmus
		InstructionCounter counter;
		Ipa_algorithm f;
		f = (Ipa_algorithm)GetProcAddress(hInstLibrary, "ipa_algorithm");
		
		int iterations = 0;
		bool start = USE_ASSEMBLER && f;

		if ( start ) {
			counter.start();
 			iterations = f( image.data, output.data, image.cols, image.rows, argc, argv );
			counter.end();
			if ( iterations > 0 ) {
 				cout << "Pocet taktu: ";
				counter.print();
 			}
 			else {
 				cout << "Procesor na tomto pocitaci nepodporuje AVX2 instrukce.\nBude spusten neakcelerovany algoritmus." << endl;
 				start = false;
 			}
		}

		if ( ! start ) {
			counter.start();
			iterations = IPA_algorithm_c( image.data, output.data, image.cols, image.rows, argc, argv );
			counter.end();
 			cout << "Pocet taktu: ";
			counter.print();
		}

		cout << "Pocet iteraci: " << iterations << endl;
	}


	namedWindow(PROJECT_NAME_WIN_OUT, WINDOW_AUTOSIZE);
	
	cv::resize(output, output, cv::Size(q*image.cols, q*image.rows));
	imshow(PROJECT_NAME_WIN_OUT, output);
	
	waitKey(0); 
	FreeLibrary(hInstLibrary);
	return 0;
}


int hex2ToDec( char *hex ) {
	int output = 0;
	int index = 0;

	while ( index < 2 ) {
		switch ( hex[ index ] ) {
			case '0': break; 
			case '1': output +=  1 * ( index == 0 ? 16 : 1 ); break; 
			case '2': output +=  2 * ( index == 0 ? 16 : 1 ); break; 
			case '3': output +=  3 * ( index == 0 ? 16 : 1 ); break; 
			case '4': output +=  4 * ( index == 0 ? 16 : 1 ); break; 
			case '5': output +=  5 * ( index == 0 ? 16 : 1 ); break; 
			case '6': output +=  6 * ( index == 0 ? 16 : 1 ); break; 
			case '7': output +=  7 * ( index == 0 ? 16 : 1 ); break; 
			case '8': output +=  8 * ( index == 0 ? 16 : 1 ); break; 
			case '9': output +=  9 * ( index == 0 ? 16 : 1 ); break; 
			case 'a': output += 10 * ( index == 0 ? 16 : 1 ); break; 
			case 'A': output += 10 * ( index == 0 ? 16 : 1 ); break; 
			case 'b': output += 11 * ( index == 0 ? 16 : 1 ); break; 
			case 'B': output += 11 * ( index == 0 ? 16 : 1 ); break; 
			case 'c': output += 12 * ( index == 0 ? 16 : 1 ); break; 
			case 'C': output += 12 * ( index == 0 ? 16 : 1 ); break; 
			case 'd': output += 13 * ( index == 0 ? 16 : 1 ); break; 
			case 'D': output += 13 * ( index == 0 ? 16 : 1 ); break; 
			case 'e': output += 14 * ( index == 0 ? 16 : 1 ); break; 
			case 'E': output += 14 * ( index == 0 ? 16 : 1 ); break; 
			case 'f': output += 15 * ( index == 0 ? 16 : 1 ); break; 
			case 'F': output += 15 * ( index == 0 ? 16 : 1 ); break; 
			default: return -1;
		}

		++index;
	}
	
	return output;
}

float color_distance( unsigned char *color1, unsigned char *color2 ) {
	// distance = sqrt( a^2 + b^2 + c^2 )
	// a = ( R1 - R2 )
	// b = ( G1 - G2 )
	// c = ( B1 - B2 )

	float a = color1[0] - color2[0];
	float b = color1[1] - color2[1];
	float c = color1[2] - color2[2];

	return sqrt( a * a + b * b + c * c );
}

int IPA_algorithm_c(unsigned char *input_data, unsigned char *output_data, unsigned int width, unsigned int height, int argc, char** argv) {
	/* * Vytvorim shluky * */

	// Barva, kterou chci vykreslit
	unsigned char draw[3];
	if ( argc > 2 && argv[2][0] == '#' && strlen( argv[2] ) == 7 ) {
		char   red[2] = { argv[2][1], argv[2][2] };
		char green[2] = { argv[2][3], argv[2][4] };
		char  blue[2] = { argv[2][5], argv[2][6] };

		unsigned char R = hex2ToDec( red  );
		unsigned char G = hex2ToDec( green );
		unsigned char B = hex2ToDec( blue  );

		draw[ 2 ] = R;
		draw[ 1 ] = G;
		draw[ 0 ] = B;
	}

	// Nahodne vyberu 3 barvy
	srand( (unsigned) time(NULL));
	unsigned char color1[3] = { rand() % 256, rand() % 256, rand() % 256 };
	unsigned char color2[3] = { rand() % 256, rand() % 256, rand() % 256 };
	unsigned char color3[3] = { rand() % 256, rand() % 256, rand() % 256 };
	

	float new_color1[3], new_color2[3], new_color3[3];

	// Postupne projdu vsechny pixely
	// Vyberu, ktere ze tri barev se blizi
	// Nakonec prepocitam barvy a spustim algoritmus znovu
	// Pokud nedojde ke zmene -> konec
	int image_size = width * height * 3;

	int a, b, c, iterations = 0;
	float tmp = 0.0;
	while ( true ) {
		a = b = c = 0;

		for ( int i = 0 ; i < image_size ; i += 3 ) {
			// Najdu nejblizsi barvu
			// spocitam vzdalenost v prostoru mezi aktualni barvou a trema klicovyma barvama
			float dist1 = color_distance( &input_data[ i ], color1 );
			float dist2 = color_distance( &input_data[ i ], color2 );
			float dist3 = color_distance( &input_data[ i ], color3 );

			// U barvy ktera je nejbliz si poznacim, ke ktere barve je nejbliz
			// Prepocitam novou barvu, ktera je ve stredu pro dany shluk
			if ( dist1 < dist2 && dist1 < dist3 ) {
				new_color1[ 0 ] = ( ( (float)new_color1[ 0 ] * a ) + (float)input_data[ i     ] ) / ( a + 1 );
				new_color1[ 1 ] = ( ( (float)new_color1[ 1 ] * a ) + (float)input_data[ i + 1 ] ) / ( a + 1 );
				new_color1[ 2 ] = ( ( (float)new_color1[ 2 ] * a ) + (float)input_data[ i + 2 ] ) / ( a + 1 );

				output_data[ i ] = 1;
				++a;
			}
			else if ( dist2 < dist3 ) {
				new_color2[ 0 ] = ( ( (float)new_color2[ 0 ] * b ) + (float)input_data[ i     ] ) / ( b + 1 );
				new_color2[ 1 ] = ( ( (float)new_color2[ 1 ] * b ) + (float)input_data[ i + 1 ] ) / ( b + 1 );
				new_color2[ 2 ] = ( ( (float)new_color2[ 2 ] * b ) + (float)input_data[ i + 2 ] ) / ( b + 1 );

				output_data[ i ] = 2;
				++b;
			}
			else {
				new_color3[ 0 ] = ( ( (float)new_color3[ 0 ] * c ) + (float)input_data[ i     ] ) / ( c + 1 );
				new_color3[ 1 ] = ( ( (float)new_color3[ 1 ] * c ) + (float)input_data[ i + 1 ] ) / ( c + 1 );
				new_color3[ 2 ] = ( ( (float)new_color3[ 2 ] * c ) + (float)input_data[ i + 2 ] ) / ( c + 1 );

				output_data[ i ] = 3;
				++c;
			}


		}

		++iterations;

		if (
			abs( new_color1[ 0 ] - color1[ 0 ] ) < 2 &&
			abs( new_color1[ 1 ] - color1[ 1 ] ) < 2 &&
			abs( new_color1[ 2 ] - color1[ 2 ] ) < 2 &&
			abs( new_color2[ 0 ] - color2[ 0 ] ) < 2 &&
			abs( new_color2[ 1 ] - color2[ 1 ] ) < 2 &&
			abs( new_color2[ 2 ] - color2[ 2 ] ) < 2 &&
			abs( new_color3[ 0 ] - color3[ 0 ] ) < 2 &&
			abs( new_color3[ 1 ] - color3[ 1 ] ) < 2 &&
			abs( new_color3[ 2 ] - color3[ 2 ] ) < 2
		) {
			break;
		}
		else if ( iterations > 50 ) { break; }	// Ochrana proti zaseknuti
		else {
			color1[ 0 ] = new_color1[ 0 ];
			color1[ 1 ] = new_color1[ 1 ];
			color1[ 2 ] = new_color1[ 2 ];
			
			color2[ 0 ] = new_color2[ 0 ];
			color2[ 1 ] = new_color2[ 1 ];
			color2[ 2 ] = new_color2[ 2 ];
			
			color3[ 0 ] = new_color3[ 0 ];
			color3[ 1 ] = new_color3[ 1 ];
			color3[ 2 ] = new_color3[ 2 ];
		}
		
	}

	// Vytvoril jsem shluky
	// Obarvym obrazek zpet nad shlukem, ktery je nejblize chtene barve
	// Ostatni shluky prevedu do odstinu sedi
	
	if ( argc > 2 ) {
		a = color_distance( draw, color1 );
		b = color_distance( draw, color2 );
		c = color_distance( draw, color3 );
	}

	int x;
	     if ( a < b && a < c ) { x = 1; }
	else if ( b < c          ) { x = 2; }
	else                       { x = 3; }

	for ( int i = 0 ; i < image_size ; i += 3 ) {
		// Obarvim zpet
		if ( output_data[ i ] == x ) {
			output_data[ i ] = input_data[ i ];
		}
		// Vytvorim odstiny sedi
		else {
			output_data[ i ] = input_data[ i ] / 4 + input_data[ i + 1 ] / 2 + input_data[ i + 2 ] / 4;
			output_data[ i + 1 ] = output_data[ i + 2 ] = output_data[ i ];
		}
	}

	return iterations;
}