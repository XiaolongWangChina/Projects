#ifndef INCLUDE_H
#define INCLUDE_H
#define  CC2420DB

/*
 * Standard GCC include files for AVR
 */
#include <stdio.h> 
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/signal.h>
#include <avr/iom128.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

/*
 * Common data types
 */
typedef unsigned char		BOOL;

typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;
typedef unsigned long long	QWORD;

typedef unsigned char		UINT8;
typedef unsigned short		UINT16;
typedef unsigned long		UINT32;
typedef unsigned long long	UINT64;

typedef signed char		INT8;
typedef signed short		INT16;
typedef signed long		INT32;
typedef signed long long	INT64;

/*
 * Common values
 */
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef NULL
#define NULL 0
#endif

/*
 * Useful stuff
 */
#define BM(n) (1 << (n))
#define BF(x,b,s) (((x) & (b)) >> (s))
#define MIN(n,m) (((n) < (m)) ? (n) : (m))
#define MAX(n,m) (((n) < (m)) ? (m) : (n))
#define ABS(n) ((n < 0) ? -(n) : (n))

#define NOP() asm volatile ("nop\n\t" ::)

//延时函数
// This sequence uses exactly 8 clock cycle for each round
#define halWait(timeout)		\
	do {				\
		UINT16 _temp = timeout;	\
		do {			\
			NOP();		\
			NOP();		\
			NOP();		\
			NOP();		\
		} while (--_temp);	\
	} while (0)

// General
#define ENABLE_GLOBAL_INT()         do { asm ("sei\n\t" ::); } while (0)
#define DISABLE_GLOBAL_INT()        do { asm ("cli\n\t" ::); } while (0)

/*
 * Dynamic function pointer
 */
typedef void (*VFPTR)(void);


/*
 * 系统中用到的端口
 */
//PORTA

//PORTB
#define DOOR_485DIR		0 // PB.0
#define DKEY0			4 // PB.4
#define DKEY1			5 // PB.5
#define DKEY2			6 // PB.6
#define DKEY3			7 // PB.7

//PORTC

//PORTD
#define DOOR_RXD1		2 // PD.2
#define DOOR_TXD1		3 // PD.3
#define DLED1			4 // PD.4
#define DLED2			5 // PD.5
#define DLED3			6 // PD.6

//PORTE
#define DOOR_RXD0		0 // PE.0
#define DOOR_TXD0		1 // PE.1
#define DLED4			3 // PE.3
#define DKEYINT			4 // PE.4
#define DOOR_IN			5 // PE.5 Input: door is close or open
#define DOOR_OUT		7 // PE.7 Output:door close or open

//PORTF
#define DLED5			2 // PF.2
#define DOORBEEP		3 // PF.3
#define DKEY4			4 // PF.4
#define DKEY5			5 // PF.5
#define DKEY6			6 // PF.6
#define DKEY7			7 // PF.7

#endif
