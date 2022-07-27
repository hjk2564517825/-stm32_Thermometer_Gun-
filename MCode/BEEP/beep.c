#include "beep.h"

//蜂鸣器引脚配置函数
void Beep_PinConfig(void)
{
	GPIO_InitTypeDef GPIOInitStruct;
	
	//GPIOF时钟使能  应该将PF8配置为输出
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	//PA0--KEY0引脚配置
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_8;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;//低速
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//片外已有上拉 片内可以不用
	GPIO_Init(GPIOF,&GPIOInitStruct);
}

