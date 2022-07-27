//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ÖÐ¾°Ô°µç×Ó
//µêÆÌµØÖ·£ºhttp://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  ÎÄ ¼þ Ãû   : main.c
//  °æ ±¾ ºÅ   : v2.0
//  ×÷    Õß   : Evk123
//  Éú³ÉÈÕÆÚ   : 2014-0101
//  ×î½üÐÞ¸Ä   : 
//  ¹¦ÄÜÃèÊö   : 0.69´çOLED ½Ó¿ÚÑÝÊ¾Àý³Ì(STM32F103ZEÏµÁÐIIC)
//              ËµÃ÷: 
//              ----------------------------------------------------------------
//              GND   µçÔ´µØ
//              VCC   ½Ó5V»ò3.3vµçÔ´
//              SCL   ½ÓPD9£¨SCL£©
//              SDA   ½ÓPE15£¨SDA£©            
//              ----------------------------------------------------------------
//Copyright(C) ÖÐ¾°Ô°µç×Ó2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////£

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"

//OLEDµÄÏÔ´æ
//´æ·Å¸ñÊ½ÈçÏÂ.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
/**********************************************
//IIC Start
**********************************************/
/**********************************************
//IIC Start
**********************************************/


//IIC×ÜÏßµÄ³õÊ¼»¯  SCL--PB8  SDA--PB9
void IIC_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;			//Êä³öÄ£Ê½
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;			//ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;		//Êä³öËÙÂÊ
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;			//ÎÞÉÏÏÂÀ­
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;				//Òý½Å±àºÅ
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;				//Òý½Å±àºÅ
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	

	//3.¿ÕÏÐ×´Ì¬ÏÂSDAºÍSCL¶¼Îª¸ßµçÆ½
	IIC_SCL_WRITE(1);
	IIC_SDA_WRITE(1);
	
}	


void IIC_SDAOutputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//1.´ò¿ªGPIOÍâÉèÊ±ÖÓ
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	
	//2.ÅäÖÃGPIOÒý½Å
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;									//Òý½Å±àºÅ
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;								//Êä³öÄ£Ê½
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;								//ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;							//Êä³öËÙÂÊ
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;									//ÉÏÀ­µç×è
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	

}

void IIC_SDAInputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//1.´ò¿ªGPIOÍâÉèÊ±ÖÓ
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	
	//2.ÅäÖÃGPIOÒý½Å
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;									//Òý½Å±àºÅ
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;									//ÊäÈëÄ£Ê½
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;									//ÉÏÀ­µç×è
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	

}

//IIC×ÜÏß¿ªÊ¼ÐÅºÅ
void IIC_StartSignal(void)
{
	//1.ÉèÖÃSDAÒý½ÅÎªÊä³öÄ£Ê½
	IIC_SDAOutputMode();
	
	//2.È·±£SDAºÍSCLÎª¸ßµçÆ½
	IIC_SCL_WRITE(1);
	IIC_SDA_WRITE(1);

    delay_us(5);            //Ìá¸ß³ÌÐò¿É¿¿ÐÔ
    
	//3.°ÑSDAÒý½ÅÀ­µÍ
	IIC_SDA_WRITE(0);

	delay_us(5);   // IIC×ÜÏßµÄÍ¨ÐÅËÙÂÊÎª100KHZ  1000000us = 100000HZ  10us = 1HZ 
	
	//4.°ÑSCLÒý½ÅÀ­µÍ£¬±íÊ¾×¼±¸Í¨ÐÅ
	IIC_SCL_WRITE(0);
}

//IIC×ÜÏßÍ£Ö¹ÐÅºÅ
void IIC_StopSignal(void)
{
	//1.ÉèÖÃSDAÒý½ÅÎªÊä³öÄ£Ê½
	IIC_SDAOutputMode();

	//2.È·±£SDAºÍSCLÎªµÍµçÆ½
	IIC_SCL_WRITE(0);
	IIC_SDA_WRITE(0);
	
	//4.°ÑSCLÒý½ÅÀ­¸ß
	IIC_SCL_WRITE(1);
	delay_us(5);
	
	//5.°ÑSDAÒý½ÅÀ­¸ß
	IIC_SDA_WRITE(1);
	delay_us(5);  //È·±£SDAµÄµçÆ½×´Ì¬¿ÉÒÔ±»ÆäËû´ÓÆ÷¼þ¼ì²âµ½
}

//Ö÷»úµÈ´ý´Ó»úÓ¦´ð  ·µ»Ø0 ËµÃ÷´Ó»úÓ¦´ð   ·µ»Ø1 ËµÃ÷´Ó»úÃ»Ó¦´ð
uint8_t IIC_WaitAck(void)
{
	uint8_t ack;
	
	//1.ÉèÖÃSDAÒý½ÅÎªÊäÈëÄ£Ê½
	IIC_SDAInputMode();

	//2.SCLÎªµÍµçÆ½      ´Ó»ú×¼±¸Êý¾Ý
	IIC_SCL_WRITE(0);
	delay_us(5);
	
	//3.SCLÎª¸ßµçÆ½      ´Ó»ú·¢ËÍÊý¾Ý
	IIC_SCL_WRITE(1);
	delay_us(5);
	
	//Ö÷»úÅÐ¶Ï´Ó»úµÄÊý¾Ý
	if( IIC_SDA_READ == 1)
	{
		ack=1; //ËµÃ÷ÎÞÓ¦´ð
	}
	else
		ack=0; //ËµÃ÷ÒÑÓ¦´ð
	
	//4.SCLÎªµÍµçÆ½       Ö÷»úºöÂÔÊý¾Ý
	IIC_SCL_WRITE(0);
	delay_us(5);
	
	return ack;
}

//Ö÷»ú·¢ËÍÊý¾Ý£¨´Ó»ú¶ÁÈ¡Êý¾Ý£©
void IIC_SendBytes(uint8_t Data)  
{
	uint8_t i= 0;
	
	//1.ÉèÖÃSDAÒý½ÅÎªÊä³öÄ£Ê½
	IIC_SDAOutputMode();
	
	//2.È·±£SDAºÍSCLÎªµÍµçÆ½
	IIC_SCL_WRITE(0);
	IIC_SDA_WRITE(0);
	
	//3.Ñ­»··¢ËÍbit
	for(i=0;i<8;i++)
	{
		//MSB ¸ßÎ»ÏÈ³ö   Ö÷»ú×¼±¸Êý¾Ý
		if ( Data & 1<<(7-i) ) 
		{
			IIC_SDA_WRITE(1);
		}
		else
			IIC_SDA_WRITE(0);
		
		delay_us(5);
		
		//SCLÎª¸ßµçÆ½    Ö÷»ú·¢ËÍÊý¾Ý
		IIC_SCL_WRITE(1);
		delay_us(5);
		
		//SCLÎªµÍµçÆ½    Ö÷»ú×¼±¸Êý¾Ý
		IIC_SCL_WRITE(0);
		delay_us(5);
	}
}


/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_StartSignal();
	IIC_SendBytes(0x78);            //Slave address,SA0=0
	IIC_WaitAck();	
	IIC_SendBytes(0x00);			//write command
	IIC_WaitAck();	
	IIC_SendBytes(IIC_Command); 
	IIC_WaitAck();	
	IIC_StopSignal();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_StartSignal();
	IIC_SendBytes(0x78);			//D/C#=0; R/W#=0
	IIC_WaitAck();	
	IIC_SendBytes(0x40);			//write data
	IIC_WaitAck();	
	IIC_SendBytes(IIC_Data);
	IIC_WaitAck();	
	IIC_StopSignal();
}




void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
			{

   Write_IIC_Data(dat);
   
		}
	else {
   Write_IIC_Command(dat);
		
	}


}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,1);
			}
	}
}


/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}

//×ø±êÉèÖÃ

	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//¿ªÆôOLEDÏÔÊ¾    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDCÃüÁî
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//¹Ø±ÕOLEDÏÔÊ¾     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDCÃüÁî
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//ÇåÆÁº¯Êý,ÇåÍêÆÁ,Õû¸öÆÁÄ»ÊÇºÚÉ«µÄ!ºÍÃ»µãÁÁÒ»Ñù!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //ÉèÖÃÒ³µØÖ·£¨0~7£©
		OLED_WR_Byte (0x00,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁÐµÍµØÖ·
		OLED_WR_Byte (0x10,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁÐ¸ßµØÖ·   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //¸üÐÂÏÔÊ¾
}
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //ÉèÖÃÒ³µØÖ·£¨0~7£©
		OLED_WR_Byte (0x00,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁÐµÍµØÖ·
		OLED_WR_Byte (0x10,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁÐ¸ßµØÖ·   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //¸üÐÂÏÔÊ¾
}
//ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸ö×Ö·û,°üÀ¨²¿·Ö×Ö·û
//x:0~127
//y:0~63
//mode:0,·´°×ÏÔÊ¾;1,Õý³£ÏÔÊ¾				 
//size:Ñ¡Ôñ×ÖÌå 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//µÃµ½Æ«ÒÆºóµÄÖµ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^nº¯Êý
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//ÏÔÊ¾2¸öÊý×Ö
//x,y :Æðµã×ø±ê	 
//len :Êý×ÖµÄÎ»Êý
//size:×ÖÌå´óÐ¡
//mode:Ä£Ê½	0,Ìî³äÄ£Ê½;1,µþ¼ÓÄ£Ê½
//num:ÊýÖµ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//ÏÔÊ¾Ò»¸ö×Ö·ûºÅ´®
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//ÏÔÊ¾ºº×Ö
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********¹¦ÄÜÃèÊö£ºÏÔÊ¾ÏÔÊ¾BMPÍ¼Æ¬128¡Á64ÆðÊ¼µã×ø±ê(x,y),xµÄ·¶Î§0¡«127£¬yÎªÒ³µÄ·¶Î§0¡«7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//³õÊ¼»¯SSD1306	  SCL  PD9   SDA PE15				    
void OLED_Init(void)
{ 	
 

	GPIO_InitTypeDef   GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;			//Êä³öÄ£Ê½
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;			//ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;		//Êä³öËÙÂÊ
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;			//ÎÞÉÏÏÂÀ­
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;				//Òý½Å±àºÅ
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;				//Òý½Å±àºÅ
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOD,GPIO_Pin_9);
	GPIO_SetBits(GPIOE,GPIO_Pin_15);

	delay_ms(200);

	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  





























