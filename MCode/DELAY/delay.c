#include "delay.h"

//粗延时
void delay(int tim)
{
	int i;
	while(tim--)
		for(i=38400;i>0;i--);
}

//参考SysTick实现的us精准延时
void delay_us(int n)
{
	//选择SysTick时钟源---21Mhz 1/21us
	SysTick->CTRL &= ~(0x01<<2);
	
	//根据你要延时的us得到要计的脉冲个数
	SysTick->LOAD  = 21*n-1;
	
	//将这个计数值丢到VAL内  VAL写入任何数都会清零-->自动从LOAD加载值过来
	//该步骤也可以将CTRL的标志位清零
	SysTick->VAL  = 0;
	
	//开启SysTick计数器
	SysTick->CTRL |= 0x01;
	
	//等待指定时间到达----等延时结束--每次读完都清除标志位
	while(!(SysTick->CTRL & (0x01<<16)));
	
	//关闭SysTick
	SysTick->CTRL &= ~0x01;
}

//LOAD最大：16777216/21000 >= n    n<=798ms
void delay_ms(int n)
{
	//选择SysTick时钟源---21Mhz 1/21us
	SysTick->CTRL &= ~(0x01<<2);
	
	//根据你要延时的us得到要计的脉冲个数
	SysTick->LOAD  = 21000*n-1;
	
	//将这个计数值丢到VAL内  VAL写入任何数都会清零-->自动从LOAD加载值过来
	//该步骤也可以将CTRL的标志位清零
	SysTick->VAL  = 0;
	
	//开启SysTick计数器
	SysTick->CTRL |= 0x01;
	
	//等待指定时间到达----等延时结束--每次读完都清除标志位
	while(!(SysTick->CTRL & (0x01<<16)));
	
	//关闭SysTick
	SysTick->CTRL &= ~0x01;

}

void delay_s(int n)
{
	while(n--)
	{
		delay_ms(500);
		delay_ms(500);
	}
}

//void delay_ms(int n)
//{
//	TIM_TimeBaseInitTypeDef TIMInitStruct;
//	
//	//使能时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
//	
//	//配置定时器时基单元
//	TIMInitStruct.TIM_Prescaler     = 8400-1;//PSC 0.1ms/脉冲  10个脉冲够1ms
//	TIMInitStruct.TIM_Period        = 65536-1;//我们不希望CNT在做精准延时时。提前溢出
//	TIMInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//递增
//	TIMInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频
//	TIM_TimeBaseInit(TIM6,&TIMInitStruct);
//	
//	TIM6->CNT = 0;//先清零
//	
//	//开启计数器
//	TIM6->CR1 |= 0x01;
//	
//	//10*n<=65536  n<= 6553.6ms
//	while(TIM6->CNT != (10*n-1));//等待CNT计到指定脉冲值---延时一定时间

//}




////用TIM6实现us级别延时，可以根据传参决定延时多少us
//void delay_us(int n)
//{
//	
//	
//	
//	
//}


////实现s级别延时，可以根据传参决定延时多少us
//void delay_s(int n)
//{
//	
//	
//	
//	
//}


