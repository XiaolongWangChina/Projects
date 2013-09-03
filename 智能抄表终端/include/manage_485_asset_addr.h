/*********************************************************************************
* File Name          : manage_485_asset_addr.h
* Description        : 处理485地址、资产编号的新设、增加、删除、替换
********************************************************************************/
#ifndef  _MANAGE_485_ASSET_ADDR_H_
#define _MANAGE_485_ASSET_ADDR_H_
#include "71x_lib.h"
#include "Global_define.h"


/*******************************************************************************
* Function Name              : Set_Meter_485_Addr
* Description                   : 新设485地址
* inputpara                     :输入缓冲区
* Return                          : u8
* author                          :shizhide
* Date First Issued           :26/05/2007
*******************************************************************************/
u8 Set_Meter_485_Addr(u8* Process_Buf_Ptr);

/*******************************************************************************
* Function Name              : Set_Meter_Asset_Addr
* Description                   : 新设资产编号
* inputpara                     :输入缓冲区
* Return                          : u8
* author                          :shizhide
* Date First Issued           :26/05/2007
*******************************************************************************/
u8 Set_Meter_Asset_Addr(u8* Process_Buf_Ptr);

/*******************************************************************************
* Function Name              : Delete_Meter_485_Asset_Addr
* Description                   : 删除485地址、资产编号
* inputpara                     :输入缓冲区
* Return                          : u8
* author                          :shizhide
* Date First Issued           :26/05/2007
*******************************************************************************/
u8 Delete_Meter_485_Asset_Addr(u8* Process_Buf_Ptr);


#endif

