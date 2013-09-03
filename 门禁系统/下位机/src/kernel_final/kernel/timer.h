#ifndef TICK_H
#define TICK_H

#include "../include.h"
//8MHZ
#define CLOCK_FREQ                      (8000000)      
#define TICKS_PER_SECOND                (50)
#define TICK_PRESCALE_VALUE             (8)

typedef DWORD TICK;

#define TICK_SECOND             ((TICK)TICKS_PER_SECOND)


#define TICK_TEMP_VALUE   (65535-( (CLOCK_FREQ/TICK_PRESCALE_VALUE)/TICKS_PER_SECOND))

#if TICK_TEMP_VALUE < 5535
	#error timer set wrong 
#endif

#define TICK_COUNTER_HIGH       ((TICK_TEMP_VALUE >> 8) & 0xff)

#define TICK_COUNTER_LOW        (TICK_TEMP_VALUE & 0xff)

#if (TICK_PRESCALE_VALUE == 1)
    #define TIMER_PRESCALE  (1)
#elif ( TICK_PRESCALE_VALUE == 8 )
    #define TIMER_PRESCALE  (2)
#elif ( TICK_PRESCALE_VALUE == 64 )
    #define TIMER_PRESCALE  (3)
#elif ( TICK_PRESCALE_VALUE == 256 )
    #define TIMER_PRESCALE  (4)
#elif ( TICK_PRESCALE_VALUE == 1024 )
    #define TIMER_PRESCALE  (5)
#else
    #error Invalid _TICK_PRESCALE_VALUE_ specified.
#endif

extern TICK TickCount;

void TickInit(void);

TICK TickGet(void);

#define TickGetDiff(a, b)       (a-b)

void TickUpdate(void);
void TickDisable(void);
void TickEnable(void);

//¶¨Ê±Æ÷0
void Tick0Init(void);

#endif
