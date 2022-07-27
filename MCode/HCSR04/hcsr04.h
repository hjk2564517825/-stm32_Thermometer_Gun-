#ifndef __HCSR04_H
#define __HCSR04_H

#include "stm32f4xx.h"
#include "delay.h"
#include "sys.h"

void  HCSR04_Init(void);
float HCSR04_GetDistance(void);

#endif
