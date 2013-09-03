/*************************************************************************************************
*File Name   : Bspi.h
*Description : ����ͨ��spi������Դ�ļ�
*Node(s)     : BSPI����ײ�������ļ� 
*              �����ļ�
**************************************************************************************************/

#include "bspi.h"

/*******************************************************************************
* Function Name  : BSPI_Init
* Description    : Initializes BSPI peripheral control and registers to their default reset values.
* Input          : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Output         : BSPI peripheral registers and BSPI global variables are initialized.
* Return         : None.
*******************************************************************************/
void BSPI_Init(BSPI_TypeDef *BSPIx)
{
  BSPIx->CSR2 = 0x41;
  BSPIx->CSR1 = 0x00;
  BSPIx->CLK  = 0x06;
}
