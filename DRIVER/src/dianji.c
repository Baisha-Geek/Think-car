#include "dianji.h"

void dianji_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    //6接IN1，7接IN2，8接ENA，9接IN3，10接IN4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

void LED(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN | LED2_GPIO_PIN | LED3_GPIO_PIN
                                	            | LED4_GPIO_PIN | LED5_GPIO_PIN;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED_GPIO_PORT,LED1_GPIO_PIN|LED2_GPIO_PIN|LED3_GPIO_PIN|LED4_GPIO_PIN|LED5_GPIO_PIN);
        
        GPIO_SetBits(LED_GPIO_PORT, LED1_GPIO_PIN|LED3_GPIO_PIN|LED5_GPIO_PIN);
}

void dianji_qian(void)
{
    GPIO_SetBits(GPIOF, GPIO_Pin_6 | GPIO_Pin_9);
    GPIO_ResetBits(GPIOF, GPIO_Pin_7 | GPIO_Pin_10);
    yuyin_qian();
}

void dianji_hou(void)
{
    GPIO_SetBits(GPIOF, GPIO_Pin_7 | GPIO_Pin_10);
    GPIO_ResetBits(GPIOF, GPIO_Pin_6 | GPIO_Pin_9);   
    yuyin_hou();
}

void dianji_ting(void)
{
    GPIO_SetBits(GPIOF, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10);
    yuyin_ting();
}

void dianji_zuo(void)
{
    GPIO_SetBits(GPIOF, GPIO_Pin_6 | GPIO_Pin_10);
    GPIO_ResetBits(GPIOF, GPIO_Pin_7 | GPIO_Pin_9);  
    yuyin_zuo();
}

void dianji_you(void)
{
    GPIO_SetBits(GPIOF, GPIO_Pin_7 | GPIO_Pin_9);
    GPIO_ResetBits(GPIOF, GPIO_Pin_6 | GPIO_Pin_10);  
    yuyin_you();
    Delay(10);
    GPIO_SetBits(GPIOF, GPIO_Pin_8);
}

void Delay(uint16_t c)
{
	uint16_t a, b;
	for(; c>0; c--)
		for(a=1000; a>0; a--)
			for(b=1000; b>0; b--);
}

