#ifndef __FILTER__
#define __FILTER__

#include <ac_int.h>
#include "../../cpu/common.h"

#ifdef CCS_DUT_SYSC
	#include <iostream>
	#include <string>
	using namespace std;

	#define IN_SIM(x) {x}
	#define DEBUG(x) cout << x << endl
	#define DEBUG_NOCRLF(x) cout << x << flush
#else
	#define DEBUG(x)
	#define IN_SIM(x)
	#define DEBUG_NOCRLF(x)
#endif

#define MCU_SIZE        512

typedef ac_int<PIXEL_WIDTH,false> t_pixel;
typedef ac_int<32,false> t_mcu_data;


// typedef int int_9b;  // 512
// typedef int int_8b;  // 256
// typedef int int_7b;  // 128
// typedef int int_6b;  // 64
// typedef int int_5b;  // 32
// typedef int int_4b;  // 16
// typedef int int_3b;  // 8
// typedef int int_2b;  // 4
// typedef int int_1b;  // 2


typedef ac_int<32,false> uint_32b;  // 4 294 967 296

typedef ac_int<9,false> uint_9b;  // 512
typedef ac_int<8,false> uint_8b;  // 256
typedef ac_int<7,false> uint_7b;  // 128
typedef ac_int<6,false> uint_6b;  // 64
typedef ac_int<5,false> uint_5b;  // 32
typedef ac_int<4,false> uint_4b;  // 16
typedef ac_int<3,false> uint_3b;  // 8
typedef ac_int<2,false> uint_2b;  // 4
typedef ac_int<1,false> uint_1b;  // 2


void filter(t_pixel &in_data, bool &in_data_vld, t_pixel &out_data, t_mcu_data mcu_data[MCU_SIZE]);


/***************************************************************************
 Funkce thresholding() provadi prahovani vstupniho pixelu vuci zadanemu
 prahu.

 Vstupy:
   pixel      vstupni pixel
   threshold  hodnota prahu
 Vystupy:
   navratova hodnota reprezentuje vystupni pixel po provedeni prahovani
***************************************************************************/
inline t_pixel thresholding( t_pixel pixel, int threshold ) {
	return ( pixel >= threshold ) ? MAX_PIXEL : MIN_PIXEL;
}

void buffer( t_pixel din, int c, t_pixel *col_window );
void clip_window( int r, int c, t_pixel *iw, t_pixel *ow );
void shift_window( t_pixel *window, t_pixel *col_window );
uint_1b system_input( t_pixel din, t_pixel *cliped_window, uint_1b &last_pixel );


#endif
