#include "yuyin.h"

//语音
char qian[] = "正在前进";
char hou[] = "正在后退";
char zuo[] = "正在左转";
char you[] = "正在右转";
char ting[] = "正在停止";

void yuyin_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    USART_InitTypeDef USART_InitStructure;
    
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    
    USART_Init(USART1, &USART_InitStructure);
    
    USART_Cmd(USART1, ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ClearFlag(USART1, USART_FLAG_TC);
}


unsigned char yuyin_ct = 0;
unsigned char yuyin_test_ok = 0;
unsigned char  headOfFrame[5];
unsigned char length ;
unsigned char ecc = 0 ; //定义校验字节
unsigned int yuyin_ct_i = 0;


void yuyin_Send_Byte(char *yuyin_n )
{
    ecc = 0;
    length = (strlen(yuyin_n)); //需要发送文本的长度
/*****************发送过程**************************************/ 
    headOfFrame[0] = 0xFD ; //构造帧头 FD
    headOfFrame[1] = 0x00 ; //构造数据区长度的高字节
    headOfFrame[2] = length + 3; //构造数据区长度的低字节
    headOfFrame[3] = 0x01 ; //构造命令字：合成播放命令
    headOfFrame[4] = 0x01 ; //构造命令参数：编码格式为 GBK
    for(yuyin_ct_i = 0; yuyin_ct_i<5; yuyin_ct_i++) //依次发送构造好的 5 个帧头字节
    {
        ecc=ecc^(headOfFrame[yuyin_ct_i]); //对发送的字节进行异或校验
        USART_SendData(USART1,headOfFrame[yuyin_ct_i]);
        /* 等待发送完毕 */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
    }
    for(yuyin_ct_i = 0; yuyin_ct_i < length; yuyin_ct_i++) //依次发送待合成的文本数据
    {
        ecc=ecc^(yuyin_n[yuyin_ct_i]); //对发送的字节进行异或校验
        USART_SendData(USART1,yuyin_n[yuyin_ct_i]);
        /* 等待发送完毕 */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
    }
    USART_SendData(USART1,ecc);
    /* 等待发送完毕 */
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
}

//我不知道下面这个函数有卵用
void USART1_IRQHandler()
{
        /* 接收中断 */
        if(((USART1->SR) & 0X0020) == 0X0020)
        {
                USART1->SR &=~0X0020;
                if(USART1->DR == 0x4F)
                {
                        yuyin_test_ok = 1;
                }
        }
}

//下面是各个动作的语音
void yuyin_qian()
{
    yuyin_Send_Byte(qian );
}

void yuyin_hou()
{
    yuyin_Send_Byte(hou );
}

void yuyin_zuo()
{
    yuyin_Send_Byte(zuo );
}

void yuyin_you()
{
    yuyin_Send_Byte(you );
}

void yuyin_ting()
{
    yuyin_Send_Byte(ting );
}



