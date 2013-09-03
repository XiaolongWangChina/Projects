/*************************************************************************************************
*File Name   : Spi_Flash.h
*Description : 用SPI控制从Flash中读写数据的头文件
*Node(s)     : 这个文件是建立在Bspi和Flash这两个文件的基础上
*              创建文件
**************************************************************************************************/
#ifndef	_SPI_FLASH_H_
#define	_SPI_FLASH_H_

#define	BASE_ADDR_ASSET_NUMBER	0X0000		/*32块表的资产编号*/
#define	BASE_ADDR_485_ADDR		0X00C0		/*32块表的485地址*/ 
#define	BASE_ADDR_VALID_FLAG	0X0180	    /*32块表的有效标志*/
#define	BASE_ADDR_BASE_PARA 	0X01C0		/*一些基本参数，这些参数在内存中存在同一个结构体中*/
#define	TYPE_FOR_INIT_FLASH_0	0X00		/*在烧写完程序后第一次上电执行的初始化操作*/

#define	OPCODE_CONTINUOUS_ARRAY_READ	        0XE8       /*以下5行为一些操作码*/
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
* Description  : 对Flash的初始化                  
* Parameter(s) :     
*                
* Return       : void
***************************************************************************************************/
void InitBSPIFlash (void);

/***************************************************************************************************
* FunctionName : BPSI_DataSendReceive
* Description  : 从当前地址向Flash中发送数据                  
* Parameter(s) :     
*                DATA:要向Flash中发送的数据
*
* Return       : u8:从Flash中返回的数据
***************************************************************************************************/
u8 BPSI_DataSendReceive(u8 DATA);    


/***************************************************************************************************
* FunctionName : SPI_Write_161d
* Description  : 通过SPI口向外部的Flash中写数据                 
* Parameter(s) :                 
*                Dst_Addr_161d:要写的Flash中的目的地址
*                Src_Addr     :将要写入到Flash中的数据的源地址
*                Len_Of_Data  :要写入的数据的长度
* Return       : void
***************************************************************************************************/
void SPI_Write_161d(u32 Dst_Addr_161d,u8* Src_Addr,u16 Len_Of_Data);

/***************************************************************************************************
* FunctionName : SPI_Read_161d
* Description  : 通过SPI口从外部的Flash中读数据                   
* Parameter(s) :                 
*                Src_Addr_161d:要读的Flash中的数据的源地址
*                Dst_Addr     :从Flash中读出的数据存储在内存中的源地址
*                Len_Of_Data  :要读的数据的长度
* Return       : void
***************************************************************************************************/
void SPI_Read_161d(u32 Src_Addr_161d,u8* Dst_Addr,u16 Len_Of_Data);


#endif 

