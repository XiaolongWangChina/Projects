#ifndef  UART0_H
#define  UART0_H
/////////////////////////////////////////////////
//            MCU时钟频率定义                  //
/////////////////////////////////////////////////
#define  CPU_CLK_FREQ 7372800L     //系统时钟7.3728M
/////////////////////////////////////////////////
//              uart0驱动定义                  //
/////////////////////////////////////////////////
//初始化后默认为关中断
//允许串口0接收中断
#define Enable_UART0_RX(void)  UCSR0B |= (1<<RXCIE0)  
//禁止串口0接收中断        
#define Disable_UART0_RX(void) UCSR0B &= ~(1<<RXCIE0)

//串口数据缓存定义
#define UART0_RX_BUFFER_SIZE 256 /* 1,2,4,8,16,32,64,128 or 256 bytes */
#define UART0_RX_BUFFER_MASK ( UART0_RX_BUFFER_SIZE - 1 )
#define UART0_TX_BUFFER_SIZE 128 /* 1,2,4,8,16,32,64,128 or 256 bytes */
#define UART0_TX_BUFFER_MASK ( UART0_TX_BUFFER_SIZE - 1 )

#if ( UART0_RX_BUFFER_SIZE & UART0_RX_BUFFER_MASK )
#error RX0 buffer size is not a power of 2
#endif

#if ( UART0_TX_BUFFER_SIZE & UART0_TX_BUFFER_MASK )
#error TX0 buffer size is not a power of 2
#endif

//串口初始化函数
//baudrate: 串口波特率，例：9600
void UART0_InitUART( unsigned long baudrate );
//从接收缓存里接收一个字节
unsigned char UART0_ReceiveByte( void );
//将一个字节放入发送缓存
void UART0_TransmitByte( unsigned char data );
//发送一个const字符串（发送非const字符串会出错)
void UART0_Transmitconstr(const unsigned char *str); 
//发送一个非const字符串 
void UART0_Transmitstr(unsigned char *str);      
//判断接收缓存是否为空
unsigned char UART0_DataInReceiveBuffer( void );

//刷新接收缓存
void UART0_RX_flash(void);
//刷新发送缓存
void UART_TX_flash(void);

unsigned char if_receive_a_frame(void);
void Reset_receive_frame(void);

#endif
