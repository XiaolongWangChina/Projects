#ifndef __TIM_H
#define __TIM_H

#include "71x_map.h"
#include "71x_lib.h"

typedef enum { TIM_EXTERNAL,
               TIM_INTERNAL
             } TIM_Clocks; 

typedef enum { TIM_RISING,
               TIM_FALLING
             } TIM_Clock_Edges; 

typedef enum { TIM_CHANNEL_A,
               TIM_CHANNEL_B
             } TIM_Channels;  

typedef enum { TIM_WITH_IT,
               TIM_WITHOUT_IT
             } TIM_IT_Mode;

typedef enum { TIM_TIMING,
               TIM_WAVE
             } TIM_OC_Modes; 

typedef enum { TIM_HIGH,
               TIM_LOW
             } TIM_Logic_Levels; 

typedef enum { TIM_START,
               TIM_STOP,
               TIM_CLEAR
             } TIM_CounterOperations; 

typedef enum { TIM_ICFA = 0x8000,
               TIM_OCFA = 0x4000,
               TIM_TOF  = 0x2000,
               TIM_ICFB = 0x1000,
               TIM_OCFB = 0x0800
             } TIM_Flags;

typedef struct { u16 Pulse;
                 u16 Period;
               } PWMI_parameters; 

#define TIM_ECKEN_Mask  0x0001 
#define TIM_EXEDG_Mask  0x0002 

#define TIM_IEDGA_Mask  0x0004 
#define TIM_IEDGB_Mask  0x0008 

#define TIM_PWM_Mask    0x0010 

#define TIM_OPM_Mask    0x0020 

#define TIM_OCAE_Mask   0x0040 
#define TIM_OCBE_Mask   0x0080  

#define TIM_OLVLA_Mask  0x0100 
#define TIM_OLVLB_Mask  0x0200  

#define TIM_FOLVA_Mask  0x0400  
#define TIM_FOLVB_Mask  0x0800 

#define TIM_PWMI_Mask   0x4000 

#define TIM_EN_Mask     0x8000  

#define TIM_OCBIE_mask  0x0800  
#define TIM_ICBIE_Mask  0x1000  
#define TIM_TOE_Mask    0x2000  
#define TIM_OCAIE_mask  0x4000   

#define TIM_ICAIE_Mask  0x8000   

#define TIM_ICA_IT   0x8000 /* Input Capture Channel A  */
#define TIM_OCA_IT   0x4000 /* Output Compare Channel A */
#define TIM_TO_IT    0x2000 /* Timer OverFlow           */
#define TIM_ICB_IT   0x1000 /* Input Capture Channel B  */
#define TIM_OCB_IT   0x0800 /* Input Capture Channel A  */


/*******************************************************************************
* Function Name  : TIM_Init
* Description    : This routine is used to Initialize the TIM peripheral
* Input          : TIM Timer to Initialize
* Return         : None
*******************************************************************************/
void TIM_Init( TIM_TypeDef *TIMx );
/*******************************************************************************
* Function Name  : TIM_PrescalerConfig
* Description    : This routine is used to configure the TIM prescaler value
*                  ( using an internal clock )
* Input          : (1) TIM Timer
*                : (2) Prescaler ( u8 )
* Return         : None
*******************************************************************************/
inline void TIM_PrescalerConfig ( TIM_TypeDef *TIMx, u8 Xprescaler )
{
	TIMx->CR2 = ( TIMx->CR2 & 0xFF00 ) | Xprescaler;
}
/*******************************************************************************
* Function Name  : TIM_OCMPModeConfig
* Description    : This routine is used to configure the output compare feature
* Input          : (1) TIM Timer
*                : (2) OCMP Channel ( Channel_A or Channel_B )
*                : (3) Pulse Length
*                : (4) OC_Mode     : output wave, or only timing.
*                : (5) Level       : Rising edge or Falling edge after the ==
* Output         : None
*******************************************************************************/
void TIM_OCMPModeConfig (  TIM_TypeDef  *TIMx,
                           TIM_Channels Xchannel,
                           u16          XpulseLength,
                           TIM_OC_Modes     Xmode,
                           TIM_Logic_Levels Xlevel );
/*******************************************************************************
* Function Name  : TIM_PWMOModeConfig
* Description    : This routine is used to configure the PWM in output mode
* Input          : (1) TIM Timer
*                : (2) DutyCycle   : u16
*                : (3) Level 1     : During the Duty Cycle
*                : (4) Level 2     : During the after the pulse
*                : (5) Full period : u16
* Output         : None
*******************************************************************************/
void TIM_PWMOModeConfig ( TIM_TypeDef  *TIMx,
                          u16          XDutyCycle,
                          TIM_Logic_Levels XLevel1,
                          u16          XFullperiod,
                          TIM_Logic_Levels XLevel2
                        );
/*******************************************************************************
* Function Name  : TIM_CounterConfig
* Description    : This routine is used to start/stop and clear the selected
*                  timer counter
* Input          : (1) TIM Timer
*                : (2) TIM_CounterOperations
                      TIM_START Enables or resumes the counter
*                     TIM_STOP  Stops the TIM counter
*                     TIM_CLEAR Set the TIM counter value to FFFCh
* Output         : None
*******************************************************************************/
void TIM_CounterConfig ( TIM_TypeDef *TIMx, TIM_CounterOperations Xoperation );

/*******************************************************************************
* Function Name  : TIM_ITConfig
* Description    : This routine is used to configure the TIM IT
* Input          : (1) TIM Timer
*                : (2) TIM interrupt (input capture A or input capture B or timer overflow or out compA or outpu com B
*                : (2) ENABLE / DISABLE
* Output         : None
*******************************************************************************/
inline void TIM_ITConfig ( TIM_TypeDef *TIMx, u16 New_IT, FunctionalState NewState )
{
	if (NewState == ENABLE) TIMx->CR2 |= New_IT; else TIMx->CR2 &= ~New_IT;
}
/*******************************************************************************
* Function Name  : TIM_FlagClear
* Description    : This routine is used to clear Flags.
* Input          : (1) TIM Timer
*                : (2) The TIM FLag
* Output         : None
*******************************************************************************/
inline void TIM_FlagClear ( TIM_TypeDef *TIMx, TIM_Flags Xflag )
{
	TIMx->SR &= ~Xflag;
}
#endif	/* __TIM_H */

