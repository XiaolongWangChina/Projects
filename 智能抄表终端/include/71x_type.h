#ifndef _71x_type_H
#define _71x_type_H

typedef unsigned long   u32;
typedef unsigned short  u16;
typedef unsigned char   u8;
typedef volatile unsigned long   vu32;
typedef volatile unsigned short  vu16;
typedef volatile unsigned char   vu8;
typedef volatile signed long   vs32;
typedef volatile signed short  vs16;
typedef volatile signed char   vs8;
typedef enum { FALSE= 0, TRUE  = !FALSE } bool;
typedef enum { RESET = 0, SET   = !RESET } FlagStatus;
typedef enum { DISABLE = 0, ENABLE  = !DISABLE} FunctionalState;
typedef enum { INDIRECT = 0, DIRECT  = !INDIRECT} RegisterAccess;

#endif /* _71x_type_H */

