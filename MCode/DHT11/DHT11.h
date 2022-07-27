#ifndef __DHT11_H
#define __DHT11_H

#define DHT11_SET(n)  n ? GPIO_WriteBit(GPIOG,GPIO_Pin_9,Bit_SET): GPIO_WriteBit(GPIOG,GPIO_Pin_9,Bit_RESET)
#define DHT11_READ	  GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)

#include "stm32f4xx.h"
#include "delay.h"
#include "sys.h"
#include "stdint.h"

void  DHT11_Init(void);
void DHT11_SetInputMode(void);
void DHT11_Start(void);
uint8_t  DHT11_WaitAck(void);
uint8_t DHT11_ReadBit(void);
uint8_t DHT11_ReadByte(void);
uint8_t DHT11_GetVal(uint8_t *pbuf);


#endif
