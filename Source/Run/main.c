/*******************************************************************************
工程文件的开始
所有头文件用<includes.h>包含
主函数文件进行时钟和中断的配置
*******************************************************************************/
#include "stm32f10x.h"
#include "ili9320.h"
#include "USART.h"
#include "init.h"
#include "GPS.h"

/****************************全局变量声明*************************************/
u8 USART1_RX_BUF[USART1_RX_LEN]; 				//USART1,接收缓存区
u8 USART1_TX_BUF[USART1_RX_LEN]; 				//USART1,发送缓存区

u8 USART2_RX_BUF[USART2_RX_LEN]; 				//USART2,接收缓存区
u8 USART2_TX_BUF[USART2_RX_LEN]; 				//USART2,发送缓存区

u32 USART1_RX_STA = 0;   								//USART1数据接收状态（SCBI协议）
u32 USART2_RX_STA = 0;   								//USART2数据接收状态（GPS模块）

u8 Tim1_Time;										 //TIM1定时器溢出计数
u16 Speed=200;										//PWM速度
u8 flag=0;

__IO u16 ADC_ConvertedValue; 						//ADC1转换的电压值通过DMA方式传到flash			 
__IO u16 ADC_ConvertedValueLocal; 				    // 局部变量，用于存从flash读到的电压值
u16 Extend=248;									    //扩展

extern GPS_INFO GPS;  //在display.c中定义，使用时要加extern

/*********************************主函数***************************************/
int main(void)
{
	u8 *str;
	RCC_Configuration();
	NVIC_Configuration();
	GPIO_Configuration();
	Usart1_Init();			   		            	
	ili9320_Initializtion();
	ili9320_Clear(0xffff); 
/*	   
	while(1)
	{ 
	    GUI_Text(120,10,(u8 *)"GPS DATA",8,Red,White);
		GUI_Text(40,30,(u8 *)"Longitude:",10,Blue,White);
		GUI_Text(40,50,(u8 *)"Latitude:",9,Blue,White);
		GUI_Text(40,70,(u8 *)"Altitude:",9,Blue,White);
		GUI_Text(40,90,(u8 *)"Height:",7,Blue,White);
		GUI_Text(40,110,(u8 *)"Speed:",6,Blue,White);
		GUI_Text(40,130,(u8 *)"Direction:",10,Blue,White);
		GUI_Text(40,150,(u8 *)"DateTime:",9,Blue,White);
		GUI_Text(40,170,(u8 *)"PosNum:",7,Blue,White);
		GUI_Text(40,190,(u8 *)"HDOP:",5,Blue,White);
		GUI_Text(40,210,(u8 *)"VVOP:",5,Blue,White);
	   
	    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
		if(flag)
		{
		   USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
		   GPS_RMC_Parse(USART1_RX_BUF, &GPS);
//		   GPS_GGA_Parse(USART1_RX_BUF, &GPS);
	       flag = 0;
		   Delay(3000);
//		   ili9320_Clear(0xffff);   	
	  }
//    if(USART1_RX_STA&0x80)
//			{					   
//			len=USART1_RX_STA&0x3f;//得到此次接收到的数据长度
//			printf("\n您发送的消息为:\n");
//			for(t=0 ;t<len ;t++ )
//			{
//			USART_SendData(USART1, (uint8_t)USART1_RX_BUF[t]);	
//         	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}
//			}
//			printf("\n\n");//插入换行
//			USART1_RX_STA = 0;
//			}	   

    }		
*/
}


