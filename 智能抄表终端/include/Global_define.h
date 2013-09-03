#ifndef  _GLOBAL_DEFINE_H_
#define	_GLOBAL_DEFINE_H_
#include "71x_lib.h"

#define	DATE_START_YEAR			1900		// 可用的日期开始年
#define	CURRENT_CENTURY_YEAR		2000
#define	DATE_END_YEAR		        2035		// 可用的日期终止年
#define RTC_PRESC 0x8000
#define I2C0_SCL 	0x2000
#define I2C0_SDA 	0x4000
#define I2C0_WP		0x8000
#define BSPI_FLASH 	BSPI1
#define true		1
#define false	0
//读参数命令的控制码
#define CTRL_CODE_READ_PARA		0X01
#define	CTRL_CODE_ACK_READ_PARA	0X81	//对读参数命令的正确应答控制码
#define	CTRL_CODE_NAK_READ_PARA	0XC1	//对读参数命令的错误应答控制码
//读电量命令的控制码
#define	CTRL_CODE_READ_DL			0X02
#define	CTRL_CODE_ACK_READ_DL	0X82	//对读电量命令的正确应答控制码
#define	CTRL_CODE_NAK_READ_DL	0XC2	//对读电量命令的错误应答控制码
//写参数命令的控制码
#define	CTRL_CODE_WRITE_PARA				0X04
#define	CTRL_CODE_ACK_WRITE_PARA		0X84	//写参数命令的正确的控制码
#define   CTRL_CODE_NAK_WRITE_PARA		0XC4	//写参数命令的错误的控制码
//更改波特率命令的控制码
#define	CTRL_CODE_CHANGE_BAUD 			0X0C
#define	CTRL_CODE_ACK_CHANGE_BAUD		0X8C	//更改波特率命令的正确应答
#define	CTRL_CODE_NAK_CHANGE_BAUD		0XCC	//更改波特率命令的错误应答
//红外以及无线通讯中的 id 值

#define DI_TERMINAL_ASSET_NUMBER			0XC034	//读写，采集终端 的资产号
#define DI_METER_485_ADDR_SET				0XE03F	//读写，全部32块表的485地址
#define	DI_METER_ASSET_NUMBER_SET			0XE05F	//读写，全部32块表的资产编号
#define	DI_METER_ADD							0XE031	//只写，增加或修改某块表的资产号和485地址
#define	DI_METER_DELETE						0XE032	//只写，删除表
#define	DI_METER_REPLACE						0XE033	//只写，替换某块表的资产编号和485地址
#define	DI_METER_TOTAL						0XE014	//只读，读取表计总数
#define	DI_DATE								0XC010	//读写，日期
#define	DI_TIME									0XC011	//读写，时间
#define	DI_CYCLE_SHOW_INTERVAL				0XC113	//读写，循显时间，BCD码，　01--99
#define	DI_COLLECTION_INTERVAL				0XE511	//读写，抄表周期
#define	DI_FREEZE_DAY							0XC117	//读写，月冻结日
#define	DI_DATA_CLEAR							0XC119	//只写，数据清零命令
#define DI_RD_DL								0X9010	//抄总电量
#define DI_RD_DLS								0X901F	//抄总电量集合
#define	DL_TYPE_CURRENT_DL					0X00	//抄当前电量
#define	DI_RD_ADDR_485						0X0B	//根据485地址读取单表电量
#define COM_RECV_BUF_SIZE_HW_PC		256		//串口接收缓冲区的大小
#define COM_SEND_BUF_SIZE_HW_PC	256		//串口发送缓冲区的大小
#define COM_RECV_BUF_SIZE_BJ		256		//串口接收缓冲区的大小
#define COM_SEND_BUF_SIZE_BJ		256		//串口发送缓冲区的大小
#define START_CHAR					0X68	//串口通信的起始字符
#define STOP_CHAR					0X16	//串口通信的终止字符
#define MIN_COM_LENGTH			12		//串口通信的最小长度

//错误字类型定义
#define	METER_IS_VALID			0X01
#define	METER_NOT_VALID		0x00

//轮循相关的宏定义
#define	LUNXUN_1_S_NUMBERS_FOR_1_MIN	60	 //60s   一秒钟的1s 的数目 
#define	LENGTH_FOR_485_ADDR					192			//485地址的总长度
#define	LENGTH_FOR_ASSET_NUMBER				192			//资产编号的总长度
#define	NUMBER_40_FOR_25_MS				40	//40*25=1000ms   40 有点大

//更改波特率命令码
#define BAUD_UART_PC_38400                   38400
#define	BAUD_UART_PC_ORDER_38400				0X20

//Timer1  A 输出
#define	TIMER_A_OUTPUT_COMPARE_VALUE	40000
#endif

