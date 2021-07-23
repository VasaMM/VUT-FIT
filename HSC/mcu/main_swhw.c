#include <fitkitlib.h>
#include "../fpga/src_filter/addr_space.h"
#include "../cpu/common.h"
#include <stdint.h>


/*******************************************************************************
 * Vypis uzivatelske napovedy (funkce se vola pri vykonavani prikazu "help")
*******************************************************************************/
inline void print_user_help() {}


/*******************************************************************************
 * Dekodovani uzivatelskych prikazu a jejich vykonavani
*******************************************************************************/
inline unsigned char decode_user_cmd( char *cmd_ucase, char *cmd ) {
	return ( CMD_UNKNOWN );
}


/*******************************************************************************
 * Inicializace periferii/komponent po naprogramovani FPGA
*******************************************************************************/
inline void fpga_initialized() {}


/*******************************************************************************
 Funkce fpga_read() slouzi pro cteni hodnoty z adresoveho prostoru FPGA.

 Vstupy:
   addr  index 32-bitove polozky v ramci adresoveho prostoru FPGA
 Vystupy:
   navratova hodnota reprezentuje prectenou hodnotu z FPGA
*******************************************************************************/
uint32_t fpga_read( int addr ) {
	uint32_t val;
	FPGA_SPI_RW_AN_DN( SPI_FPGA_ENABLE_READ, addr, (unsigned char*)&val, 2, 4 );

	return val;
}


/*******************************************************************************
 Funkce fpga_read() slouzi pro cteni hodnoty z adresoveho prostoru FPGA.

 Vstupy:
   addr  index 32-bitove polozky v ramci adresoveho prostoru FPGA
   data  prectena hodnota z FPGA
*******************************************************************************/
inline void fpga_read2( int addr, uint32_t *data ) {
	FPGA_SPI_RW_AN_DN( SPI_FPGA_ENABLE_READ, addr, (unsigned char*)data, 2, 4 );
}


/*******************************************************************************
 Funkce fpga_write() slouzi pro zapis hodnoty do adresoveho prostoru FPGA.

 Vstupy:
   addr  index 32-bitove polozky v ramci adresoveho prostoru FPGA
 Vystupy:
   data  data pro zapis
*******************************************************************************/
inline void fpga_write( int addr, uint32_t data ) {
	FPGA_SPI_RW_AN_DN( SPI_FPGA_ENABLE_WRITE, addr, (unsigned char*)&data, 2, 4 );
}


uint32_t otsu( uint32_t *hist );
void print_results( uint32_t frame, uint32_t threshold, uint32_t *hist );

/*******************************************************************************
 * Hlavni funkce
*******************************************************************************/
int main() {
	short counter = 0;
	uint32_t mcu_ready;

	initialize_hardware();
	set_led_d6( 1 );  // rozsvitit LED D6
	set_led_d5( 1 );  // rozsvitit LED D5

	/**************************************************************************/
	/*                      Aktualizovany hlavni program                      */
	/**************************************************************************/
	uint32_t frame_cnt = 1;
	uint32_t prev_frame_cnt = 0;
	uint32_t histogram[ PIXELS ] = { 0 };
	uint32_t i;
	uint32_t threshold;


	mcu_ready = 1;
	fpga_write( FPGA_MCU_READY, mcu_ready );
	while( fpga_read( FPGA_MCU_READY ) != 2 ) {}

	term_send_str("Both FPGA and MCU are ready.\n"); 

	while ( 1 ) {
		// Prectu si cislo snimku
		fpga_read2( FPGA_FRAME_CNT, &frame_cnt );

		// Pokud se lisi od predchoziho a je delitelne 10
		if ( frame_cnt != prev_frame_cnt && frame_cnt % 10 == 0 ) {
			// Nactu histogram z FPGA a vynuluji ho
			for ( i = 0 ; i < PIXELS ; ++i ) {
				fpga_read2( FPGA_HISTOGRAM + i, &histogram[i] );
				fpga_write( FPGA_HISTOGRAM + i, 0 );
			}

			threshold = otsu( histogram );
			fpga_write( FPGA_THRESHOLD, threshold );

			if ( frame_cnt % 100 == 0 ) {
				print_results( frame_cnt, threshold, histogram );
			}


			prev_frame_cnt = frame_cnt;
		}
	}

	/**************************************************************************/

	set_led_d5( 0 );  //zhasnout LED D5

	while ( 1 ) {
		delay_ms( 1 );  //zpozdeni 1ms

		++counter;
		if ( counter == 500 ) {
			flip_led_d6(); //invertovat LED
			counter = 0;
		}

		terminal_idle();  // obsluha terminalu
	}
}





/**************************************************************************/
/*                          ---  PRIDANO  ---                             */
/**************************************************************************/

/***************************************************************************
 Funkce otsu() vypocte hodnotu prahu na zaklade histogramu pixelu snimku.

 Vstupy:
    hist  ukazatel na histogram
 Vystupy:
    hodnota vypocteneho prahu
***************************************************************************/
uint32_t otsu( uint32_t *hist ) {
	unsigned long total = 0;
	float sum = 0;
	float sumB = 0;
	float varMax = 0;
	float varBetween;
	int	wB = 0;
	int wF = 0;
	uint32_t threshold = 0;
	float mB, mF;  
	int	t;

	for ( t = 0 ; t < PIXELS ; ++t ) {
		sum += t * hist[t];
		total += hist[t];
	}

	for ( t = 0 ; t < PIXELS; ++t ) {
		wB += hist[t];			  /* Weight Background */
		if ( wB == 0 ) { continue; }

		wF = total - wB;			 /* Weight Foreground */
		if ( wF == 0 ) { break; }

		sumB += (float)( t * hist[t] );

		mB = sumB / wB;				/* Mean Background */
		mF = ( sum - sumB ) / wF;	/* Mean Foreground */

		/* Calculate Between Class Variance */
		varBetween = (float)wB * (float)wF * ( mB - mF ) * ( mB - mF );

		/* Check if new maximum found */
		if ( varBetween > varMax ) {
			varMax = varBetween;
			threshold = t + 1;
		}
	}

	return threshold;
}



/***************************************************************************
 Pomocna procedura pro tisk vysledku

 Vstupy:
   frame      cislo sminku
   threshold  vypocteny prah
   hist       adresa histogramu
   n          pocet polozek histogramu
***************************************************************************/
void print_results( uint32_t frame, uint32_t threshold, uint32_t *hist ) {
	term_send_str( "Frame: " );
	term_send_num( frame );
	term_send_crlf();

	term_send_str( "Histogram: " );
	term_send_num( hist[0] );

	short i;
	for ( i = 1 ; i < PIXELS ; ++i ) {
		term_send_str( ", " );
		term_send_num( hist[i] );
	} 

	term_send_crlf();

	term_send_str( "Threshold: " );
	term_send_num( threshold );
	term_send_crlf();
}
