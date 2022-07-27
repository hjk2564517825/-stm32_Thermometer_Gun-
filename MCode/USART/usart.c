#include "usart.h"



void USART1_Init(unsigned int Baud)
{
	GPIO_InitTypeDef        GPIOInitStruct;
	USART_InitTypeDef       USARTInitStructure;
	NVIC_InitTypeDef        NVICInitStruct;
	
	//①时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//②将指定引脚复用为USART1功能  PA9 PA10
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//③配置PA9 PA10为AF复用功能
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AF;//复用模式
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//低速足够 2Mhz
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//片外已有上拉 片内可以不用
	GPIO_Init(GPIOA,&GPIOInitStruct);
	
	//④串口的过采样倍数选择  ENABLE--8倍  DISABLE--16倍
	USART_OverSampling8Cmd(USART1, ENABLE);  
	
	//⑤配置串口1
	USARTInitStructure.USART_BaudRate   = Baud;//波特率设置
	USARTInitStructure.USART_WordLength = USART_WordLength_8b;
	USARTInitStructure.USART_StopBits   = USART_StopBits_1;
	USARTInitStructure.USART_Parity     = USART_Parity_No;//无校验
	USARTInitStructure.USART_Mode       = USART_Mode_Tx | USART_Mode_Rx;//收发模式
	USARTInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控
	USART_Init(USART1, &USARTInitStructure);
	
	//开启串口的接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//中断优先级配置
	NVICInitStruct.NVIC_IRQChannel = USART1_IRQn;//37号
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0x00;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVICInitStruct);
	

	//⑥开启串口
	USART_Cmd(USART1,ENABLE);
}




void USART1_SendString(char *str)
{
	while(*str != '\0')
	{
		USART1->DR = *str;
		str++;
		//等当前字符发完  发送数据寄存器为空标志 ---- 置位即可
		while(!(USART1->SR & (0x01<<7)));
	}
}


//重定向printf底层函数 fputc
int fputc(int ch, FILE * fp)
{
	USART1->DR = (u8)ch;
	while(!(USART1->SR & (0x01<<7)));//等待当前字符发完
	
	return ch;//返回啥任意
}


