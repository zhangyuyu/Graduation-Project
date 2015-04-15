/*******************************************************************************
�����ļ��Ŀ�ʼ
����ͷ�ļ���<includes.h>����
�������ļ�����ʱ�Ӻ��жϵ�����
*******************************************************************************/
#include "stm32f10x.h"
#include "ili9320.h"
#include "USART.h"
#include "init.h"
#include "GPS.h"

/****************************ȫ�ֱ�������*************************************/
u8 USART1_RX_BUF[USART1_RX_LEN]; 				//USART1,���ջ�����
u8 USART1_TX_BUF[USART1_RX_LEN]; 				//USART1,���ͻ�����

u8 USART2_RX_BUF[USART2_RX_LEN]; 				//USART2,���ջ�����
u8 USART2_TX_BUF[USART2_RX_LEN]; 				//USART2,���ͻ�����

u32 USART1_RX_STA = 0;   								//USART1���ݽ���״̬��SCBIЭ�飩
u32 USART2_RX_STA = 0;   								//USART2���ݽ���״̬��GPSģ�飩

u8 Tim1_Time;										 //TIM1��ʱ���������
u16 Speed=200;										//PWM�ٶ�
u8 flag=0;

__IO u16 ADC_ConvertedValue; 						//ADC1ת���ĵ�ѹֵͨ��DMA��ʽ����flash			 
__IO u16 ADC_ConvertedValueLocal; 				    // �ֲ����������ڴ��flash�����ĵ�ѹֵ
u16 Extend=248;									    //��չ

extern GPS_INFO GPS;  //��display.c�ж��壬ʹ��ʱҪ��extern

/*********************************������***************************************/
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
//			len=USART1_RX_STA&0x3f;//�õ��˴ν��յ������ݳ���
//			printf("\n�����͵���ϢΪ:\n");
//			for(t=0 ;t<len ;t++ )
//			{
//			USART_SendData(USART1, (uint8_t)USART1_RX_BUF[t]);	
//         	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}
//			}
//			printf("\n\n");//���뻻��
//			USART1_RX_STA = 0;
//			}	   

    }		
*/
}


