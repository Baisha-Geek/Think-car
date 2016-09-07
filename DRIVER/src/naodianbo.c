#include "naodianbo.h"

u8 Uart2_data[];
u8 Uart2_Buffer[8];//存放小包数据
unsigned int Uart2_Rx = 0;
unsigned int Uart2_Tx = 0;
unsigned int Uart2_Len = 8;
unsigned int Uart2_Sta = 0;
unsigned int tx2;

unsigned int T1[];//眨眼的样本
unsigned int T2[];//传进来的rawdata数据组，不断进行更新，具体数组有多长还需要论证，还要不要把他放在rom中？
unsigned int a;//数组长度
unsigned int attention;//这个数据会不断更新，如果低于阈值会自动停车,阈值暂定为50
                         //还有大包的接收，现在还没做，姑且认为可以识别到大包
unsigned int signal;

//先配置蓝牙,USART2,PA2 PA3
void lanya_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    USART_DeInit(USART1);  //复位串口1      
    
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
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//这里要考虑
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    
    USART_Init(USART2, &USART_InitStructure);
    
    

    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                                               //选择中断分组2       
    NVIC_InitStructure.NVIC_IRQChannel= USART2_IRQn;                              //选择串口2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;                                 //抢占式中断优先级设置为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;                                            //响应式中断优先级设置为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                                 //使能中断
    NVIC_Init(&NVIC_InitStructure);
    
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
}

// 下面来配置蓝牙


int chuli(unsigned int Uart2_Buffer[8]);

unsigned int rawdata;//这是单个小包里的rawdata，总感觉应该把下面的两个函数写到 _it.c 文件里



/*下面的两个函数暂时移到it文件
//接下来处理传进来的数据，先验证校验值，然后获得rawdata
void panduan(unsigned int Uart2_Buffer[])
{
    if( Uart2_Sta )
    {
         if(Uart2_Rx == 8)
             if(Uart2_Buffer[7] == (((0x80 + 0x02 + Uart2_Buffer[5] + Uart2_Buffer[6])^ 0xFFFFFFFF) & 0xFF))
                  rawdata = chuli(Uart2_Buffer);
         
         Uart2_Rx = 0; //初始化
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
int sum(int x[],int a)//x为数据组，a为数组长度，求数组和
{
	unsigned int sum = 0;
	for (unsigned int i = 0;i != a;i++)
		sum += x[i];
	return sum;
}

//下面这个函数计算两个数组的相关系数并作为返回值返回，介于1和-1之间。1为完全正相关，0为不相关，-1为完全负相关，
//T1是样本数组，T2是接收到的数据组

double pearson(unsigned int T1[],unsigned int T2[],int a) //a为数组长度
{
      unsigned int xhe = sum(T1,a);
      unsigned int yhe = sum(T2,a);
      unsigned int xyhe = 0;
      unsigned int x2he = 0;
      unsigned int y2he = 0;
      int i = 0;
      double r;
      //所有的x*y加起来的和
      for(i = 0;i != a;i++)
      {
          xyhe += (T1[i]*T2[i]);
      }
      //所有x平方的和
      for(i = 0;i != a;i++)
      {
          x2he += (pow(T1[i],2));
      }
      //所有y平方的和
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

//下面的函数用来判断一秒内眨眼几次
//还需要改进


int howmany(unsigned int T1[],unsigned int T2[],int a)
{
      int times_time = 0;//用来在第一次眨眼后延时
      int times = 0;      //用来在函数中记录眨眼次数
      u8 time_return = 0;//用来返回眨眼次数
      
            switch(times)
            {
            case 0:
              {
                if(pearson(T1[],T2[],a) > 0.95)
                {
                  times = 1;
                  times_time = 0;
                  //要不要在这里清空T2（也就是存放rawdata的数组），或者说每一次成功识别眨眼后都清空T2
                }
                else
                {
                  time_return = 0;
                  break;
                }
              }
            case 1:
              {
                //这一行记得加上加延时函数，不知道延迟多久
                //这里还有问题，是不是要记录眨眼之后就自动清除数据，然后再来识别下一次，不然会立刻识别第二次
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

//下面的函数用来判断是否集中注意力
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

//下面这个程序是用脑电波控制电机的程序
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

