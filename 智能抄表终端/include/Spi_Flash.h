/*************************************************************************************************
*File Name   : Spi_Flash.h
*Description : ��SPI���ƴ�Flash�ж�д���ݵ�ͷ�ļ�
*Node(s)     : ����ļ��ǽ�����Bspi��Flash�������ļ��Ļ�����
*              �����ļ�
**************************************************************************************************/
#ifndef	_SPI_FLASH_H_
#define	_SPI_FLASH_H_

#define	BASE_ADDR_ASSET_NUMBER	0X0000		/*32�����ʲ����*/
#define	BASE_ADDR_485_ADDR		0X00C0		/*32����485��ַ*/ 
#define	BASE_ADDR_VALID_FLAG	0X0180	    /*32������Ч��־*/
#define	BASE_ADDR_BASE_PARA 	0X01C0		/*һЩ������������Щ�������ڴ��д���ͬһ���ṹ����*/
#define	TYPE_FOR_INIT_FLASH_0	0X00		/*����д�������һ���ϵ�ִ�еĳ�ʼ������*/

#define	OPCODE_CONTINUOUS_ARRAY_READ	        0XE8       /*����5��ΪһЩ������*/
#define	OPCODE_MAIN_MEM_TO_BUFFER_1	            0X53
#define	OPCODE_BUFFER_1_WRITE			        0X84
#define	OPCODE_BUFFER_1_TO_MAIN_MEM	            0X83
#define	OPCODE_RAM_TO_MAIN_MEM_THROUGH_BUF_1    0x82

#define	SPI_FLASH_PAGE_SIZE 512

#define BSPI1_MISO   0x0010          /* 0.4 SPI1: Master in/Slave out data */
#define BSPI1_MOSI   0x0020          /* 0.5 SPI1: Master out/Slave In data */
#define BSPI1_SCLK   0x0040          /* 0.6 SPI1: Serial Clock */
#define BSPI1_SSN    0x0080          /*  0.7 SPI1: SSN */  
#define FLASH_CS     0x0008          /*  0.3 M25 Ship Select */

#define AT45DB161_MainMemoeryPage_Read  0xD2
#define AT45DB161_MAIN2BUF1_TRAN        0x82
#define AT45DB161_Chip_Select_ENABLE  do{								\
										    EIC_DISABLE;				\
											GPIO0->PD &= ~FLASH_CS;	    \
											EIC_ENABLE;  				\
										 }while(0)
											
#define  AT45DB161_Chip_Select_DISABLE do{								\
										    EIC_DISABLE;				\
										    GPIO0->PD |= FLASH_CS;		\
										    EIC_ENABLE; 				\
										  }while(0)

#define Dummy              0x0F

#define BSPI_InstructionSend(Instruction)   BPSI_DataSendReceive (Instruction)
#define	BSPI_ByteSend( DATA)				BPSI_DataSendReceive( DATA)
#define	BSPI_ByteRecv( )					BPSI_DataSendReceive(Dummy)

/***************************************************************************************************
* FunctionName : InitBSPIFlash
* Description  : ��Flash�ĳ�ʼ��                  
* Parameter(s) :     
*                
* Return       : void
***************************************************************************************************/
void InitBSPIFlash (void);

/***************************************************************************************************
* FunctionName : BPSI_DataSendReceive
* Description  : �ӵ�ǰ��ַ��Flash�з�������                  
* Parameter(s) :     
*                DATA:Ҫ��Flash�з��͵�����
*
* Return       : u8:��Flash�з��ص�����
***************************************************************************************************/
u8 BPSI_DataSendReceive(u8 DATA);    


/***************************************************************************************************
* FunctionName : SPI_Write_161d
* Description  : ͨ��SPI�����ⲿ��Flash��д����                 
* Parameter(s) :                 
*                Dst_Addr_161d:Ҫд��Flash�е�Ŀ�ĵ�ַ
*                Src_Addr     :��Ҫд�뵽Flash�е����ݵ�Դ��ַ
*                Len_Of_Data  :Ҫд������ݵĳ���
* Return       : void
***************************************************************************************************/
void SPI_Write_161d(u32 Dst_Addr_161d,u8* Src_Addr,u16 Len_Of_Data);

/***************************************************************************************************
* FunctionName : SPI_Read_161d
* Description  : ͨ��SPI�ڴ��ⲿ��Flash�ж�����                   
* Parameter(s) :                 
*                Src_Addr_161d:Ҫ����Flash�е����ݵ�Դ��ַ
*                Dst_Addr     :��Flash�ж��������ݴ洢���ڴ��е�Դ��ַ
*                Len_Of_Data  :Ҫ�������ݵĳ���
* Return       : void
***************************************************************************************************/
void SPI_Read_161d(u32 Src_Addr_161d,u8* Dst_Addr,u16 Len_Of_Data);


#endif 

