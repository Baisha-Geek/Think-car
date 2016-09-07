#ifndef __yuyin_h_
#define __yuyin_h_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "string.h"

void yuyin_init(void);
void delay_yuyin(unsigned int timer_delay);
void yuyin_Send_Byte(char *yuyin_n);
void UART5_IRQHandler();
void yuyin_qian();
void yuyin_hou();
void yuyin_zuo();
void yuyin_you();
void yuyin_ting();



#endif