#ifndef  _TASK_FLAG_H_
#define _TASK_FLAG_H_

//�����־   ��Ӧ�� Global_Task_Flag
#define  	TASK_FLAG_RX_BJ_BIT_OK						0X00000010	//�д���BJ	���յ�һ����Ч֡��־	
#define  	TASK_FLAG_RX_PC_BIT_OK						0X00000020	//�д���PC	���յ�һ����Ч֡��־	
#define	TASK_FLAG_SET_ASSET_NUMBER_9213			0X00000080	//�����ʲ����
#define	TASK_FLAG_SET_485_ADDRESS_7213				0X00000100	//����485��ַ
#define	TASK_FLAG_KEY_UP_PRESSED					0X00000400	//���ϵļ�����⵽
#define	TASK_FLAG_25MS_TASK							0X00002000	//25 MSִ��һ�ε�����
//��ѭ���õ��ı�־
#define	TASK_FLAG_LUNXUN_PERMIT						0X00008000	//������ѭ��־
#define	TASK_FLAG_LUNXUN_ING							0X00010000	//���ڽ�����ѭ��־

#define	TASK_FLAG_BEGIN_LUNXUN						0X00100000	//��ʼ��ѭ��־
#define	TASK_FLAG_LUNXUN_COMMAND_SEND_COMPLETE	0X00200000	//��ѭ��������,�������ڼ�⵽�ñ�־��ʱ�򣬿�ʼ��ʱֱ���յ���Ƶ�Ӧ������

//�������õ��ı�־  ��Ӧ�� Com_Task_Flag
#define	TASK_FLAG_COM_BJ_RECV_TWO_START_CHAR	       0X00000002	// ����BJ���յ�������START CHAR (0X68)
#define	TASK_FLAG_COM_PC_RECV_TWO_START_CHAR		0X00000004	// ����PC���յ�������START CHAR (0X68)
#define	TASK_FLAG_COM_BJ_SEND_TIME					0X00000010	//����BJ ����ʱ������� 
#define	TASK_FLAG_COM_PC_SEND_TIME					0X00000020	// ����PC ����ʱ������� 
#define	TASK_FLAG_COM_BJ_SEND_COMPLETE				0X00000080	//����BJ ���͵������Ѿ�ȫ������FIFO ,�ҷ������
#define	TASK_FLAG_COM_PC_SEND_COMPLETE				0X00000100	//����PC ���͵������Ѿ�ȫ������FIFO ,�ҷ������


#endif

