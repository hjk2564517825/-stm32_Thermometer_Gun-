#ifndef __MAIN_H
#define __MAIN_H

extern int hour_num;
extern int minute_num;
extern int second_num;



#include "stm32f4xx.h"//这个头文件已经帮我们定义好了所有的外设寄存器
#include "led.h"//LED灯相关
#include "key.h"
#include "beep.h"
#include "delay.h"
#include "tim.h"
#include "sys.h"
#include "usart.h"
#include <stdio.h>
#include "oled.h"
#include "stdlib.h"
#include "DHT11.h"
#include "IIC.h"


#endif
