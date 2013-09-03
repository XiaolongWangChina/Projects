/*************************************************************************************************
*File Name   : Flash.c
*Description : Flash的驱动源文件
*Node(s)     : Flash的最底层的驱动文件 
*              创建文件
**************************************************************************************************/

#include "flash.h"

/*******************************************************************************
* Function Name  : FLASH_Init
* Description    : Initialise the Flash registers to their default values.
* Input          : None.
* Return         : None.
*******************************************************************************/
void FLASH_Init(void)
{
/* Reset Flash Control Registers */
  FLASHR->CR0 = 0x00000000;
  FLASHR->CR1 = 0x00000000;
/* Reset Flash Data Registers */
  FLASHR->DR0 = 0xFFFFFFFF;
  FLASHR->DR1 = 0xFFFFFFFF;
/* Reset Flash Error Register */
  FLASHR->ER  = 0x00000000;
}

/*******************************************************************************
* Function Name  : FLASH_WordRead
* Description    : Read a single word from the flash
* Input 1        : Source Address
* Return         : Word
*******************************************************************************/
u32 FLASH_WordRead(u32 FLASH_SourceAdd)
{
  FLASH_WaitForLastTask();
  // Waits for the flash to be unlocked
  while((FLASH_FlagStatus(FLASH_LOCK))== SET);
  // Reads the data from the specified Address
  // return *(u32 *)(SourceAdd);
  return *(u32 *)(FLASH_SourceAdd );
}

/*******************************************************************************
* Function Name  : FLASH_FlagStatus
* Description    : Returns the NewState of Flash flags
* Input          : Xflag(FLASH_BSYA0,  FLASH_BSYA1, FLASH_LOCK, FLASH_INTP
*                  FLASH_B0S, FLASH_B1S, FLASH_ERR, FLASH_ERER, FLASH_PGER, 
*                  FLASH_10ER, FLASH_SEQER, FLASH_RESER, FLASH_WPF)  
* Return         : FlagStatus (SET or RESET).
*******************************************************************************/
FlagStatus FLASH_FlagStatus(flashflags Xflag)
{
  FlagStatus TmpResult = RESET;
  u8 TmpReg;
  u8 TmpPos;
  /* Get the Register Index */
  TmpReg = (Xflag & FLASH_Reg_Mask) >> 5;
  /* Get the Flag Index */
  TmpPos = (Xflag & FLASH_Flag_Mask);

  switch(TmpReg)
  {
    case 0 : /* CR0 */
    {
      /* Returns the status of the CR0[TmpPos] flag */
      TmpResult = (FLASHR->CR0 & (1<<TmpPos))==0 ? RESET : SET;
      break;
    }
    case 1 : /* CR1 */
    {
      /* Returns the status of the CR1[TmpPos] flag */
      TmpResult = (FLASHR->CR1 & (1<<TmpPos))==0 ? RESET : SET;
      break;
    }
    case 5 : /* ER */
    {
      /* Returns the status of the ER[TmpPos] flag */
      TmpResult = (FLASHR->ER  & (1<<TmpPos))==0 ? RESET : SET;
      break;
    }
  }
  return(TmpResult);
}



/*******************************************************************************
* Function Name  : FLASH_WaitForLastTask
* Description    : Waits for the end of last task on a Flash Bank.
* Input 1        : None.
* Return         : None.
*******************************************************************************/
void FLASH_WaitForLastTask(void)
{
  while (FLASHR->CR0&0x16);
}

