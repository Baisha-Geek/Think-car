/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

extern u8 Uart2_data[];//存放不断传输过来的数据
extern u8 Uart2_Buffer[];//存放小包数据
extern unsigned int Uart2_Rx, Uart2_Tx, Uart2_Len, Uart2_Sta, tx2;
extern unsigned int rawdata;
extern unsigned int attention;
extern unsigned int signal;
void USART2_IRQHandler()
{
     if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //中断产生
     {
         USART_ClearITPendingBit(USART2,USART_IT_RXNE); //清除中断标志
         Uart2_data[Uart2_Rx] = USART_ReceiveData(USART2);                 
         Uart2_Rx++;
         Uart2_Rx &= 0xFF;//这一句是干什么的。。。。FF=1111 1111
     }
     if(Uart2_data[Uart2_Tx + 2] != 0x20)//这一句检测是大包还是小包
     {
       //下面这段暂时不用，本来打算判断是否丢包，结果发现好像不用
/*         if(Uart2_Rx == 5)
         {
            //
             if((Uart2_data[0] != 0xAA) || (Uart2_data[1] != 0xAA) || (Uart2_data[2] != 0x04) || (Uart2_data[3] != 0x80) || (Uart2_data[4] != 0x02))
             {
                Uart2_Rx = 0; //初始化
                Uart2_Tx = 0;
                Uart2_Sta = 0;         
             }
         }
*/
         if((Uart2_data[Uart2_Tx] == 0xAA) && (Uart2_Rx == 8));  //   检测到头的情况下检测到尾
         {
             Uart2_Sta=1; //标志位
             for(int i = 0;i < 8;i++)
                 Uart2_Buffer[i] = Uart2_data[i];
         }
     }
     else  //这是大包
     {
         if(Uart2_Rx - 1 == 4)
           if(Uart2_data[Uart2_Rx-1] == 29 || Uart2_data[Uart2_Rx-1] == 54 || Uart2_data[Uart2_Rx-1] == 55 || Uart2_data[Uart2_Rx-1] == 56 || Uart2_data[Uart2_Rx-1] == 80 || Uart2_data[Uart2_Rx-1] == 81 || Uart2_data[Uart2_Rx-1] == 82 || Uart2_data[Uart2_Rx-1] == 107 || Uart2_data[Uart2_Rx-1] == 200)
           {
             Uart2_Rx = 0; //初始化
             Uart2_Tx = 0;
             Uart2_Sta = 0;
             //这里还没写完
           }//这里要提示耳机没带好,还要清空数据,代码先放过，用语音来做。而且上面的比较涉及到十进制与十六进制，改天再看
         if(Uart2_Rx - 1 == 32)
         {
           
           attention = Uart2_data[Uart2_Rx - 1];//这是专注度，还有个放松度是什么鬼
           Uart2_Rx = 0; //初始化
           Uart2_Tx = 0;
           Uart2_Sta = 0;
         }
     }
     
     if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET) //溢出
     {
         USART_ClearFlag(USART2,USART_FLAG_ORE); //读SR
         USART_ReceiveData(USART2); //读DR
     }//这个溢出的部分感觉不对，暂时不管
     
     //下面是当标记位为1时，处理小包数据，取出rawdata，然后不管丢不丢包，都初始化
     if( Uart2_Sta )
    {
         if(Uart2_Rx == 8)
             if(Uart2_Buffer[7] == (((0x80 + 0x02 + Uart2_Buffer[5] + Uart2_Buffer[6])^ 0xFFFFFFFF) & 0xFF))
             {
                 rawdata = (Uart2_Buffer[5] << 8) | Uart2_Buffer[6];
                 if(rawdata > 32768)
                 {
                     rawdata -=65536;
                 }
             }
         
         Uart2_Rx = 0; //初始化
         Uart2_Tx = 0;
         Uart2_Sta = 0;
    }
}


