#ifndef __GPIO_H
#define __GPIO_H

#include "71x_map.h"


/*  PC  , 表计所对应的控制端口*/
#define	GPIO_PC_CTRL		GPIO0
#define	GPIO_BJ_CTRL		GPIO0

/* LCD  的四个引脚所对应的I/O 端口*/
#define	GPIO_LCD_CS		GPIO1
#define	GPIO_LCD_A0		GPIO1
#define	GPIO_LCD_SCL		GPIO1
#define	GPIO_LCD_SI		GPIO1
/* LCD 背景灯的控制引脚*/
#define	GPIO_LCD_BACK_LIGHT_CTRL		GPIO1

/* 3 个LED (上行,下行,报警) 所对应的I/O 端口*/
#define	GPIO_LED_UP			GPIO1
#define	GPIO_LED_DOWN			GPIO1
#define	GPIO_LED_WARNING		GPIO1


/* 按键所对应的I/O 端口 */
#define	GPIO_KEY_UP				GPIO0
#define	GPIO_KEY_DOWN				GPIO1

/* Timer1 的输出比较功能所对应的I/O 端口 */
#define	GPIO_TIMER_1_OCMP			GPIO1

typedef enum
{
  GPIO_HI_AIN_TRI, /*HIGH IMPEDANCE ANALOG INPUT TRISTATE*/
  GPIO_IN_TRI_TTL, /* INPUT TRISTATE TTL*/
  GPIO_IN_TRI_CMOS,/*INPUT TRSISTATE CMOS*/
  GPIO_IPUPD_WP,   /*INPUT PULL UP/PULL DOWN WEAK PUSH PULL*/
  GPIO_OUT_OD,     /* OUTPUT OPEN DRAIN*/
  GPIO_OUT_PP,     /* OUTPUT PUSH PULL*/
  GPIO_AF_OD,      /*ALTERNATE FUNCTION OPEN DRAIN */
  GPIO_AF_PP       /* ALTERNATE FUNCTION PUSH PULL*/
} GpioPinMode_TypeDef;

#define GPIO_LSB  0x00
#define GPIO_MSB  0x08

/*******************************************************************************
* Function Name  : GPIO_Config
* Description    : Configure the GPIO port pins.
* Input 1        : GPIOx (x can be 0,1 or 2) the desired port.
* Input 2        : Port_Pins : pins placements.
* Input 3        : Pins Mode(GPIO_HI_AIN_TRI,GPIO_IN_TRI_TTL,GPIO_IN_TRI_CMOS,
*                   GPIO_IPUPD_WP,GPIO_OUT_OD,GPIO_OUT_PP,GPIO_AF_OD,GPIO_AF_PP).
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Config (GPIO_TypeDef *GPIOx, u16 Port_Pins, GpioPinMode_TypeDef GPIO_Mode);
/*******************************************************************************
* Function Name  : GPIO_WordRead
* Description    : Read the desired port word value.
* Input 1        : Selected GPIO port(GPIO1, GPIO1, GPIO2).
* Output         : None
* Return         : The selected PD register value.
*******************************************************************************/
inline u16 GPIO_WordRead(GPIO_TypeDef *GPIOx)
{
  return GPIOx->PD;
}
/*******************************************************************************
* Function Name  : GPIO_WordWrite
* Description    : Write word value to the selected PD register.
* Input 1        : Selected GPIO port(GPIO1, GPIO1, GPIO2).
* Input 2        : Value.
* Output         : None
* Return         : None.
*******************************************************************************/
inline void GPIO_WordWrite(GPIO_TypeDef *GPIOx, u16 Port_Val)
{
  EIC->ICR &= ~0x0001;
  GPIOx->PD = Port_Val;
  EIC->ICR |= 0x0001;
}

#endif /* __GPIO_H */
