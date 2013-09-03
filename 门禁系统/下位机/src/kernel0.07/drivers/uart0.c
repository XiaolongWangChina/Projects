#include "uart0.h"
#include "../include.h"

#define RECEIVE_SIZE 100

#ifndef uchar
#define uchar unsigned char
#endif

/////////////////////////////////////////////////
//                uart0驱动                    //
/////////////////////////////////////////////////

//静态全局变量
static unsigned char UART0_RxBuf[UART0_RX_BUFFER_SIZE];
static volatile unsigned char UART0_RxHead;
static volatile unsigned char UART0_RxTail;
static unsigned char UART0_TxBuf[UART0_TX_BUFFER_SIZE];
static volatile unsigned char UART0_TxHead;
static volatile unsigned char UART0_TxTail;
static volatile unsigned char UART0_Rx_flag = 0;
static volatile unsigned char command_valid = 0;
static volatile unsigned char command=0x00;// by sprife
static volatile unsigned char data_FCS = 0;

//从串口接收有效数据缓冲区
static unsigned char receive_data[125];
static unsigned char send_data[125];

int receive_length = 0;
int receive_count = 0;
int receive_a_frame = 0;
int i;

static unsigned char UART0_485_TxBuf[]={0xBB,0xFF,0x06,0x00,0xA1,0x00,0x00,0x00,0x00,0xFF};

//串口初始化函数
void UART0_InitUART( unsigned long baudrate )
{
    unsigned char x;
	
	DDRE |= BM(DOOR_TXD0);
	DDRE &= ~BM(DOOR_RXD0);
	PORTE |= BM(DOOR_RXD0);
    UBRR0H = (unsigned char)(CPU_CLK_FREQ/(16*baudrate) - 1) >>8;         //设置波特率
    UBRR0L = (unsigned char)(CPU_CLK_FREQ/(16*baudrate) - 1) & 0xFF;

    UCSR0B = ( (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0) );  //允许串口接收、发送和允许接收中断
    UCSR0C = ( (1<<UCSZ01) | (1<<UCSZ00) );
	////1位停止位,8位数据位
	//不能影响其他的设置
	DDRB |= BM(DOOR_485DIR);
	PORTB &= ~BM(DOOR_485DIR);
	
    x = 0;              //初始化数据缓存
    UART0_RxTail = x;
    UART0_RxHead = x;
    UART0_TxTail = x;
    UART0_TxHead = x;
}

//串口接收中断处理函数
SIGNAL(SIG_UART0_RECV)//18
{
    unsigned char data;
    unsigned char tmphead;
    data = UDR0;     //读接收数据
    tmphead = ( UART0_RxHead + 1 ) & UART0_RX_BUFFER_MASK;    //计算缓存索引
    UART0_RxHead = tmphead;         //保存新的缓存索引
    if ( tmphead == UART0_RxTail )
    {
        return ;//这里加入缓存溢出处理
    }
    UART0_RxBuf[tmphead] = data;   //把接收数据保存到接收缓存
	//置位
	UART0_Rx_flag = 1;
	receive_count ++;
	if(receive_count >= 15){
		receive_count = 0;
		receive_a_frame = 1;
	}

}

//串行发送中断处理函数
//发送中断是要修改的
SIGNAL(SIG_USART0_DATA)//19
{
    unsigned char tmptail;
	PORTB |= BM(DOOR_485DIR);
	//一定要加延时，
	//该延时是等待485总线稳定
	halWait(10);

	if ( UART0_TxHead != UART0_TxTail )  //检查是否发送缓存里的所以数据都已经发送完毕
    {
        tmptail = ( UART0_TxTail + 1 ) & UART0_TX_BUFFER_MASK;       //计算缓存索引
        UART0_TxTail = tmptail;     //保存新的缓存索引
        UDR0 = UART0_TxBuf[tmptail];     //开始发送
        UCSR0B &= ~(1<<UDRIE0);       //关UDRE中断
    }
    else
    {
        UCSR0B &= ~(1<<UDRIE0);       //关UDRE中断 
    }

	//一定要加延时，
	halWait(50);
	PORTB &= ~BM(DOOR_485DIR);
}

//从接收缓存里接收一个字节
unsigned char UART0_ReceiveByte( void )
{
    unsigned char tmptail;

    while ( UART0_RxHead == UART0_RxTail ); //等待接收数据
        
    tmptail = ( UART0_RxTail + 1 ) & UART0_RX_BUFFER_MASK;    //计算缓存索引
    UART0_RxTail = tmptail;       //保存新的缓存索引
    return UART0_RxBuf[tmptail]; 
}

//将一个字节放入发送缓存
void UART0_TransmitByte( unsigned char data )
{
    unsigned char tmphead;
    tmphead = ( UART0_TxHead + 1 ) & UART0_TX_BUFFER_MASK;   //计算缓存索引

    while ( tmphead == UART0_TxTail );       //等待有空地缓存空间
        
    UART0_TxBuf[tmphead] = data;     //保存数据到缓存
    UART0_TxHead = tmphead;     //保存新的缓存索引
    UCSR0B |= (1<<UDRIE0); //开 UDRE 中断
}

void UART0_Transmitconstr(const unsigned char *str)    //发送一个const字符串（发送非const字符串会出错)
{
    unsigned char i;
    unsigned char max = strlen(str);
    for(i = 0; i < max; i++)
    {
        UART0_TransmitByte(str[i]);
    }   
}

void UART0_Transmitstr(unsigned char *str)    //发送一个字符串
{
    unsigned char i;
    unsigned char max = strlen(str);
    for(i = 0; i < max; i++)
    {
        UART0_TransmitByte(str[i]);
    }   
}

//判断接收缓存是否为空
unsigned char UART0_DataInReceiveBuffer( void )
{
    return ( UART0_RxHead != UART0_RxTail ); //返回0表示接收缓存已空
}

/******************************************
*
*  工具函数
*
*
******************************************/
int UART0_read(unsigned char *buf,int nbytes){
	int i = 0;
	for(i = 0;i < nbytes;i ++){
		buf[i] = UART0_ReceiveByte();
	}
	return i;
}
int UART0_write(unsigned char *buf,int nbytes){
	int i = 0;
	
	for(i = 0;i < nbytes;i ++){
		UART0_TransmitByte(buf[i]);
	}
	
	return i;
}


//刷新接收缓存
void UART0_RX_flash(){
    UART0_RxTail = 0;
    UART0_RxHead = 0;
}
//刷新发送缓存
void UART_TX_flash(){
    UART0_TxTail = 0;
    UART0_TxHead = 0;
}

unsigned char if_receive_a_frame(){
	return receive_a_frame ;
}
void Reset_receive_frame(){
	receive_a_frame = 0;
}