/*************************************************************************************************
*File Name   : Flash.h
*Description : Flash的驱动头文件
*Node(s)     : Flash的最底层的驱动文件 
*              创建文件
**************************************************************************************************/

#ifndef __FLASH_H
#define __FLASH_H

#include "71x_map.h"


typedef enum
{
  FLASH_BSYA0   =  0x01, /* 000-00001 = 0000 0001 = 0x01 */ /* FLASH_CR0.1 */
  FLASH_BSYA1   =  0x02, /* 000-00010 = 0000 0010 = 0x02 */ /* FLASH_CR0.2 */
  FLASH_LOCK    =  0x04, /* 000-00100 = 0000 0100 = 0x04 */ /* FLASH_CR0.4*/
  FLASH_INTP    =  0x14, /* 000-10100 = 0001 0100 = 0x14 */ /* FLASH_CR0.20 */
  FLASH_B0S     =  0x38, /* 001-11000 = 0011 1000 = 0x38 */ /* FLASH_CR1.24*/
  FLASH_B1S     =  0x39, /* 001-11001 = 0011 1001 = 0x39 */ /* FLASH_CR1.25*/
  FLASH_ERR     =  0xA0, /* 101-00000 = 1010 0000 = 0xA0 */ /* FLASH_ER.0*/
  FLASH_ERER    =  0xA1, /* 101-00001 = 1010 0001 = 0xA1 */ /* FLASH_ER.1*/
  FLASH_PGER    =  0xA2, /* 101-00010 = 1010 0010 = 0xA2 */ /* FLASH_ER.2*/
  FLASH_10ER    =  0xA3, /* 101-00011 = 1010 0011 = 0xA3 */ /* FLASH_ER.3*/
  FLASH_SEQER   =  0xA6, /* 101-00110 = 1010 0110 = 0xA6 */ /* FLASH_ER.6*/
  FLASH_RESER   =  0xA7, /* 101-00111 = 1010 0111 = 0xA7 */ /* FLASH_ER.7*/
  FLASH_WPF     =  0xA8  /* 101-01000 = 1010 1000 = 0xA8 */ /* FLASH_ER.8*/
} flashflags;

#define FLASH_WMS_Mask   0x80000000 
#define FLASH_SUSP_Mask  0x40000000 
#define FLASH_WPG_Mask   0x20000000 
#define FLASH_DWPG_Mask  0x10000000  
#define FLASH_SER_Mask   0x08000000  
#define FLASH_SPR_Mask   0x01000000 
#define FLASH_DBGP_Mask  0x00000002
#define FLASH_ACCP_Mask  0x00000001
#define FLASH_Reg_Mask   0xE0 
#define FLASH_Flag_Mask  0x1F 
#define FLASH_INTM_Mask  0x00200000 

/*******************************************************************************
* Function Name  : FLASH_Init
* Description    : Initialise the Flash
* Input          : None
* Return         : None
*******************************************************************************/
void FLASH_Init(void);

/*******************************************************************************
* Function Name  : FLASH_FlagStatus
* Description    : Returns the NewState of Flash flags
* Input 1        : Flash Flag (FLASH_BSYA0,  FLASH_BSYA1, FLASH_LOCK, FLASH_INTP
*                  FLASH_B0S, FLASH_B1S, FLASH_ERR, FLASH_ERER, FLASH_PGER, 
*                  FLASH_10ER, FLASH_SEQER, FLASH_RESER, FLASH_WPF)  
* Return         : flagstatus (SET or RESET)
*******************************************************************************/
FlagStatus FLASH_FlagStatus(flashflags Xflag);

/*******************************************************************************
* Function Name  : FLASH_WordRead
* Description    : Read a single word of the flash
* Input 1        : Source Address
* Return         : Word
*******************************************************************************/
u32 FLASH_WordRead(u32 FLASH_SourceAdd);


/*******************************************************************************
* Function Name  : FLASH_WaitForLastTask
* Description    : Waits for the end of last task on a Flash Bank
* Input 1        : None
* Return         : Nones
*******************************************************************************/
void FLASH_WaitForLastTask(void);



#endif  /* __FLASH_H */

