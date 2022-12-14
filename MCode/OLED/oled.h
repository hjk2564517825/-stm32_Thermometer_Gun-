//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : Evk123
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : 0.69寸OLED 接口演示例程(STM32F103ZE系列IIC)
//              说明: 
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PD6（SCL）
//              SDA   接PD7（SDA）            
//              ----------------------------------------------------------------
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#ifndef __OLED_H
#define __OLED_H			  	 

#include "stdlib.h"	 
#include "stm32f4xx.h"
#include "sys.h"

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  

//IIC总线的引脚
#define  IIC_SDA_WRITE(n)   (n)?GPIO_SetBits(GPIOE,GPIO_Pin_15):GPIO_ResetBits(GPIOE,GPIO_Pin_15)
#define  IIC_SCL_WRITE(n)   (n)?GPIO_SetBits(GPIOD,GPIO_Pin_9):GPIO_ResetBits(GPIOD,GPIO_Pin_9)

#define  IIC_SDA_READ       GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15)


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
void Picture(void);



void IIC_Init(void);

void IIC_SDAOutputMode(void);
void IIC_SDAInputMode(void);
void IIC_StartSignal(void);
void IIC_StopSignal(void);
//主机等待从机应答  返回0 说明从机应答   返回1 说明从机没应答
uint8_t IIC_WaitAck(void);
//主机发送数据（从机读取数据）
void IIC_SendBytes(uint8_t Data) ;



#endif  
	 



