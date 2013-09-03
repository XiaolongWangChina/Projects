#ifndef  UART1_H
#define  UART1_H

#include "../include.h"

//MCU时钟频率定义
#define  CPU_CLK_FREQ 7372800L     //系统时钟7.3728M

//定义串行接收和发送中断向量
//UART1_RX_interrupt 为RX结束中断
//UART1_TX_interrupt 为USART数据寄存器空中断
#pragma interrupt_handler UART1_RX_interrupt:19 UART1_TX_interrupt:20

//初始化后默认为关中断
//允许串口1接收中断
#define Enable_UART1_RX()  UCSR1B |= (1<<RXCIE0)  
//禁止串口1接收中断        
#define Disable_UART1_RX() UCSR1B &= ~(1<<RXCIE0)

//串口数据缓存定义
#define UART1_RX_BUFFER_SIZE 256 /* 1,2,4,8,16,32,64,128 or 256 bytes */
#define UART1_RX_BUFFER_MASK ( UART1_RX_BUFFER_SIZE - 1 )
#define UART1_TX_BUFFER_SIZE 128 /* 1,2,4,8,16,32,64,128 or 256 bytes */
#define UART1_TX_BUFFER_MASK ( UART1_TX_BUFFER_SIZE - 1 )

#if ( UART1_RX_BUFFER_SIZE & UART1_RX_BUFFER_MASK )
#error RX1 buffer size is not a power of 2
#endif

#if ( UART1_TX_BUFFER_SIZE & UART1_TX_BUFFER_MASK )
#error TX1 buffer size is not a power of 2
#endif

//串口初始化函数
//baudrate: 串口波特率，例：9600
void UART1_InitUART( unsigned long baudrate );
//从接收缓存里接收一个字节
unsigned char UART1_ReceiveByte( void );
//将一个字节放入发送缓存
void UART1_TransmitByte( unsigned char data );
//发送一个const字符串（发送非const字符串会出错)
void UART1_Transmitconstr(const unsigned char *str); 
//发送一个非const字符串 
void UART1_Transmitstr(unsigned char *str);      
//判断接收缓存是否为空
unsigned char UART1_DataInReceiveBuffer( void );

int UART1_read(unsigned char *buf,int nbytes);
int UART1_write(unsigned char *buf,int nbytes);

//刷新接收缓存
void UART1_RX_flash(void);
//刷新发送缓存
void UART1_TX_flash(void);

#endif
