#include "naodianbo.h"

u8 Uart2_data[];
u8 Uart2_Buffer[8];//���С������
unsigned int Uart2_Rx = 0;
unsigned int Uart2_Tx = 0;
unsigned int Uart2_Len = 8;
unsigned int Uart2_Sta = 0;
unsigned int tx2;

unsigned int T1[];//գ�۵�����
unsigned int T2[];//��������rawdata�����飬���Ͻ��и��£����������ж೤����Ҫ��֤����Ҫ��Ҫ��������rom�У�
unsigned int a;//���鳤��
unsigned int attention;//������ݻ᲻�ϸ��£����������ֵ���Զ�ͣ��,��ֵ�ݶ�Ϊ50
                         //���д���Ľ��գ����ڻ�û����������Ϊ����ʶ�𵽴��
unsigned int signal;

//����������,USART2,PA2 PA3
void lanya_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    USART_DeInit(USART1);  //��λ����1      
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    USART_InitTypeDef USART_InitStructure;
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//����Ҫ����
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    
    USART_Init(USART2, &USART_InitStructure);
    
    

    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                                               //ѡ���жϷ���2       
    NVIC_InitStructure.NVIC_IRQChannel= USART2_IRQn;                              //ѡ�񴮿�2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;                                 //��ռʽ�ж����ȼ�����Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;                                            //��Ӧʽ�ж����ȼ�����Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                                 //ʹ���ж�
    NVIC_Init(&NVIC_InitStructure);
    
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
}

// ��������������


int chuli(unsigned int Uart2_Buffer[8]);

unsigned int rawdata;//���ǵ���С�����rawdata���ܸо�Ӧ�ð��������������д�� _it.c �ļ���



/*���������������ʱ�Ƶ�it�ļ�
//�������������������ݣ�����֤У��ֵ��Ȼ����rawdata
void panduan(unsigned int Uart2_Buffer[])
{
    if( Uart2_Sta )
    {
         if(Uart2_Rx == 8)
             if(Uart2_Buffer[7] == (((0x80 + 0x02 + Uart2_Buffer[5] + Uart2_Buffer[6])^ 0xFFFFFFFF) & 0xFF))
                  rawdata = chuli(Uart2_Buffer);
         
         Uart2_Rx = 0; //��ʼ��
         Uart2_Tx = 0;
         Uart2_Sta = 0;
    }
    
}

int chuli(unsigned int Uart2_Buffer[])
{
    unsigned int rawdata_;
    rawdata_ = (Uart2_Buffer[5] << 8) | Uart2_Buffer[6];
    if(rawdata_ > 32768)
    {
        rawdata_ -=65536;
    }
    return rawdata_;
}
*/
int sum(int x[],int a)//xΪ�����飬aΪ���鳤�ȣ��������
{
	unsigned int sum = 0;
	for (unsigned int i = 0;i != a;i++)
		sum += x[i];
	return sum;
}

//���������������������������ϵ������Ϊ����ֵ���أ�����1��-1֮�䡣1Ϊ��ȫ����أ�0Ϊ����أ�-1Ϊ��ȫ����أ�
//T1���������飬T2�ǽ��յ���������

double pearson(unsigned int T1[],unsigned int T2[],int a) //aΪ���鳤��
{
      unsigned int xhe = sum(T1,a);
      unsigned int yhe = sum(T2,a);
      unsigned int xyhe = 0;
      unsigned int x2he = 0;
      unsigned int y2he = 0;
      int i = 0;
      double r;
      //���е�x*y�������ĺ�
      for(i = 0;i != a;i++)
      {
          xyhe += (T1[i]*T2[i]);
      }
      //����xƽ���ĺ�
      for(i = 0;i != a;i++)
      {
          x2he += (pow(T1[i],2));
      }
      //����yƽ���ĺ�
      for(i = 0;i != a;i++)
      {
          y2he += (pow(T2[i],2));
      }
      double fenzi = a * xyhe - xhe*yhe;
      double fenmu = pow((a*x2he - pow(xhe,2)),0.5) * pow((a*y2he - pow(yhe,2)),0.5);
      if(fenmu == 0)
          r = 0;
      else
          r = fenzi / fenmu;
      return r;
}

//����ĺ��������ж�һ����գ�ۼ���
//����Ҫ�Ľ�


int howmany(unsigned int T1[],unsigned int T2[],int a)
{
      int times_time = 0;//�����ڵ�һ��գ�ۺ���ʱ
      int times = 0;      //�����ں����м�¼գ�۴���
      u8 time_return = 0;//��������գ�۴���
      
            switch(times)
            {
            case 0:
              {
                if(pearson(T1[],T2[],a) > 0.95)
                {
                  times = 1;
                  times_time = 0;
                  //Ҫ��Ҫ���������T2��Ҳ���Ǵ��rawdata�����飩������˵ÿһ�γɹ�ʶ��գ�ۺ����T2
                }
                else
                {
                  time_return = 0;
                  break;
                }
              }
            case 1:
              {
                //��һ�мǵü��ϼ���ʱ��������֪���ӳٶ��
                //���ﻹ�����⣬�ǲ���Ҫ��¼գ��֮����Զ�������ݣ�Ȼ������ʶ����һ�Σ���Ȼ������ʶ��ڶ���
                if(pearson(T1[],T2[],a) > 0.95)
                {
                  time_return = 2;
                  times = 0;
                }
                else
                {
                  if(++times_time > 50)
                  {
                    time_return = 1;
                    times = 0;
                  }
                }
                break;
              }
              
            }
            return time_return;
}

//����ĺ��������ж��Ƿ���ע����
int zhuyili()
{
    if(attention < 50)
    {
      dianji_ting();
      return 0;
    }
    else
      return 1;
}

//����������������Ե粨���Ƶ���ĳ���
void naoKongzhi()
{
  while(1)
  {
    if(int i = zhuyili())
    {
      switch(int x = howmany(T1[],T2[],a))
      {
      case 1:dianji_zuo();
      break;
      case 2:dianji_you();
      break;
      case 3:dianji_ting();
      }
    }
    
  }
}

