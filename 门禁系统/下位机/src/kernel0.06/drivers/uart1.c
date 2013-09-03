#include "uart1.h"
#include "led.h"

#define RECEIVE_SIZE 100
#ifndef uchar
#define uchar unsigned char
#endif

//静态全局变量
static unsigned char UART1_RxBuf[UART1_RX_BUFFER_SIZE];
static volatile unsigned char UART1_RxHead;
static volatile unsigned char UART1_RxTail;
static unsigned char UART1_TxBuf[UART1_TX_BUFFER_SIZE];
static volatile unsigned char UART1_TxHead;
static volatile unsigned char UART1_TxTail;
static volatile unsigned char UART1_Rx_flag = 0;



//串口初始化函数
void UART1_InitUART( unsigned long baudrate )
{
	unsigned char x;

	UBRR1H = (unsigned char)(CPU_CLK_FREQ/(16*baudrate) - 1) >>8;         //设置波特率
	UBRR1L = (unsigned char)(CPU_CLK_FREQ/(16*baudrate) - 1) & 0xFF;

	UCSR1B = ( (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0) );  //允许串口接收、发送和允许接收中断
	UCSR1C = ( (1<<UCSZ01) | (1<<UCSZ00) ); //位停止位,8位数据位
	x = 0;              //初始化数据缓存
	UART1_RxTail = x;
	UART1_RxHead = x;
	UART1_TxTail = x;
	UART1_TxHead = x;
}

//串口接收中断处理函数
SIGNAL(USART1_RX_vect)//30
{
	unsigned char data;
	unsigned char tmphead;
	data = UDR1;     //读接收数据
	tmphead = ( UART1_RxHead + 1 ) & UART1_RX_BUFFER_MASK;    //计算缓存索引
	UART1_RxHead = tmphead;         //保存新的缓存索引
	if ( tmphead == UART1_RxTail )
	{
		return ;//这里加入缓存溢出处理
	}
	UART1_RxBuf[tmphead] = data;   //把接收数据保存到接收缓存
	//置位
	UART1_Rx_flag = 1;
}

//串行发送中断处理函数
//发送中断是要修改的
SIGNAL(USART1_UDRE_vect)//31
{
	unsigned char tmptail;

	if ( UART1_TxHead != UART1_TxTail )  //检查是否发送缓存里的所以数据都已经发送完毕
	{
		tmptail = ( UART1_TxTail + 1 ) & UART1_TX_BUFFER_MASK;       //计算缓存索引
		UART1_TxTail = tmptail;     //保存新的缓存索引
		UDR1 = UART1_TxBuf[tmptail];     //开始发送
	}
	else
	{
		UCSR1B &= ~(1<<UDRIE0);       //关UDRE中断 
	}
}

//从接收缓存里接收一个字节
unsigned char UART1_ReceiveByte( void )
{
	unsigned char tmptail;

	while ( UART1_RxHead == UART1_RxTail ); //等待接收数据

	tmptail = ( UART1_RxTail + 1 ) & UART1_RX_BUFFER_MASK;    //计算缓存索引
	UART1_RxTail = tmptail;       //保存新的缓存索引
	return UART1_RxBuf[tmptail]; 
}

//将一个字节放入发送缓存
void UART1_TransmitByte( unsigned char data )
{
	unsigned char tmphead;
	tmphead = ( UART1_TxHead + 1 ) & UART1_TX_BUFFER_MASK;   //计算缓存索引

	while ( tmphead == UART1_TxTail );       //等待有空地缓存空间

	UART1_TxBuf[tmphead] = data;     //保存数据到缓存
	UART1_TxHead = tmphead;     //保存新的缓存索引
	UCSR1B |= (1<<UDRIE0); //开 UDRE 中断
}

void UART1_Transmitconstr(const unsigned char *str)    //发送一个const字符串（发送非const字符串会出错)
{
	unsigned char i;
	unsigned char max = strlen(str);
	for(i = 0; i < max; i++)
	{
		UART1_TransmitByte(str[i]);
	}   
}

void UART1_Transmitstr(unsigned char *str)    //发送一个字符串
{
	unsigned char i;
	unsigned char max = strlen(str);
	for(i = 0; i < max; i++)
	{
		UART1_TransmitByte(str[i]);
	}   
}

//判断接收缓存是否为空
unsigned char UART1_DataInReceiveBuffer( void )
{
	return ( UART1_RxHead != UART1_RxTail ); //返回0表示接收缓存已空
}

/******************************************
 *
 *  工具函数
 *
 *
 ******************************************/
int UART1_read(unsigned char *buf,int nbytes){
	int i = 0;
	for(i = 0;i < nbytes;i ++){
		buf[i] = UART1_ReceiveByte();
	}
	return i;
}
int UART1_write(unsigned char *buf,int nbytes){
	int i = 0;
	for(i = 0;i < nbytes;i ++){
		UART1_TransmitByte(buf[i]);
	}
	return i;
}
//刷新接收缓存
void UART1_RX_flash(){
	UART1_RxTail = 0;
	UART1_RxHead = 0;
}
//刷新发送缓存
void UART1_TX_flash(){
	UART1_TxTail = 0;
	UART1_TxHead = 0;
}

