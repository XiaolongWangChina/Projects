/*************************************************************************************************
*File Name   : Spi_Flash.c
*Description : ��SPI���ƴ�Flash�ж�д���ݵ�ͷ�ļ�
*Node(s)     : ����ļ��ǽ�����Bspi��Flash�������ļ��Ļ�����
*              �����ļ�
**************************************************************************************************/
#include "71x_lib.h" 
#include "spi_flash.h"


/***************************************************************************************************
* FunctionName : InitBSPIFlash
* Description  : ��Flash�ĳ�ʼ��                  
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
* Description  : �ӵ�ǰ��ַ��Flash�з�������                  
* Parameter(s) :     
*                DATA:Ҫ��Flash�з��͵�����
*
* Return       : u8:��Flash�з��ص�����
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
* Description  : ͨ��SPI�����ⲿ��Flash��д����                 
* Parameter(s) :                 
*                Dst_Addr_161d:Ҫд��Flash�е�Ŀ�ĵ�ַ
*                Src_Addr     :��Ҫд�뵽Flash�е����ݵ�Դ��ַ
*                Len_Of_Data  :Ҫд������ݵĳ���
* Return       : void
***************************************************************************************************/
void SPI_Write_161d(u32 Dst_Addr_161d,u8* Src_Addr,u16 Len_Of_Data)
{
	u32  Temp_Dst_Addr;		//ÿ��ѭ����Ҫ��FLASH��д��ĵ�ַ
	u16  Temp_Len;			//ÿ��ѭ����Ҫ��FLASH��д������ݵĳ���
	u32  i=0;
	u16  Data_Left_Len;		//ʣ��Ļ�û��д������ݵĳ���

	Temp_Dst_Addr = Dst_Addr_161d;
	Temp_Len      = (SPI_FLASH_PAGE_SIZE - Dst_Addr_161d%SPI_FLASH_PAGE_SIZE)<Len_Of_Data? (SPI_FLASH_PAGE_SIZE - Dst_Addr_161d%SPI_FLASH_PAGE_SIZE):Len_Of_Data;
	Data_Left_Len = Len_Of_Data;

	do
	{
		WDG_CntRefresh(); /*ˢ�¿��Ź��ļ�����ֵ*/

   		AT45DB161_Chip_Select_ENABLE;   
		BSPI_ByteSend(OPCODE_MAIN_MEM_TO_BUFFER_1);	 /*���������ֽ�*/
		BSPI_ByteSend((Temp_Dst_Addr&0XFF0000)>>16); /*���͵�һ����ַ�ֽ�*/
		BSPI_ByteSend((Temp_Dst_Addr&0XFF00)>>8);	 /*���͵ڶ�����ַ�ֽ�*/
		BSPI_ByteSend((Temp_Dst_Addr&0XFF));		 /*���͵�������ַ�ֽ�*/
    	AT45DB161_Chip_Select_DISABLE;

		for(i=0;i<0X0FFFF;i++);				

		AT45DB161_Chip_Select_ENABLE;   
		BSPI_ByteSend(OPCODE_RAM_TO_MAIN_MEM_THROUGH_BUF_1); /*���������ֽ�*/
		BSPI_ByteSend((Temp_Dst_Addr&0XFF0000)>>16);         /*���͵�һ����ַ�ֽ�*/
		BSPI_ByteSend((Temp_Dst_Addr&0XFF00)>>8);            /*���͵ڶ�����ַ�ֽ�*/
		BSPI_ByteSend((Temp_Dst_Addr&0XFF));		     /*���͵�������ַ�ֽ�*/
		for(i=0;i<Temp_Len;i++)
		{
			BSPI_ByteSend(*Src_Addr++);
		}
		AT45DB161_Chip_Select_DISABLE;

		for(i=0;i<0XFFFF;i++);	
		WDG_CntRefresh();      /*ˢ�¿��Ź��ļ�����ֵ*/
		for(i=0;i<0XFFFF;i++);	
		WDG_CntRefresh();	   /*ˢ�¿��Ź��ļ�����ֵ*/
		for(i=0;i<0XFFFF;i++);	
		WDG_CntRefresh();	   /*ˢ�¿��Ź��ļ�����ֵ*/
		for(i=0;i<0XFFFF;i++);	
		WDG_CntRefresh();	   /*ˢ�¿��Ź��ļ�����ֵ*/

		Data_Left_Len  -=Temp_Len;
		Temp_Dst_Addr  += Temp_Len;	/*�ı���һ�ε�д��ĵ�ַ�ͳ���*/
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
* Description  : ͨ��SPI�ڴ��ⲿ��Flash�ж�����                   
* Parameter(s) :                 
*                Src_Addr_161d:Ҫ����Flash�е����ݵ�Դ��ַ
*                Dst_Addr     :��Flash�ж��������ݴ洢���ڴ��е�Դ��ַ
*                Len_Of_Data  :Ҫ�������ݵĳ���
* Return       : void
***************************************************************************************************/
void SPI_Read_161d(u32 Src_Addr_161d,u8* Dst_Addr,u16 Len_Of_Data)
{
	WDG_CntRefresh();	/*ˢ�¿��Ź��ļ�����ֵ*/

    AT45DB161_Chip_Select_ENABLE;
	BSPI_ByteSend(OPCODE_CONTINUOUS_ARRAY_READ); /*���������ֽ�*/
	BSPI_ByteSend((Src_Addr_161d&0XFF0000)>>16); /*���͵�һ����ַ�ֽ�*/
	BSPI_ByteSend((Src_Addr_161d&0XFF00)>>8);    /*���͵ڶ�����ַ�ֽ�*/
	BSPI_ByteSend((Src_Addr_161d&0XFF));         /*���͵�������ַ�ֽ�*/
	BSPI_ByteSend(0x00);                         /*�����ĸ�don't care �ֽ�*/
	BSPI_ByteSend(0x00);
	BSPI_ByteSend(0x00);
	BSPI_ByteSend(0x00);

	while(Len_Of_Data--> 0)
	{
	    *Dst_Addr++ = (u8)BSPI_ByteRecv();
	}
    AT45DB161_Chip_Select_DISABLE;

	//�����ʱ���ź��Լ�Ƭѡ�ź�Ӧ����˭��������?
	//��оƬ�Լ����������ɳ���������
	//ʱ���ź�Ӧ����Ӳ�������ģ�Ƭѡ�ź���?
	
}

