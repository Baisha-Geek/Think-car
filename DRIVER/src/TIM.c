#include "TIM.h"

void tim_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
 
    /* GPIOA and GPIOB clock enable */ 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);  
 
    /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */ 
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           // ����������� 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
    TIM_OCInitTypeDef  TIM_OCInitStructure;  
    /* PWM�źŵ�ƽ����ֵ */  
    u16 CCR1= 3600;          
    /*PCLK1����2��Ƶ����ΪTIM3��ʱ�Ӷ�Դ����72MHz*/  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
    /* Time base configuration */                                            
    TIM_TimeBaseStructure.TIM_Period =0x1770;  
    TIM_TimeBaseStructure.TIM_Prescaler = 2;                                    //����Ԥ��Ƶ��Ԥ��Ƶ=2����Ϊ72/3=24MHz  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                                //����ʱ�ӷ�Ƶϵ��������Ƶ  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 //���ϼ������ģʽ  
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
    /* PWM1 Mode configuration: Channel1 */  
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //����ΪPWMģʽ1  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = CCR1;                                       //��������ֵ�������������������ֵʱ����ƽ��������  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //����ʱ������ֵС��CCR1ʱΪ�ߵ�ƽ  
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);                                    //ʹ��ͨ��1      
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  
    /* PWM1 Mode configuration: Channel2 */  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
    TIM_OCInitStructure.TIM_Pulse = CCR1;                                       //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM  
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                    //����ʱ������ֵС��CCR2ʱΪ�͵�ƽ 
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);                                    //ʹ��ͨ��2  
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  
    TIM_ARRPreloadConfig(TIM3, ENABLE);                                         //ʹ��TIM3���ؼĴ���ARR  
    /* TIM3 enable counter */  
    TIM_Cmd(TIM3, ENABLE);   
}

void tim2_init(void)
{
    
}
