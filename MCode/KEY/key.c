#include "key.h"

void KEY_PinConfig(void)
{
	GPIO_InitTypeDef GPIOInitStruct;
	
	//GPIOA时钟使能  应该将PA0配置为输入
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	//PA0--KEY0引脚配置
	GPIOInitStruct.GPIO_Pin  = GPIO_Pin_2;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//片外已有上拉 片内可以不用
	GPIO_Init(GPIOE,&GPIOInitStruct);

}

//外部中断EXTI0配置
void KEY_EXTIInit(void)
{
	EXTI_InitTypeDef EXTIInitStruct;
	NVIC_InitTypeDef NVICInitStruct;
	
	//①使能时钟 GPIOA SYSCFG 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);

	//②将引脚PA0配置为浮空输入
	KEY_PinConfig();

	//③选择指定引脚连接到指定的外部中断线
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2);

	//④配置EXTI
	EXTIInitStruct.EXTI_Line = EXTI_Line2;
	EXTIInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTIInitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发---按下触发
	EXTIInitStruct.EXTI_LineCmd = ENABLE;//开启中断
	EXTI_Init(&EXTIInitStruct);
	
	//⑤NVIC中断管理---给刚刚开启的中断的优先级配置
	NVICInitStruct.NVIC_IRQChannel = EXTI2_IRQn;//8号
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0x00;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVICInitStruct);

}

//EXTI2 的中断服务函数--该函数在中断触发时。CPU直接跳转过来
void EXTI2_IRQHandler(void)
{
	delay(50);//消抖
	if(EXTI->PR & (0x01<<2))
	{
		//按键每按下一次 LED0翻转一次
		GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
		
		EXTI->PR = (0x01<<2);//对位写1清零 写0 不影响  必须清零
		//EXTI_PR(2) = 1;//写1清零
	}
}














