/*************************************************************************************************
*File Name   : Spi_Flash.c
*Description : 用SPI控制从Flash中读写数据的头文件
*Node(s)     : 这个文件是建立在Bspi和Flash这两个文件的基础上
*              创建文件
**************************************************************************************************/
#include "71x_lib.h" 
#include "spi_flash.h"


/***************************************************************************************************
* FunctionName : InitBSPIFlash
* Description  : 对Flash的初始化                  
* Parameter(s) :     
*                
* Return       : void
***************************************************************************************************/
void InitBSPIFlash (void)
{
  	/* Configure GPI00 on mode Alternate function Push Pull */
  	GPIO_Config ( GPIO0, BSPI1_MISO|BSPI1_MOSI|BSPI1_SCLK, GPIO_AF_PP );
  	/* Configure SCLK & SSN  clock and data lines control */
  	GPIO_Config ( GPIO0, FLASH_CS, GPIO_OUT_PP );
  	/* Initialize BSPI1 device */
  	BSPI_Init ( BSPI1 );
  	/* Configure Baud rate Frequency: ---> APB1/6 */
  	BSPI_ClockDividerConfig (BSPI1, 6);
  	/* Enable BSPI1 */
  	BSPI_Enable ( BSPI1, ENABLE );
  	/* Configure BSPI1 as a Master */
  	BSPI_MasterEnable  ( BSPI1, ENABLE );
  	/* Configure the clock to be active low */
  	BSPI_ClkActiveHigh ( BSPI1, DISABLE );
  	/* Enable capturing the first Data sample on the first edge of SCK */
  	BSPI_ClkFEdge ( BSPI1, DISABLE );
  	/* Set the word length to 8 bit */
  	BSPI_8bLEn ( BSPI1, ENABLE );
}

/***************************************************************************************************
* FunctionName : BPSI_DataSendReceive
* Description  : 从当前地址向Flash中发送数据                  
* Parameter(s) :     
*                DATA:要向Flash中发送的数据
*
* Return       : u8:从Flash中返回的数据
***************************************************************************************************/
u8 BPSI_DataSendReceive(u8 DATA)    
{
  	u8 temp;
  	while(BSPI1->CSR2&BSPI_RFNE)
  	temp = BSPI1->RXR;
	
  	while(!(BSPI1->CSR2&BSPI_TFE));    /*  Wait until the Transmit FIFO is empty */
  
  	BSPI1->TXR = DATA<<8;              /*  Send data to Transmit buffer */
	
  	while(!(BSPI1->CSR2&BSPI_RFF));    /*  Wait until the end of transmission */
	
  	temp = (BSPI1->RXR)>>8;            /*  Read the received data */
  	return temp;                     
}


/***************************************************************************************************
* FunctionName : SPI_Write_161d
* Description  : 通过SPI口向外部的Flash中写数据                 
* Parameter(s) :                 
*                Dst_Addr_161d:要写的Flash中的目的地址
*                Src_Addr     :将要写入到Flash中的数据的源地址
*                Len_Of_Data  :要写入的数据的长度
* Return       : void
***************************************************************************************************/
void SPI_Write_161d(u32 Dst_Addr_161d,u8* Src_Addr,u16 Len_Of_Data)
{
	u32  Temp_Dst_Addr;		//每次循环将要向FLASH中写入的地址
	u16  Temp_Len;			//每次循环将要向FLASH中写入的数据的长度
	u32  i=0;
	u16  Data_Left_Len;		//剩余的还没有写入的数据的长度

	Temp_Dst_Addr = Dst_Addr_161d;
	Temp_Len      = (SPI_FLASH_PAGE_SIZE - Dst_Addr_161d%SPI_FLASH_PAGE_SIZE)<Len_Of_Data? (SPI_FLASH_PAGE_SIZE - Dst_Addr_161d%SPI_FLASH_PAGE_SIZE):Len_Of_Data;
	Data_Left_Len = Len_Of_Data;

	do
	{
		WDG_CntRefresh(); /*刷新看门狗的计数器值*/

   		AT45DB161_Chip_Select_ENABLE;   
		BSPI_ByteSend(OPCODE_MAIN_MEM_TO_BUFFER_1);	 /*发送命令字节*/
		BSPI_ByteSend((Temp_Dst_Addr&0XFF0000)>>16); /*发送第一个地址字节*/
		BSPI_ByteSend((Temp_Dst_Addr&0XFF00)>>8);	 /*发送第二个地址字节*/
		BSPI_ByteSend((Temp_Dst_Addr&0XFF));		 /*发送第三个地址字节*/
    	AT45DB161_Chip_Select_DISABLE;

		for(i=0;i<0X0FFFF;i++);				

		AT45DB161_Chip_Select_ENABLE;   
		BSPI_ByteSend(OPCODE_RAM_TO_MAIN_MEM_THROUGH_BUF_1); /*发送命令字节*/
		BSPI_ByteSend((Temp_Dst_Addr&0XFF0000)>>16);         /*发送第一个地址字节*/
		BSPI_ByteSend((Temp_Dst_Addr&0XFF00)>>8);            /*发送第二个地址字节*/
		BSPI_ByteSend((Temp_Dst_Addr&0XFF));		     /*发送第三个地址字节*/
		for(i=0;i<Temp_Len;i++)
		{
			BSPI_ByteSend(*Src_Addr++);
		}
		AT45DB161_Chip_Select_DISABLE;

		for(i=0;i<0XFFFF;i++);	
		WDG_CntRefresh();      /*刷新看门狗的计数器值*/
		for(i=0;i<0XFFFF;i++);	
		WDG_CntRefresh();	   /*刷新看门狗的计数器值*/
		for(i=0;i<0XFFFF;i++);	
		WDG_CntRefresh();	   /*刷新看门狗的计数器值*/
		for(i=0;i<0XFFFF;i++);	
		WDG_CntRefresh();	   /*刷新看门狗的计数器值*/

		Data_Left_Len  -=Temp_Len;
		Temp_Dst_Addr  += Temp_Len;	/*改变下一次的写入的地址和长度*/
		if(Data_Left_Len >=SPI_FLASH_PAGE_SIZE)
		{
			Temp_Len =SPI_FLASH_PAGE_SIZE;
		}
		else
		{
			Temp_Len =Data_Left_Len;
		}					
	}
	while(Data_Left_Len>0);
}

/***************************************************************************************************
* FunctionName : SPI_Read_161d
* Description  : 通过SPI口从外部的Flash中读数据                   
* Parameter(s) :                 
*                Src_Addr_161d:要读的Flash中的数据的源地址
*                Dst_Addr     :从Flash中读出的数据存储在内存中的源地址
*                Len_Of_Data  :要读的数据的长度
* Return       : void
***************************************************************************************************/
void SPI_Read_161d(u32 Src_Addr_161d,u8* Dst_Addr,u16 Len_Of_Data)
{
	WDG_CntRefresh();	/*刷新看门狗的计数器值*/

    AT45DB161_Chip_Select_ENABLE;
	BSPI_ByteSend(OPCODE_CONTINUOUS_ARRAY_READ); /*发送命令字节*/
	BSPI_ByteSend((Src_Addr_161d&0XFF0000)>>16); /*发送第一个地址字节*/
	BSPI_ByteSend((Src_Addr_161d&0XFF00)>>8);    /*发送第二个地址字节*/
	BSPI_ByteSend((Src_Addr_161d&0XFF));         /*发送第三个地址字节*/
	BSPI_ByteSend(0x00);                         /*发送四个don't care 字节*/
	BSPI_ByteSend(0x00);
	BSPI_ByteSend(0x00);
	BSPI_ByteSend(0x00);

	while(Len_Of_Data--> 0)
	{
	    *Dst_Addr++ = (u8)BSPI_ByteRecv();
	}
    AT45DB161_Chip_Select_DISABLE;

	//这里的时钟信号以及片选信号应该由谁来产生呢?
	//由芯片自己产生还是由程序来产生
	//时钟信号应该是硬件产生的，片选信号呢?
	
}

