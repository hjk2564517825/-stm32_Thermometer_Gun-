#include "main.h"

uint8_t dht_buf[5];  //保存获取的温湿度数据
uint8_t hjk_temp;//单温度数据
uint8_t hjk_humi;//单湿度数据

float	temperature; //红外温度

__IO uint8_t  RTC_TimeFlag = 0;  //表示获取时间标志  __IO 表示volatile 易变的


RTC_TimeTypeDef   RTC_TimeStructure;

int hour_num=0;
int minute_num=0;
int second_num=0;

int decimal_bcd_code(int decimal);

//#define DHT11_SET(n)  n ? GPIO_WriteBit(GPIOG,GPIO_Pin_9,Bit_SET): GPIO_WriteBit(GPIOG,GPIO_Pin_9,Bit_RESET)
//#define DHT11_READ	  GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)

////温湿度传感器的初始化
//void  DHT11_Init(void)
//{
//	GPIO_InitTypeDef GPIOInitStruct;
//	
//	//GPIOF时钟使能  应该将PG9配置为输出
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
//	
//	//PA0--KEY0引脚配置
//	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9;
//	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;		//输出模式
//	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;		//推挽输出
//	GPIOInitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//输出速率
//	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;	//片外已有上拉 片内可以不用
//	GPIO_Init(GPIOG,&GPIOInitStruct);
//	
//	//PG引脚输出高电平 表示空闲
//	DHT11_SET(1);
//}

////设置DHT11引脚为输入模式
//void DHT11_SetInputMode(void)
//{
//	GPIO_InitTypeDef GPIOInitStruct;
//	
//	//GPIOG时钟使能  应该将PG9配置为输入
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
//	
//	//引脚配置
//	GPIOInitStruct.GPIO_Pin  = GPIO_Pin_9;
//	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN;//输入模式
//	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//片外已有上拉 片内可以不用
//	GPIO_Init(GPIOG,&GPIOInitStruct);
//}

////DHT11开始信号
//void DHT11_Start(void)
//{
//	//1.配置DHT11引脚为输出模式
//	DHT11_Init();
//	
//	//2.把PG9引脚拉低 至少18ms
//	DHT11_SET(0);
//	delay_ms(20);
//	
//	//3.把PG9引脚拉高
//	DHT11_SET(1);
//	delay_us(30);
//}

////DHT11的响应信号  返回0 表示未响应  
//uint8_t  DHT11_WaitAck(void)
//{
//	int i = 0; //为了记录延时时间
//	
//	//1.配置PG9引脚为输入模式
//	DHT11_SetInputMode();
//	
//	//2.主机读取DHT11的低电平信号 等待低电平到来  超时处理 100us
//	while( DHT11_READ == 1 && i < 100)
//	{
//		delay_us(1);
//		i++;
//	}
//	
//	if(i>=100)
//	{
//		return 0; //未响应 
//	}
//	
//	i = 0; 
//	
//	//3.判断DHT11引脚是否拉低80us
//	while( DHT11_READ == 0 && i < 100)
//	{
//		delay_us(1);
//		i++;
//	}
//	
//	if(i>=100)
//	{
//		return 0; //未响应 
//	}
//	else
//		return 1; //已响应
//}

////DHT11读取1bit数据
//uint8_t DHT11_ReadBit(void)
//{
//	int  i = 0;  
//	
//	//1.等待引脚低电平到来
//	while( DHT11_READ == 1 && i < 100)
//	{
//		delay_us(1);
//		i++;
//	}
//	
//	i = 0;
//	//2.等待引脚高电平到来
//	while( DHT11_READ == 0 && i < 100)
//	{
//		delay_us(1);
//		i++;
//	}
//	
//	//3.延时一段时间  27us < ？ < 74us
//	delay_us(40);
//	
//	//4.判断延时之后的电平状态
//	if( DHT11_READ == 1)
//		return 1;  //数字1  8'b0000_0001
//	else
//		return 0;  //数字0  8'b0000_0000
//	
//}

////读取DHT11的1个字节
//uint8_t DHT11_ReadByte(void)
//{
//	uint8_t i = 0;
//	uint8_t data = 0; //data为了保存数据  data = 8'b0000_0000
//	
//	//高位先出
//	for(i=0;i<8;i++)
//	{
//		data <<= 1;	
//		data |= DHT11_ReadBit(); 			
//	}

//	return data;
//}

////获取温湿度数据  返回0 表示数据有误
//uint8_t DHT11_GetVal(uint8_t *pbuf)
//{
//	uint8_t i = 0;
//	
//	//1.发送开始信号
//	DHT11_Start();
//	
//	//2.等待响应信号
//	if( DHT11_WaitAck() == 1 ) //表示响应
//	{
//		//循环接收到5个字节 40bit
//		for(i=0;i<5;i++)
//		{
//			pbuf[i] = DHT11_ReadByte();		
//		}
//		//进行数据校验
//		if( pbuf[0] + pbuf[1] + pbuf[2] + pbuf[3] != pbuf[4] )
//		{
//			return 0;	//读取失败  原因是校验有误
//		}
//	}
//	else
//		return 0;	//读取失败  原因是未响应
//	
//	return 1;

//}

//独立看门狗的初始化
void IWDG_Init(void)
{
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* IWDG counter clock: LSI/32 LSI是32KHZ/32 = 1KHZ --->相当于1ms计数1次 */
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	
	//设置重载值 相当于250ms就是超时时间
	IWDG_SetReload(250-1);
	
	/* Reload IWDG counter */
	IWDG_ReloadCounter();
	
	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}

//RTC的配置
void RTC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	RTC_InitTypeDef   RTC_InitStructure;
	
	/* Enable the PWR APB1 Clock Interface 打开PWR外设的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to BKP Domain 解除RTC寄存器的写保护*/
	PWR_BackupAccessCmd(ENABLE);
	
	/* Enable the RTC Interrupt 设置RTC时钟的唤醒中断优先级的参数*/
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;			//唤醒通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* EXTI configuration 设置RTC自动唤醒的外部中断线的参数*/
	EXTI_ClearITPendingBit(EXTI_Line22);
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;				//中断线编号
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//上升沿有效
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable the LSE OSC 使能LSE低速外部时钟*/
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* Wait till LSE is ready 等待LSE准备完成*/  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	
	/* Select the RTC Clock Source 选择LSE作为RTC时钟源*/
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	/* Enable the RTC Clock 使能RTC时钟*/
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC APB registers synchronisation 等待RTC时钟同步完成*/
	RTC_WaitForSynchro();
	
	
	/*对RTC时钟进行初始化 ck_spre(1Hz) = RTCCLK(LSE) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)  计算RTC内部时钟 32768HZ / (127+1) / (255+1) = 1HZ */
	RTC_InitStructure.RTC_AsynchPrediv = 128-1;				//设置异步预分频器  32768HZ / 128 = 256HZ 
	RTC_InitStructure.RTC_SynchPrediv  = 256-1;				//设置同步预分频器  256 / 256 = 1HZ
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//采用24小时制
	RTC_Init(&RTC_InitStructure);

	
	/* Set the Time 设置RTC的时间*/
	RTC_TimeStructure.RTC_Hours   = 0x08;	//不是16进制   使用BCD码格式    16进制： 0x29+1 = 0x2A   BCD码:  0x29+1 = 0x30  10:00:00  0x10 --> 10
	RTC_TimeStructure.RTC_Minutes = 0x00;
	RTC_TimeStructure.RTC_Seconds = 0x00;
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);  
	
	/* Disable Wakeup Counter 禁止唤醒定时器*/
	RTC_WakeUpCmd(DISABLE);
	
	/* Configure the RTC Wakeup Clock source and Counter (Wakeup event each 1 second) 配置RTC唤醒周期  表示唤醒周期是内部时钟的周期  1HZ---> 1秒1次 */
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	/*设置自动重载值*/
	RTC_SetWakeUpCounter(0);  //RTCCLK = 1HZ  所以计数1次花费1s
	 
	/* Enable the Wakeup Interrupt 打开RTC唤醒中断*/
	RTC_ITConfig(RTC_IT_WUT, ENABLE);

	/* Enable Wakeup Counter 打开RTC唤醒定时器*/
	RTC_WakeUpCmd(ENABLE); 
	
	/*清除RTC时钟的唤醒标志 */
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	/*清除EXTI22外部中断线的标志 */
	EXTI_ClearITPendingBit(EXTI_Line22);
}

int main()
{
	char tbuf[40]={0};//时间
	char tbuf_temp_humi[20]={0};//温湿度
	char tbuf_red_Temp[10]={0};//红外
	
	//设置中断的优先级分组  组2则表示2bit抢占优先级（0~3）和2bit响应优先级（0~3） 该函数不应该频繁调用  应该放置在所有外设的初始化之前
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	USART1_Init(9600);	//串口1的初始化

	RTC_Config();		//RTC时钟的配置
	
	OLED_Init();			//初始化OLED  
	OLED_Clear() ; 
	//printf("mcu hardware init ok\r\n");
	DHT11_Init();
	IIC_red_Init();

	while(1)
	{
		temperature=Get_Tem_DATA(0x07);					//读取温度值
		DHT11_GetVal(dht_buf); //读取一次温湿度数据
		hjk_temp = dht_buf[2];
		hjk_humi = dht_buf[0];
		//如果满足了条件
		if(RTC_TimeFlag == 1)
		{
			RTC_TimeFlag = 0; 
			
			/* Get the current Time 获取时间 BCD格式  (0x10 / 16) * 10 + (0x10%16) = 10   */
			RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
			sprintf(tbuf,"%02x:%02x:%02x",RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);	
			OLED_ShowString(1,0,tbuf,16);
			
			sprintf(tbuf_temp_humi,"temp:%d,humi:%d",hjk_temp,hjk_humi);
			OLED_ShowString(1,3,tbuf_temp_humi,16);
			
			sprintf(tbuf_red_Temp,"%2.2f",temperature);
			OLED_ShowString(1,6,tbuf_red_Temp,16);
			OLED_ShowCHinese(45,6,3);
			//printf("%02x:%02x:%02x \r\n",RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);
		}
		

	}
	
	//return 0;
}

//RTC时钟的唤醒中断服务函数  每隔1s触发一次
void RTC_WKUP_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
  {  
		RTC_TimeFlag = 1;
	  
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
  }
}

void USART1_IRQHandler(void)
{
	char buf1[10] = {0};
	char buf2[10] = {0};
	char buf3[10] = {0};
	char buf4[10] = {0};
	char buf5[10] = {0};
	char* p = "+=:";
	static char Rdata[18]={0};//每次接收一个字符都会调用中断函数，因此需要设置为静态或全局
	static int i = 0;
	if(USART1->SR & (0x01<<5))
	{
		Rdata[i] = USART1->DR;
		i++;
		if(i==17)
		{
			i=0;
			
			//set
			char* ret = strtok(Rdata, p);//找到数组中的第一个标记并改为'\0'，strtok函数将保存它在字符串的中的位置
			printf("%s\n", ret);//返回初始位置
			strcpy(buf1, ret);
			
			//time
			ret = strtok(NULL, p);//找到下一个标记改为'\0',保存位置
			printf("%s\n", ret);//返回上一个标记的位置
			strcpy(buf2, ret);
			
			//时
			ret = strtok(NULL, p);
			printf("%s\n", ret);
			strcpy(buf3, ret);
			
			//分
			ret = strtok(NULL, p);
			printf("%s\n", ret);
			strcpy(buf4, ret);
			
			//秒
			ret = strtok(NULL, p);
			printf("%s\n", ret);
			strcpy(buf5, ret);
			 
			if(strcmp(buf2,"time")==0)
			{
				int hour_set;
				int min_set;
				int sed_set;
				
				hour_set=atoi(buf3);
				min_set=atoi(buf4);
				sed_set=atoi(buf5);
				
				hour_num=hour_set;
				minute_num=min_set;
				second_num=sed_set;
				
				hour_num=decimal_bcd_code(hour_num);
				minute_num=decimal_bcd_code(minute_num);
				second_num=decimal_bcd_code(second_num);
				
				/* Set the Time 设置RTC的时间*/
				RTC_TimeStructure.RTC_Hours   = hour_num;	//不是16进制   使用BCD码格式    16进制： 0x29+1 = 0x2A   BCD码:  0x29+1 = 0x30  10:00:00  0x10 --> 10
				RTC_TimeStructure.RTC_Minutes = minute_num;
				RTC_TimeStructure.RTC_Seconds = second_num;
				RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);  
					
			}
			
		}			
			
	}
}

int decimal_bcd_code(int decimal)//十进制转BCD码
{
	int sum = 0, i;//i计数变量，sum返回的BCD码
	for ( i = 0; decimal > 0; i++)
	{
		sum |= ((decimal % 10 ) << ( 4*i));
		decimal /= 10;
	}
	return sum;
}

