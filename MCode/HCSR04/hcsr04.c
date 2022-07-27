#include "hcsr04.h"

#define TRIG PBout(10)
#define ECHO PBin(11)

void HCSR04_Init(void)
{
	//GPIO  SysTick在测距时再配置
	GPIO_InitTypeDef GPIOInitStruct;
	
	//GPIOA时钟使能  应该将PA0配置为输入
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//PB10--TRIG 输出
	GPIOInitStruct.GPIO_Pin  = GPIO_Pin_10;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_OUT;//输入模式
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//低速完全足够
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//片外已有上拉 片内可以不用
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin  = GPIO_Pin_11;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_Init(GPIOB,&GPIOInitStruct);
}

static void SysTick_HCSR04Init(void)
{
	//选择SysTick时钟源---21Mhz 1/21us
	SysTick->CTRL &= ~(0x01<<2);
	
	//先关闭
	SysTick->CTRL &= ~0x01;
	
	//LOAD
	SysTick->LOAD = 1000000;
	SysTick->VAL  = 0;
	
}

//测距函数
float HCSR04_GetDistance(void)
{
	//启动超声波发出声波
	TRIG = 1;
	delay_us(15);//TRIG拉高至少10us
	TRIG = 0;
	
	//配置SysTick
	SysTick_HCSR04Init();//从1000000开始递减
	
	//等待ECHo拉高
	while(ECHO == 0);
	//开始计时--开启滴答定时器
	SysTick->CTRL |= 0x01;
	
	//等到ECHO被拉低
	while(ECHO == 1);
	//关闭SysTick---SysTick->VAL定格
	SysTick->CTRL &= ~0x01;

	//读出VAL  340m/s  0.34mm/us
	//计的脉冲个数：(1000000-(SysTick->VAL)) 单位：个
	//t = (1000000-(SysTick->VAL))*(1/21)    单位：us
	//s = t*v = t*0.34
	//s = s/2  因为计算的是双程
	return ((1000000-(SysTick->VAL))/21)*0.17;
}


