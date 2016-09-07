#ifndef __naodianbo_h_
#define __naodianbo_h_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"

#include "math.h"
#include "dianji.h"

void lanya_init(void);
int chuli(unsigned int Uart2_Buffer[8]);

#endif