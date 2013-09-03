#ifndef _LED_H_
#define _LED_H_

#include "../include.h"

// LED初始化
#define LED_init()						\
	do { 							\
		DDRD |= BM(DLED1) | BM(DLED2) | BM(DLED3); 	\
		PORTD |= BM(DLED1) | BM(DLED2) | BM(DLED3); 	\
		DDRE |= BM(DLED4); 				\
		PORTE |= BM(DLED4); 				\
		DDRF |= BM(DLED5); 				\
		PORTF |= BM(DLED5); 				\
	} while (0)

// Clear LED
#define CLR_DLED5()                  (PORTF |= BM(DLED5))
#define CLR_DLED4()                  (PORTE |= BM(DLED4))
#define CLR_DLED3()                  (PORTD |= BM(DLED3))
#define CLR_DLED2()                  (PORTD |= BM(DLED2))
#define CLR_DLED1()                  (PORTD |= BM(DLED1))

// Set LED
#define SET_DLED5()                  (PORTF &= ~BM(DLED5))
#define SET_DLED4()                  (PORTE &= ~BM(DLED4))
#define SET_DLED3()                  (PORTD &= ~BM(DLED3))
#define SET_DLED2()                  (PORTD &= ~BM(DLED2))
#define SET_DLED1()                  (PORTD &= ~BM(DLED1))

#endif
