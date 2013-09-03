#ifndef  _GLOBAL_DEFINE_H_
#define	_GLOBAL_DEFINE_H_
#include "71x_lib.h"

#define	DATE_START_YEAR			1900		// ���õ����ڿ�ʼ��
#define	CURRENT_CENTURY_YEAR		2000
#define	DATE_END_YEAR		        2035		// ���õ�������ֹ��
#define RTC_PRESC 0x8000
#define I2C0_SCL 	0x2000
#define I2C0_SDA 	0x4000
#define I2C0_WP		0x8000
#define BSPI_FLASH 	BSPI1
#define true		1
#define false	0
//����������Ŀ�����
#define CTRL_CODE_READ_PARA		0X01
#define	CTRL_CODE_ACK_READ_PARA	0X81	//�Զ������������ȷӦ�������
#define	CTRL_CODE_NAK_READ_PARA	0XC1	//�Զ���������Ĵ���Ӧ�������
//����������Ŀ�����
#define	CTRL_CODE_READ_DL			0X02
#define	CTRL_CODE_ACK_READ_DL	0X82	//�Զ������������ȷӦ�������
#define	CTRL_CODE_NAK_READ_DL	0XC2	//�Զ���������Ĵ���Ӧ�������
//д��������Ŀ�����
#define	CTRL_CODE_WRITE_PARA				0X04
#define	CTRL_CODE_ACK_WRITE_PARA		0X84	//д�����������ȷ�Ŀ�����
#define   CTRL_CODE_NAK_WRITE_PARA		0XC4	//д��������Ĵ���Ŀ�����
//���Ĳ���������Ŀ�����
#define	CTRL_CODE_CHANGE_BAUD 			0X0C
#define	CTRL_CODE_ACK_CHANGE_BAUD		0X8C	//���Ĳ������������ȷӦ��
#define	CTRL_CODE_NAK_CHANGE_BAUD		0XCC	//���Ĳ���������Ĵ���Ӧ��
//�����Լ�����ͨѶ�е� id ֵ

#define DI_TERMINAL_ASSET_NUMBER			0XC034	//��д���ɼ��ն� ���ʲ���
#define DI_METER_485_ADDR_SET				0XE03F	//��д��ȫ��32����485��ַ
#define	DI_METER_ASSET_NUMBER_SET			0XE05F	//��д��ȫ��32�����ʲ����
#define	DI_METER_ADD							0XE031	//ֻд�����ӻ��޸�ĳ�����ʲ��ź�485��ַ
#define	DI_METER_DELETE						0XE032	//ֻд��ɾ����
#define	DI_METER_REPLACE						0XE033	//ֻд���滻ĳ�����ʲ���ź�485��ַ
#define	DI_METER_TOTAL						0XE014	//ֻ������ȡ�������
#define	DI_DATE								0XC010	//��д������
#define	DI_TIME									0XC011	//��д��ʱ��
#define	DI_CYCLE_SHOW_INTERVAL				0XC113	//��д��ѭ��ʱ�䣬BCD�룬��01--99
#define	DI_COLLECTION_INTERVAL				0XE511	//��д����������
#define	DI_FREEZE_DAY							0XC117	//��д���¶�����
#define	DI_DATA_CLEAR							0XC119	//ֻд��������������
#define DI_RD_DL								0X9010	//���ܵ���
#define DI_RD_DLS								0X901F	//���ܵ�������
#define	DL_TYPE_CURRENT_DL					0X00	//����ǰ����
#define	DI_RD_ADDR_485						0X0B	//����485��ַ��ȡ�������
#define COM_RECV_BUF_SIZE_HW_PC		256		//���ڽ��ջ������Ĵ�С
#define COM_SEND_BUF_SIZE_HW_PC	256		//���ڷ��ͻ������Ĵ�С
#define COM_RECV_BUF_SIZE_BJ		256		//���ڽ��ջ������Ĵ�С
#define COM_SEND_BUF_SIZE_BJ		256		//���ڷ��ͻ������Ĵ�С
#define START_CHAR					0X68	//����ͨ�ŵ���ʼ�ַ�
#define STOP_CHAR					0X16	//����ͨ�ŵ���ֹ�ַ�
#define MIN_COM_LENGTH			12		//����ͨ�ŵ���С����

//���������Ͷ���
#define	METER_IS_VALID			0X01
#define	METER_NOT_VALID		0x00

//��ѭ��صĺ궨��
#define	LUNXUN_1_S_NUMBERS_FOR_1_MIN	60	 //60s   һ���ӵ�1s ����Ŀ 
#define	LENGTH_FOR_485_ADDR					192			//485��ַ���ܳ���
#define	LENGTH_FOR_ASSET_NUMBER				192			//�ʲ���ŵ��ܳ���
#define	NUMBER_40_FOR_25_MS				40	//40*25=1000ms   40 �е��

//���Ĳ�����������
#define BAUD_UART_PC_38400                   38400
#define	BAUD_UART_PC_ORDER_38400				0X20

//Timer1  A ���
#define	TIMER_A_OUTPUT_COMPARE_VALUE	40000
#endif

