#include "tim.h"


void TIM6_Init(void)
{
	TIM_TimeBaseInitTypeDef TIMInitStruct;
	NVIC_InitTypeDef NVICInitStruct;
	
	//使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	//配置定时器时基单元
//	TIM6->PSC = 8400-1;//84Mhz/8400 =10000Hz 1/10ms 0.1ms/脉冲 
//	TIM6->ARR = 65536-1;//10000*0.1ms = 1000ms = 1s
	TIMInitStruct.TIM_Prescaler     = 8400-1;//PSC
	TIMInitStruct.TIM_Period        = 10000-1;//ARR 0.2s
	TIMInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//递增
	TIMInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInit(TIM6,&TIMInitStruct);
	
	
	//开启TIM6 更新中断
	TIM6->DIER |= 0x01;
	
	//NVIC中断管理---给刚刚开启的中断的优先级配置
	NVICInitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;//54号
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0x00;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVICInitStruct);
	
	//开启定时器计数器
	TIM6->CR1 |= 0x01;
}

void TIM6_DAC_IRQHandler(void)
{
	
	if(TIM6->SR & 0x01) //判断更新标志位是否置位
	{
		//时间到了
		GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
		TIM6->SR = 0;//写0清零  CNT会自动更新为初值0
	}

}



void TIM7_Init(void)
{
	TIM_TimeBaseInitTypeDef TIMInitStruct;
	//使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	
	//配置定时器时基单元
//	TIM6->PSC = 8400-1;//84Mhz/8400 =10000Hz 1/10ms 0.1ms/脉冲 
//	TIM6->ARR = 65536-1;//10000*0.1ms = 1000ms = 1s
	TIMInitStruct.TIM_Prescaler     = 8400-1;//PSC
	TIMInitStruct.TIM_Period        = 10000-1;//ARR
	TIMInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//递增
	TIMInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInit(TIM7,&TIMInitStruct);
	
	
	//开启定时器计数器
	TIM7->CR1 |= 0x01;
}

