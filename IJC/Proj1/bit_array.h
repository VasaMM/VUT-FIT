/** HEADER *****************
	bit_array.h
	Reseni IJC-DU1, 23.3.2016
	Autor: Vaclav Martinka, FIT - 1. rocnik BIT
	Prelozeno: gcc 4.9.3

	Definice maker pro praci s bitovym kodem
****************************/


#ifndef bit_array_header
#define bit_array_header

#include "error.h"
#include <stdio.h>

typedef unsigned long *bit_array_t;

#define bit_sizeof(i) (sizeof(i)*8)

#define DU1_GET_BIT(p,i) ((p[((i) / bit_sizeof(p[0]))] & (1lu << ((i) % bit_sizeof(p[0])))) ? 1 : 0)
#define DU1_SET_BIT(p,i,b) (p[((i) / bit_sizeof(p[0]))] = \
	((b) ? (p[((i) / bit_sizeof(p[0]))] | (1lu << ((i) % bit_sizeof(p[0])))) \
	: (p[((i) / bit_sizeof(p[0]))] & (~(1lu << ((i) % bit_sizeof(p[0])))))))


#define ba_create(jmeno_pole, velikost) \
	unsigned long jmeno_pole[(((velikost) / bit_sizeof(unsigned long)) + \
	(((velikost) % bit_sizeof(unsigned long)) ? 2 : 1))] = {velikost, 0} //musi byt o 1 vetsi kvuli pozici 0 s velikosti makra

	
#ifndef USE_INLINE
	
#define ba_size(jmeno_pole) jmeno_pole[0]

#define ba_get_bit(jmeno_pole,index) \
	(((index) < ba_size(jmeno_pole)) \
	? (DU1_GET_BIT(jmeno_pole, (index + bit_sizeof(jmeno_pole[0])))) \
	: (fatal_error("Index %lu je mimo rozsah 0 - %lu!\n", (unsigned long)(index), (unsigned long)ba_size(jmeno_pole)),0))

#define ba_set_bit(jmeno_pole,index,vyraz) \
	(((index) < ba_size(jmeno_pole)) \
	? ((vyraz) ? DU1_SET_BIT(jmeno_pole,(index) + bit_sizeof(jmeno_pole[0]), 1) : DU1_SET_BIT(jmeno_pole,(index) + bit_sizeof(jmeno_pole[0]), 0)) \
	: (fatal_error("Index %lu je mimo rozsah 0 - %lu!\n", (unsigned long)(index), (unsigned long)ba_size(jmeno_pole)),0))
	
	
//if def USE INLINE
#else
	
    inline unsigned ba_size(bit_array_t jmeno_pole){
		return(jmeno_pole[0]);
    }
	
	inline unsigned ba_get_bit(bit_array_t jmeno_pole, unsigned long index){
		if(index < ba_size(jmeno_pole)) {
			return DU1_GET_BIT(jmeno_pole, index + bit_sizeof(jmeno_pole[0]));
		}
		else {
			fatal_error("Index %lu je mimo rozsah 0 - %lu!\n", (unsigned long)index, (unsigned long)ba_size(jmeno_pole));
			return 0;
		}
	}
	
	inline void ba_set_bit(bit_array_t jmeno_pole, unsigned long index, unsigned long vyraz){
		if(vyraz) {
			vyraz = 1;
		}
		else {
			vyraz = 0;
		}
		if(index < ba_size(jmeno_pole)) {
			DU1_SET_BIT(jmeno_pole, index + bit_sizeof(jmeno_pole[0]), vyraz);
		}
		else {
			fatal_error("Index %lu je mimo rozsah 0 - %lu!\n", (unsigned long)index, (unsigned long)ba_size(jmeno_pole));
		}
		return;
	}
//def USE INLINE
#endif

//bit_array_header
#endif
