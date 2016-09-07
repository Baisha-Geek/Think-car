#ifndef __dianji_h_
#define __dianji_h_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "yuyin.h"
#include "TIM.h"

#define LED_GPIO_PORT                 GPIOF
#define LED1_GPIO_PIN                 GPIO_Pin_6
#define LED2_GPIO_PIN                 GPIO_Pin_7
#define LED3_GPIO_PIN                 GPIO_Pin_8
#define LED4_GPIO_PIN                 GPIO_Pin_9
#define LED5_GPIO_PIN                 GPIO_Pin_10
#define LED_GPIO_CLK                  RCC_APB2Periph_GPIOF


void dianji_init(void);
void dianji_qian(void);
void dianji_hou(void);
void dianji_ting(void);
void dianji_zuo(void);
void dianji_you(void);
void LED(void);
void Delay(uint16_t c);

#endif