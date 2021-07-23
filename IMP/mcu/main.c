/** ** ** ** ** ** ** ** ** ** ** ** **
 * AUTOR: xmarti76 (Vaclav Martinka)
 * 
 * 31. 12. 2017
 * Projekt do predmetu IMP
 * VUT FIT
 *
 ** ** ** ** ** ** ** ** ** ** ** ** **/

#include <stdbool.h>
#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>

typedef unsigned char byte;



/** ** ** ** ** ** ** ** ** ** ** ** **
 * DEFINICE KONSTANT
 ** ** ** ** ** ** ** ** ** ** ** ** **/
#define COUNT   3   // Nastavim pocet ventilatoru - maximalne 4
#define A_DELAY 7   // Delka cyklu pro program A v s
#define B_DELAY 3   // Delka cyklu pro program B v s



/** ** ** ** ** ** ** ** ** ** ** ** **
 * ZAPOJENI PINU
 ** ** ** ** ** ** ** ** ** ** ** ** **/

// Zapojeni ventilatoru
byte fans[ COUNT ] = {
	0x02,
	0x01,
	0x08
};

// Zapojeni cidel
byte sensors[ COUNT ] = {
	0x20,
	0x10,
	0x40
};



/** ** ** ** ** ** ** ** ** ** ** ** **
 * GLOBALNI PROMENNE
 ** ** ** ** ** ** ** ** ** ** ** ** **/
char last_key = 0;               // Naposledy zmacknuta klavesa
char lcd[33] = { 0, };           // String pro lcd
int  seconds;                    // Citac sekund
byte status[ COUNT ] = { 0, };   // Aktualni stav ventilatoru
long counter = 0;

// Zkontroluje stav ventilatoru
inline bool fan_is_OK( int id ) { return ! ( status[ id ] == 0xFF || status[ id ] == 0x00 ); }



/** ** ** ** ** ** ** ** ** ** ** ** **
 * PROMENNE PRO PREDDEFINOVANE PROGRAMY
 ** ** ** ** ** ** ** ** ** ** ** ** **/
bool lock = false;             // Informace, zda bezi program
int  program_counter = 0;      // Iterace beziciho programu
byte running_program_id = 0;   // ID beziciho programu
void running_program();        // Obstara beh programu

// Spusti program
inline void run_program( byte program ) {
	P6OUT = 0x00;
	lock = true;
	set_led_d5( 1 );
	running_program_id = program;
	program_counter = 0;
}



/** ** ** ** ** ** ** ** ** ** ** ** **
 * MAIN
 ** ** ** ** ** ** ** ** ** ** ** ** **/
int main( void ) {
	initialize_hardware();

	// Testovani
	// while ( true ) {
	// 	P6OUT |= fans[ 0 ];
	// 	set_led_d6( P6IN & sensors[ 0 ] );
	// }
	

	while ( true ) {
		// Nactu vstup z klavesnice a stav ventilatoru
		char tmp_char = key_decode( read_word_keyboard_4x4() );


		// Zmacknuta 'D' -> Odeslu testovaci data na terminal
		// if ( tmp_char == 'D' ) {
		// 	int i;
		// 	for ( i = 0 ; i < COUNT ; ++i ) {
		// 		term_send_num( status[ i ] );
		// 		term_send_str( "  =>  " );
		// 		term_send_str_crlf( ( fan_is_OK( i ) ? "OK" : "ERR" ) );
		// 	}
		// 	term_send_crlf();
		// }

		// Bezi program -> muze byt zmacknuta pouze 0
		if ( lock ) {
			if ( tmp_char == '0' ) {
				lock = false;
				set_led_d5( 0 );
				P6OUT = 0x00;
				CCTL0 = ! CCIE;		// Vypnu casovac
			}
			else {
				// Obstaram bezici program
				running_program();
			}
		}
		// Otestuji zmacknuti klavesy
		else if ( last_key != tmp_char ) {
			// id ventilatoru = cislo zmacknute klavesy
			int id = tmp_char - '1';

			if ( id >= 0 && id <= COUNT ) {
				// Ventilator bezi -> vypnu
				if ( P6OUT & fans[ id ] ) {
					P6OUT = P6OUT & ~ fans[ id ];
				}
				// Ventilator nebezi -> zapnu
				else {
					P6OUT |= fans[ id ];
				}
			}


			switch ( tmp_char ) {
				// Zmacknuta '*' -> zapnu vse
				case '*':
					P6OUT = 0x0F;
					break;
			
				// Zmacknuta '#' -> vse vypnu
				case '#':
					P6OUT = 0x00;
					break;

				// Zmacknuta 'A' -> spustim program A
				case 'A':
					run_program( 'A' );
					break;

				// Zmacknuta 'B' -> spustim program B
				case 'B':
					run_program( 'B' );
					break;
			}
		}


		// Projdu jednotlive ventilatory
		int i;
		for ( i = 0 ; i < COUNT ; ++i ) {
			// Otestuji, zda se ventilator toci
			status[ i ]  = P6IN & sensors[ i ] ? 0x01 : 0x00;  delay_ms( 1 );
			status[ i ] |= P6IN & sensors[ i ] ? 0x02 : 0x00;  delay_ms( 1 );

			// Nachystam string na tisknuti
			// Posunu se o 8 znaku
			int pos = i << 3;

			// Otestuji, zda se ventilator toci
			status[ i ] |= P6IN & sensors[ i ] ? 0x04 : 0x00;  delay_ms( 1 );
			status[ i ] |= P6IN & sensors[ i ] ? 0x08 : 0x00;  delay_ms( 1 );

			// Lichou pozici posunu o 1 mezeru
			if ( i % 2 ) {
				lcd[ pos ] = ' ';
				++pos;
			}

			// Nachystam string pro LCD
			lcd[ pos     ] = 'F';

			// Otestuji, zda se ventilator toci
			status[ i ] |= P6IN & sensors[ i ] ? 0x10 : 0x00;  delay_ms( 1 );
			
			// Nachystam string pro LCD
			lcd[ pos + 1 ] = '1' + i;

			// Otestuji, zda se ventilator toci
			status[ i ] |= P6IN & sensors[ i ] ? 0x20 : 0x00;  delay_ms( 1 );
			
			// Nachystam string pro LCD
			lcd[ pos + 2 ] = ':';
			
			// Otestuji, zda se ventilator toci
			status[ i ] |= P6IN & sensors[ i ] ? 0x40 : 0x00;  delay_ms( 1 );
			
			// Nachystam string pro LCD
			lcd[ pos + 3 ] = ' ';
			
			// Otestuji, zda se ventilator toci
			status[ i ] |= P6IN & sensors[ i ] ? 0x80 : 0x00;


			if ( P6OUT & fans[ i ] ) {
				if ( fan_is_OK( i ) ) {
					lcd[ pos + 4 ] = 'O';
					lcd[ pos + 5 ] = 'K';
					lcd[ pos + 6 ] = ' ';
					lcd[ pos + 7 ] = ' ';
				}
				else {
					lcd[ pos + 4 ] = 'E';
					lcd[ pos + 5 ] = 'R';
					lcd[ pos + 6 ] = 'R';
					lcd[ pos + 7 ] = ' ';
				}
			}
			else {
				lcd[ pos + 4 ] = 'O';
				lcd[ pos + 5 ] = 'F';
				lcd[ pos + 6 ] = 'F';
				lcd[ pos + 7 ] = ' ';
			}
		}

	

		lcd[ 32 ] = 0;
		
		LCD_write_string( lcd );
		
		last_key = tmp_char;
		
		delay_ms( 80 );
	}


	return 0;
}


void fpga_initialized( void ) {
	LCD_init();
	LCD_append_string( " Fan controller" );
	WDG_stop();
	keyboard_init();

	set_led_d5( 0 );
	set_led_d6( 0 );

	P6DIR = 0x0F;	// Nastavim piny 6.0 - 6.3 na  vystup; 6. - 6.7 na vstup
	P6OUT = 0;   	// a vynuluji

	TACTL = TASSEL_1 + MC_2;    // ACLK (f_tiku = 32768 Hz = 0x8000 Hz), nepretrzity rezim

	delay_ms( 1500 );
	LCD_clear();
	LCD_send_cmd( LCD_DISPLAY_ON_OFF | LCD_DISPLAY_ON | LCD_CURSOR_OFF, 0 );   // Vypne kurzor
}

void print_user_help( void ) {}

unsigned char decode_user_cmd( char *cmd_ucase, char *cmd ) { return CMD_UNKNOWN; }


void running_program() {
	counter++;
	switch ( running_program_id ) {
		// Stridave spoustim F1 - F3 (po 5 s)
		case 'A':
			switch ( program_counter ) {
				case 0:
					CCTL0 = CCIE;         // Zapnu casovac
					CCR0 += 0x8000;       // Nastavim preruseni za 1 s
					++program_counter;
					break;
				
				case 1:
					seconds = 0;
					P6OUT |= fans[ 0 ];   // Zapnu fan 1
					++program_counter;
					break;

				case 2:
					// Cekam na casovac
					break;

				case 3:
					seconds = 0;
					P6OUT = P6OUT & ~ fans[ 0 ];   // Vypnu fan 1
					P6OUT |= fans[ 1 ];            // Zapnu fan 2
					++program_counter;
					break;

				case 4:
					// Cekam na casovac
					break;

				case 5:
					seconds = 0;
					P6OUT = P6OUT & ~ fans[ 1 ];   // Vypnu fan 2
					P6OUT |= fans[ 2 ];            // Zapnu fan 3
					++program_counter;
					break;

				case 6:
					// Cekam na casovac
					break;

				case 7:
					seconds = 0;
					P6OUT = P6OUT & ~ fans[ 2 ];   // Vypnu fan 3
					P6OUT |= fans[ 0 ];            // Zapnu fan 1
					program_counter = 1;
					break;
			}
			break;


		// Pri chybe zapnu dalsi fan
		case 'B':
			switch ( program_counter ) {
				case 0:
					P6OUT |= fans[ 0 ];   // Zapnu fan 1
					CCTL0 = CCIE;         // Zapnu casovac
					CCR0 += 0x8000;       // Nastavim preruseni za 1 s
					++program_counter;
					break;
				
				case 1:
					// Cekam na roztoceni ventilatoru
					break;

				case 2:
					CCTL0 = ! CCIE;         // Vypnu casovac
					++program_counter;
					break;

				case 3:
					// Testuji, zda fan 1 funguje
					if ( ! fan_is_OK( 0 ) ) {
						++program_counter;
						P6OUT = P6OUT & ~ fans[ 0 ];   // Vypnu fan 1
						P6OUT |= fans[ 1 ];   // Zapnu fan 2
						seconds = 0;
						CCTL0 = CCIE;         // Zapnu casovac
						CCR0 += 0x8000;       // Nastavim preruseni za 1 s
					}
					break;

				case 4:
					// Cekam na roztoceni ventilatoru
					break;

				case 5:
					CCTL0 = ! CCIE;         // Vypnu casovac
					++program_counter;
					break;

				case 6:
					// Testuji, zda fan 2 funguje
					if ( ! fan_is_OK( 1 ) ) {
						++program_counter;
						P6OUT = P6OUT & ~ fans[ 1 ];   // Vypnu fan 2
						P6OUT |= fans[ 2 ];   // Zapnu fan 3
						seconds = 0;
						CCTL0 = CCIE;         // Zapnu casovac
						CCR0 += 0x8000;       // Nastavim preruseni za 1 s
					}
					break;

				case 7:
					// Cekam na roztoceni ventilatoru
					break;

				case 8:
					CCTL0 = ! CCIE;         // Vypnu casovac
					++program_counter;
					break;

				case 9:
					// Testuji, zda fan 3 funguje
					if ( ! fan_is_OK( 2 ) ) {
						P6OUT = P6OUT & ~ fans[ 2 ];   // Vypnu fan 3
						// vse selhalo...
					}
					break;
			}
			break;
	}
}


// Osetreni ubehnuti jedne s
interrupt ( TIMERA0_VECTOR ) Timer_A ( void ) {
	CCR0 += 0x8000;     // nastav po kolika ticich (32768 = 0x8000, tj. za 1 s) ma dojit k dalsimu preruseni

	++seconds;

	if ( running_program_id == 'A' && seconds >= A_DELAY ) {
		++ program_counter;
	}


	if ( running_program_id == 'B' && seconds >= B_DELAY ) {
		++ program_counter;
	}
}
