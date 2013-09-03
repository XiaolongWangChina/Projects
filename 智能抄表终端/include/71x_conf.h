#ifndef __71x_CONF_H
#define __71x_CONF_H

/* Comment the line below to put the library in release mode */
#define DEBUG
#define inline inline

/*  Main Oscillator Frequency value = 16 Mhz */
#define RCCU_Main_Osc 16000000

/* Comment the lines below corresponding to unwanted peripherals */
#define _BSPI
#define _BSPI0
#define _BSPI1
#define _EIC
#define _FLASH
#define _GPIO
#define _GPIO0
#define _GPIO1
#define _PCU
#define _RCCU
#define _RTC
#define _TIM
#define _TIM1
#define _TIM2
#define _UART
#define _UART0
#define _UART1
#define _UART2
#define _UART3
#define _WDG
#define _IRQVectors

#endif /* __71x_CONF_H */

