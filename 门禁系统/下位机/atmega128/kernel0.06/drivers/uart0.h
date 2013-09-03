#ifndef  UART0_H
#define  UART0_H
/////////////////////////////////////////////////
//            MCUʱ��Ƶ�ʶ���                  //
/////////////////////////////////////////////////
#define  CPU_CLK_FREQ 7372800L     //ϵͳʱ��7.3728M
/////////////////////////////////////////////////
//              uart0��������                  //
/////////////////////////////////////////////////
//��ʼ����Ĭ��Ϊ���ж�
//������0�����ж�
#define Enable_UART0_RX(void)  UCSR0B |= (1<<RXCIE0)  
//��ֹ����0�����ж�        
#define Disable_UART0_RX(void) UCSR0B &= ~(1<<RXCIE0)

//�������ݻ��涨��
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

//���ڳ�ʼ������
//baudrate: ���ڲ����ʣ�����9600
void UART0_InitUART( unsigned long baudrate );
//�ӽ��ջ��������һ���ֽ�
unsigned char UART0_ReceiveByte( void );
//��һ���ֽڷ��뷢�ͻ���
void UART0_TransmitByte( unsigned char data );
//����һ��const�ַ��������ͷ�const�ַ��������)
void UART0_Transmitconstr(const unsigned char *str); 
//����һ����const�ַ��� 
void UART0_Transmitstr(unsigned char *str);      
//�жϽ��ջ����Ƿ�Ϊ��
unsigned char UART0_DataInReceiveBuffer( void );

//ˢ�½��ջ���
void UART0_RX_flash(void);
//ˢ�·��ͻ���
void UART_TX_flash(void);

unsigned char if_receive_a_frame(void);
void Reset_receive_frame(void);

#endif
