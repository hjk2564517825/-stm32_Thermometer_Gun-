#ifndef __LED_H //防止同一个.c文件内重复包含
#define __LED_H

#include "stm32f4xx.h"


//函数声明列表
void LED_PinConfig(void);
void LED_Control(int LEDn,char Status);

//定时器输出PWM控灯初始化函数
void TIM14CH1_Init(void);

#endif

