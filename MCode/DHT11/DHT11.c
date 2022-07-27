#include "DHT11.h"



//温湿度传感器的初始化
void  DHT11_Init(void)
{
	GPIO_InitTypeDef GPIOInitStruct;
	
	//GPIOF时钟使能  应该将PG9配置为输出
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	
	//PA0--KEY0引脚配置
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;		//输出模式
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//输出速率
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;	//片外已有上拉 片内可以不用
	GPIO_Init(GPIOG,&GPIOInitStruct);
	
	//PG引脚输出高电平 表示空闲
	DHT11_SET(1);
}

//设置DHT11引脚为输入模式
void DHT11_SetInputMode(void)
{
	GPIO_InitTypeDef GPIOInitStruct;
	
	//GPIOG时钟使能  应该将PG9配置为输入
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	
	//引脚配置
	GPIOInitStruct.GPIO_Pin  = GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//片外已有上拉 片内可以不用
	GPIO_Init(GPIOG,&GPIOInitStruct);
}


//DHT11开始信号
void DHT11_Start(void)
{
	//1.配置DHT11引脚为输出模式
	DHT11_Init();
	
	//2.把PG9引脚拉低 至少18ms
	DHT11_SET(0);
	delay_ms(20);
	
	//3.把PG9引脚拉高
	DHT11_SET(1);
	delay_us(30);
}

//DHT11的响应信号  返回0 表示未响应  
uint8_t  DHT11_WaitAck(void)
{
	int i = 0; //为了记录延时时间
	
	//1.配置PG9引脚为输入模式
	DHT11_SetInputMode();
	
	//2.主机读取DHT11的低电平信号 等待低电平到来  超时处理 100us
	while( DHT11_READ == 1 && i < 100)
	{
		delay_us(1);
		i++;
	}
	
	if(i>=100)
	{
		return 0; //未响应 
	}
	
	i = 0; 
	
	//3.判断DHT11引脚是否拉低80us
	while( DHT11_READ == 0 && i < 100)
	{
		delay_us(1);
		i++;
	}
	
	if(i>=100)
	{
		return 0; //未响应 
	}
	else
		return 1; //已响应
}

//DHT11读取1bit数据
uint8_t DHT11_ReadBit(void)
{
	int  i = 0;  
	
	//1.等待引脚低电平到来
	while( DHT11_READ == 1 && i < 100)
	{
		delay_us(1);
		i++;
	}
	
	i = 0;
	//2.等待引脚高电平到来
	while( DHT11_READ == 0 && i < 100)
	{
		delay_us(1);
		i++;
	}
	
	//3.延时一段时间  27us < ？ < 74us
	delay_us(40);
	
	//4.判断延时之后的电平状态
	if( DHT11_READ == 1)
		return 1;  //数字1  8'b0000_0001
	else
		return 0;  //数字0  8'b0000_0000
	
}

//读取DHT11的1个字节
uint8_t DHT11_ReadByte(void)
{
	uint8_t i = 0;
	uint8_t data = 0; //data为了保存数据  data = 8'b0000_0000
	
	//高位先出
	for(i=0;i<8;i++)
	{
		data <<= 1;	
		data |= DHT11_ReadBit(); 			
	}

	return data;
}

//获取温湿度数据  返回0 表示数据有误
uint8_t DHT11_GetVal(uint8_t *pbuf)
{
	uint8_t i = 0;
	
	//1.发送开始信号
	DHT11_Start();
	
	//2.等待响应信号
	if( DHT11_WaitAck() == 1 ) //表示响应
	{
		//循环接收到5个字节 40bit
		for(i=0;i<5;i++)
		{
			pbuf[i] = DHT11_ReadByte();		
		}
		//进行数据校验
		if( pbuf[0] + pbuf[1] + pbuf[2] + pbuf[3] != pbuf[4] )
		{
			return 0;	//读取失败  原因是校验有误
		}
	}
	else
		return 0;	//读取失败  原因是未响应
	
	return 1;

}
	
