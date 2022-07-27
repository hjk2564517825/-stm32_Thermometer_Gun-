#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"
#include "delay.h"
#include "sys.h"
#include <stdio.h>

#include <string.h>
#include <stdlib.h>

void USART1_Init(unsigned int Baud);
void USART1_SendString(char *str);
#endif
