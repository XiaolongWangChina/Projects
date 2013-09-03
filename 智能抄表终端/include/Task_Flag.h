#ifndef  _TASK_FLAG_H_
#define _TASK_FLAG_H_

//任务标志   对应于 Global_Task_Flag
#define  	TASK_FLAG_RX_BJ_BIT_OK						0X00000010	//有串口BJ	接收到一个有效帧标志	
#define  	TASK_FLAG_RX_PC_BIT_OK						0X00000020	//有串口PC	接收到一个有效帧标志	
#define	TASK_FLAG_SET_ASSET_NUMBER_9213			0X00000080	//设置资产编号
#define	TASK_FLAG_SET_485_ADDRESS_7213				0X00000100	//设置485地址
#define	TASK_FLAG_KEY_UP_PRESSED					0X00000400	//向上的键被检测到
#define	TASK_FLAG_25MS_TASK							0X00002000	//25 MS执行一次的任务
//轮循所用到的标志
#define	TASK_FLAG_LUNXUN_PERMIT						0X00008000	//允许轮循标志
#define	TASK_FLAG_LUNXUN_ING							0X00010000	//正在进行轮循标志

#define	TASK_FLAG_BEGIN_LUNXUN						0X00100000	//开始轮循标志
#define	TASK_FLAG_LUNXUN_COMMAND_SEND_COMPLETE	0X00200000	//轮循命令发送完毕,主程序在监测到该标志的时候，开始计时直到收到表计的应答数据

//串口所用到的标志  对应于 Com_Task_Flag
#define	TASK_FLAG_COM_BJ_RECV_TWO_START_CHAR	       0X00000002	// 串口BJ接收到了两个START CHAR (0X68)
#define	TASK_FLAG_COM_PC_RECV_TWO_START_CHAR		0X00000004	// 串口PC接收到了两个START CHAR (0X68)
#define	TASK_FLAG_COM_BJ_SEND_TIME					0X00000010	//串口BJ 发送时间计数器 
#define	TASK_FLAG_COM_PC_SEND_TIME					0X00000020	// 串口PC 发送时间计数器 
#define	TASK_FLAG_COM_BJ_SEND_COMPLETE				0X00000080	//串口BJ 发送的数据已经全部送入FIFO ,且发送完成
#define	TASK_FLAG_COM_PC_SEND_COMPLETE				0X00000100	//串口PC 发送的数据已经全部送入FIFO ,且发送完成


#endif

