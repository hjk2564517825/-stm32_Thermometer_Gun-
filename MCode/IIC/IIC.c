#include "delay.h"
#include "IIC.h"
#include "stdlib.h"

/********************************************************************************************************************************************************
函 数 名：void IIC_init()
功    能：初始化
说    明：
入口参数：无
返 回 值：无
*********************************************************************************************************************************************************/
void IIC_red_Init(void)
{					     
	GPIO_InitTypeDef   GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;			//输出模式
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;		//输出速率
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;			//无上下拉
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8 | GPIO_Pin_9;				//引脚编号
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	IIC_SCL=1;
	IIC_SDA=1;
}
//产生IIC起始信号
/********************************************************************************************************************************************************
函 数 名：void IIC_Start(void)
功    能：IIC起始信号
说    明：
入口参数：无
返 回 值：无
*********************************************************************************************************************************************************/
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	
	delay_us(4);	
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	delay_us(4);
}	  
/********************************************************************************************************************************************************
函 数 名：void IIC_Stop()
功    能：IIC起始信号
说    明：
入口参数：无
返 回 值：无
*********************************************************************************************************************************************************/
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	delay_us(4);
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	delay_us(4);
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);									   	
}
/********************************************************************************************************************************************************
函 数 名：void IIC_Wait_Ack(void)
功    能：IIC等待应答信号
说    明：
入口参数：无
返 回 值：ack
*********************************************************************************************************************************************************/
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;delay_us(6);	   
	IIC_SCL=1;delay_us(6);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
/********************************************************************************************************************************************************
函 数 名：void IIC_Ack(void)
功    能：IIC产生应答信号
说    明：
入口参数：无
返 回 值：无
*********************************************************************************************************************************************************/
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(10);
	IIC_SCL=1;
	delay_us(10);
	IIC_SCL=0;
}
/********************************************************************************************************************************************************
函 数 名：void IIC_NAck(void)
功    能：IIC不产生应答信号
说    明：
入口参数：无
返 回 值：无
*********************************************************************************************************************************************************/
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(10);
	IIC_SCL=1;
	delay_us(10);
	IIC_SCL=0;
}				
/********************************************************************************************************************************************************
函 数 名：void IIC_Send_Byte(u8 txd)
功    能：IIC发送一个字节
说    明：
入口参数：u8 txd
返 回 值：无
*********************************************************************************************************************************************************/
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(10);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(10); 
		IIC_SCL=0;	
		delay_us(10);
    }	 
} 	 
/********************************************************************************************************************************************************
函 数 名：u8 IIC_Read_Byte(unsigned char ack)
功    能：读一个字节
说    明：
入口参数：unsigned char ack 
返 回 值：u8 receive
*********************************************************************************************************************************************************/
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(10);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(5); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

//void writemode(u8 address,u8 reg,u8 mode)
//{
//IIC_Start();
//IIC_Send_Byte(address);//发送IIC地址
//IIC_Wait_Ack();
//IIC_Send_Byte(reg);//发送寄存器2
//IIC_Wait_Ack();
//IIC_Send_Byte(mode);//发送命令
//IIC_Wait_Ack();
//IIC_Stop();
//delay_us(60);
//}
//u8 readdata(u8 address,u8 regdata)
//{
// u8 read=0;
// IIC_Start();
// IIC_Send_Byte(address);//发送IIC地址
// IIC_Wait_Ack();
// IIC_Send_Byte(regdata);
// IIC_Wait_Ack();
// IIC_Start();
// IIC_Send_Byte(address+1);
// IIC_Wait_Ack();
// delay_us(50);
// read=IIC_Read_Byte(0);
// IIC_Ack();
// IIC_Stop(); 
// return read;
//}
//u16 readKs103(u8 address)
//{
// u16 distance;
// writemode(address,0X02,0X06);
//	delay_ms(10);
//	//SCL_OUT();
// distance=readdata(address,0X02)<<8;
// delay_us(10);
// distance+=readdata(address,0X03);
// return distance*0.17;
//}
/********************************************************************************************************************************************************
函 数 名：float Get_Tem_DATA(U8 ReadAd)
功    能：IIC产生应答信号
说    明：
入口参数：无
返 回 值：无
*********************************************************************************************************************************************************/
float Get_Tem_DATA( u8 ReaAd)    //获取传感器所得温度值，℃，传入读取的RAM地址，输出摄氏度
{
	//从设备的地址 从0x00开始
   //使用读的过程 ：写读命令  ，每次从高位到低位 

   u8 Pecreg = 0;
	 u8 DataL = 0 ,DataH = 0;
	 u16 tem = 0;
	  float Temp = 0;
	
	  IIC_Start();
	
	  IIC_Send_Byte(0x00); //  主机先发送写命令写入地址
	  IIC_Wait_Ack();	
  	IIC_Send_Byte(ReaAd); //  RAM地址0x07可以获得温度的信息   

		IIC_Wait_Ack();
	//------------
		IIC_Start();
	  IIC_Send_Byte(0x01);  //主机发送读命令  	，从上面传送的地址中读取数据
  	IIC_Wait_Ack();
	
  	DataL=IIC_Read_Byte(1);
  	DataH=IIC_Read_Byte(1);
  	Pecreg=IIC_Read_Byte(1);
  	IIC_Stop();
  	tem = (DataH<<8) | DataL;;   //接收到传感器的16进制温度值
		//将16进制数值温度转换为℃  //不行，此处直接返回int型，会丢失小数数据，故还是返回浮点型
	Temp =  (((float)tem * 2) - 27315)/100;  //T= (DataH:DataL)*0.02-273.15  DataH:DataL=0x27AD,??? T=-70.01℃	  
	return Temp;
}








