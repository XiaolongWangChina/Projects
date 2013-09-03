#include "uart0.h"
#include "../include.h"

#define RECEIVE_SIZE 100

#ifndef uchar
#define uchar unsigned char
#endif

/////////////////////////////////////////////////
//                uart0����                    //
/////////////////////////////////////////////////

//��̬ȫ�ֱ���
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

//�Ӵ��ڽ�����Ч���ݻ�����
static unsigned char receive_data[125];
static unsigned char send_data[125];

int receive_length = 0;
int receive_count = 0;
int receive_a_frame = 0;
int i;

static unsigned char UART0_485_TxBuf[]={0xBB,0xFF,0x06,0x00,0xA1,0x00,0x00,0x00,0x00,0xFF};

//���ڳ�ʼ������
void UART0_InitUART( unsigned long baudrate )
{
    unsigned char x;
	
	DDRE |= BM(DOOR_TXD0);
	DDRE &= ~BM(DOOR_RXD0);
	PORTE |= BM(DOOR_RXD0);
    UBRR0H = (unsigned char)(CPU_CLK_FREQ/(16*baudrate) - 1) >>8;         //���ò�����
    UBRR0L = (unsigned char)(CPU_CLK_FREQ/(16*baudrate) - 1) & 0xFF;

    UCSR0B = ( (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0) );  //�����ڽ��ա����ͺ���������ж�
    UCSR0C = ( (1<<UCSZ01) | (1<<UCSZ00) );
	////1λֹͣλ,8λ����λ
	//����Ӱ������������
	DDRB |= BM(DOOR_485DIR);
	PORTB &= ~BM(DOOR_485DIR);
	
    x = 0;              //��ʼ�����ݻ���
    UART0_RxTail = x;
    UART0_RxHead = x;
    UART0_TxTail = x;
    UART0_TxHead = x;
}

//���ڽ����жϴ�����
SIGNAL(SIG_UART0_RECV)//18
{
    unsigned char data;
    unsigned char tmphead;
    data = UDR0;     //����������
    tmphead = ( UART0_RxHead + 1 ) & UART0_RX_BUFFER_MASK;    //���㻺������
    UART0_RxHead = tmphead;         //�����µĻ�������
    if ( tmphead == UART0_RxTail )
    {
        return ;//������뻺���������
    }
    UART0_RxBuf[tmphead] = data;   //�ѽ������ݱ��浽���ջ���
	//��λ
	UART0_Rx_flag = 1;
	receive_count ++;
	if(receive_count >= 15){
		receive_count = 0;
		receive_a_frame = 1;
	}

}

//���з����жϴ�����
//�����ж���Ҫ�޸ĵ�
SIGNAL(SIG_USART0_DATA)//19
{
    unsigned char tmptail;
	PORTB |= BM(DOOR_485DIR);
	//һ��Ҫ����ʱ��
	//����ʱ�ǵȴ�485�����ȶ�
	halWait(10);

	if ( UART0_TxHead != UART0_TxTail )  //����Ƿ��ͻ�������������ݶ��Ѿ��������
    {
        tmptail = ( UART0_TxTail + 1 ) & UART0_TX_BUFFER_MASK;       //���㻺������
        UART0_TxTail = tmptail;     //�����µĻ�������
        UDR0 = UART0_TxBuf[tmptail];     //��ʼ����
        UCSR0B &= ~(1<<UDRIE0);       //��UDRE�ж�
    }
    else
    {
        UCSR0B &= ~(1<<UDRIE0);       //��UDRE�ж� 
    }

	//һ��Ҫ����ʱ��
	halWait(50);
	PORTB &= ~BM(DOOR_485DIR);
}

//�ӽ��ջ��������һ���ֽ�
unsigned char UART0_ReceiveByte( void )
{
    unsigned char tmptail;

    while ( UART0_RxHead == UART0_RxTail ); //�ȴ���������
        
    tmptail = ( UART0_RxTail + 1 ) & UART0_RX_BUFFER_MASK;    //���㻺������
    UART0_RxTail = tmptail;       //�����µĻ�������
    return UART0_RxBuf[tmptail]; 
}

//��һ���ֽڷ��뷢�ͻ���
void UART0_TransmitByte( unsigned char data )
{
    unsigned char tmphead;
    tmphead = ( UART0_TxHead + 1 ) & UART0_TX_BUFFER_MASK;   //���㻺������

    while ( tmphead == UART0_TxTail );       //�ȴ��пյػ���ռ�
        
    UART0_TxBuf[tmphead] = data;     //�������ݵ�����
    UART0_TxHead = tmphead;     //�����µĻ�������
    UCSR0B |= (1<<UDRIE0); //�� UDRE �ж�
}

void UART0_Transmitconstr(const unsigned char *str)    //����һ��const�ַ��������ͷ�const�ַ��������)
{
    unsigned char i;
    unsigned char max = strlen(str);
    for(i = 0; i < max; i++)
    {
        UART0_TransmitByte(str[i]);
    }   
}

void UART0_Transmitstr(unsigned char *str)    //����һ���ַ���
{
    unsigned char i;
    unsigned char max = strlen(str);
    for(i = 0; i < max; i++)
    {
        UART0_TransmitByte(str[i]);
    }   
}

//�жϽ��ջ����Ƿ�Ϊ��
unsigned char UART0_DataInReceiveBuffer( void )
{
    return ( UART0_RxHead != UART0_RxTail ); //����0��ʾ���ջ����ѿ�
}

/******************************************
*
*  ���ߺ���
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


//ˢ�½��ջ���
void UART0_RX_flash(){
    UART0_RxTail = 0;
    UART0_RxHead = 0;
}
//ˢ�·��ͻ���
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