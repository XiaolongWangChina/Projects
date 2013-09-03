/*************************************************************************************************
*File Name   : 71x_lib.h
*Description : ����Ҫ�õ��ĵײ��������ļ�
*Node(s)     : ����ļ��Ǻ�71x_conf�ļ����ʹ�õ�
*              �����ļ�
**************************************************************************************************/
#ifndef __71x_LIB_H
#define __71x_LIB_H

#include "71x_map.h"
#include "71x_conf.h"
#include <string.h>

#include "71x_type.h"
#include "Task_Flag.h"
#include "Global_define.h"
#include "Spi_Flash.h"
#include "Extern_Var.h"
#include "pub_tool.h"
#include "led.h"
#include "font.h"
#include "lcd.h"
#include "tim.h"
#include "lunxun.h"
#include "uart.h"
#include "key.h"
#include "read_dl.h"


#ifdef _BSPI
  #include "bspi.h"
#endif

#ifdef _EIC
  #include "eic.h"
#endif

#ifdef _FLASH
  #include "flash.h"
#endif

#ifdef _GPIO
  #include "gpio.h"
#endif

#ifdef _RCCU
  #include "rccu.h"
#endif

#ifdef _RTC
  #include "rtc.h"
#endif

#ifdef _TIM
  #include "tim.h"
#endif

#ifdef _UART
  #include "uart.h"
#endif

#ifdef _WDG
  #include "wdg.h"
#endif

#ifdef DEBUG
  void debug(void);
#endif

#endif /* __71x_LIB_H */

