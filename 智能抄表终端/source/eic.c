/*************************************************************************************************
*File Name   : Eic.c
*Description : 增强型中断控制器的驱动源文件
*Node(s)     : 中断控制器的最底层的驱动文件 
*              创建文件
**************************************************************************************************/

#include "eic.h"

/*******************************************************************************
* Function Name  : EIC_Init
* Description    : Initialise the EIC using the load PC instruction
*                  (PC = PC +offset)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EIC_Init(void)
{
  extern u32 T0TIMI_Addr;
  u8 bCounter;
  u32 dOffset=((u32)&T0TIMI_Addr);

  /* Disable FIQ and IRQ interrupts */
  EIC->ICR = 0x00000000; 

  /* Disable all channels interrupts */
  EIC->IER = 0x00000000; 

  /* Clear all pending bits */
  EIC->IPR = 0xFFFFFFFF; 

  /* Disable all FIQ channels interrupts and clear FIQ */
  EIC->FIR = 0x0000000C; 

  /* channels pending bits */
  /* Ste the current priority level to zero */                         
  EIC->CIPR = 0x00000000; 

  /* IVR = high half of load PC instruction */
  EIC->IVR  = 0xE59F0000; 

  dOffset = (dOffset+0x00007E0)<<16;

  /* Initialize SIRn registers */
  for(bCounter=0; bCounter<32; bCounter++) 
  {
    EIC->SIR[bCounter] = dOffset|0xF0000000;
    dOffset += 0x00000004 << 16;
  }
}

