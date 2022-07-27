#include "led.h"


/*
函数功能：LED引脚初始化
参数1：viod
返回值：void
*/
void LED_PinConfig(void)
{
	GPIO_InitTypeDef p;
	
	//这段下午再讲--先开启给GPIOE  GPIPF寄存器时钟供给使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	//使用库函数完成对GPIO的配置
	p.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	p.GPIO_Mode  = GPIO_Mode_OUT;//输出
	p.GPIO_OType = GPIO_OType_PP;//推挽
	p.GPIO_Speed = GPIO_Low_Speed;//低速 
	p.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空
	GPIO_Init(GPIOF,&p);
	
	p.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOE,&p);
	
	//灯初始化后全灭
	GPIO_WriteBit(GPIOF,GPIO_Pin_9 |GPIO_Pin_10,(BitAction)1);
	GPIO_WriteBit(GPIOE,GPIO_Pin_13|GPIO_Pin_14,(BitAction)1);
}

/*
函数功能：LED引脚控制函数
参数1：LEDn 指定LED引脚 0~3 即LED0~LED3
参数2：Status 指定的LED灯状态 0-DISABLE:灭  1-ENABLE:亮
返回值：void
*/
void LED_Control(int LEDn,char Status)
{
	switch(LEDn)
	{
		case 0:
			GPIO_WriteBit(GPIOF,GPIO_Pin_9,(BitAction)!Status);
			break;
		case 1:
			GPIO_WriteBit(GPIOF,GPIO_Pin_10,(BitAction)!Status);
			break;
		case 2:
			GPIO_WriteBit(GPIOE,GPIO_Pin_13,(BitAction)!Status);
			break;
		case 3:
			GPIO_WriteBit(GPIOE,GPIO_Pin_14,(BitAction)!Status);
			break;
	}
}



void TIM14CH1_Init(void)
{
	GPIO_InitTypeDef        GPIOInitStruct;
	TIM_TimeBaseInitTypeDef TIMInitStruct;
	TIM_OCInitTypeDef       OCInitStruct;
	//使能时钟GPIOF  TIM14
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	
	//配置GPIOF 14引脚为复用AF模式
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AF;//复用模式
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//低速足够
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//片外已有上拉 片内可以不用
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
	//将PF9复用为指定功能---TIM14
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14);
	
	//定时器时基单元
	TIMInitStruct.TIM_Prescaler     = 84-1;//PSC 1us
	TIMInitStruct.TIM_Period        = 1000-1;//ARR 1ms周期
	TIMInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//递增
	TIMInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInit(TIM14,&TIMInitStruct);
	
	//定时器的输出比较设置
	OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//PWM1  CNT<CCR 输出有效  否则输出无效
	OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;//低电平有效
	OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//PWM输出使能
	OCInitStruct.TIM_Pulse       = 0;//默认100%高  灯全灭  
	TIM_OC1Init(TIM14,&OCInitStruct);
	
	//开启定时器
	TIM_Cmd(TIM14,ENABLE);
}
	























