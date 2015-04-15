#include "USART.h"
#include "ili9320.h"
extern u32 USART2_RX_STA;
extern u8 USART1_RX_BUF[USART1_RX_LEN]; 				//USART1,接收缓存区
extern u8 USART1_TX_BUF[USART1_TX_LEN];
extern u8 USART2_RX_BUF[USART2_RX_LEN]; 				//USART2,接收缓存区				//USART1,接收缓存区

void Usart1_Init(void)		//USART1初始化函数
{  	
	GPIO_InitTypeDef GPIO_InitStructure;							//GPIO初始化结构体
	USART_InitTypeDef USART_InitStructure;							//USART初始化结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
	/*串口IO初始化	  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); //TX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); //RX	

	/*串口初始化  */
	USART_InitStructure.USART_BaudRate  =115200 ;
	USART_InitStructure.USART_WordLength  = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits  = USART_StopBits_1;
	USART_InitStructure.USART_Parity  = USART_Parity_No;
	USART_InitStructure.USART_Mode  = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_InitStructure);
	 /*使能接收中断	*/

	USART_Cmd(USART1,ENABLE);
}

void Usart_SendChar(USART_TypeDef* USARTx,char dat)		//USART发送字符函数
{
	while(!USART_GetFlagStatus(USARTx,USART_FLAG_TXE));	
	USART_SendData(USARTx,dat);

}

char Usart_GetCahr(USART_TypeDef* USARTx)		//USART接收数据函数
{
	char dat;
	while(!USART_GetFlagStatus(USARTx,USART_FLAG_RXNE));
	dat = (char)(USART_ReceiveData(USARTx)&0xff);
	
	return dat;		
}

int Send(int ch)		//发送函数（stdio接口）
{

while (!(USART1->SR & USART_FLAG_TXE)); // USART1 可换成你程序中通信的串口

USART1->DR = (ch & 0x1FF);

return (ch);

}

int Get(void)		//接收函数（stdio接口）	 
{															  

while (!(USART1->SR & USART_FLAG_RXNE));

return ((int)(USART1->DR & 0x1FF));

}

void Usart2_Init(void)		//USART2初始化函数
{  	
	GPIO_InitTypeDef GPIO_InitStructure;							//GPIO初始化结构体
	USART_InitTypeDef USART_InitStructure;							//USART初始化结构体
	NVIC_InitTypeDef NVIC_InitStructure;							//NVIC初始化结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);			//使能GPIOA时钟						 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);		    //使能USART2时钟
	/*串口IO初始化	  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); //TX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); //RX	

	/*串口初始化  */
	USART_InitStructure.USART_BaudRate  = 38400;
	USART_InitStructure.USART_WordLength  = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits  = USART_StopBits_1;
	USART_InitStructure.USART_Parity  = USART_Parity_No;
	USART_InitStructure.USART_Mode  = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
	USART_Init(USART2,&USART_InitStructure);
	 /*使能接收中断	*/
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2,ENABLE);
}

void USART2_IRQHandler(void)		//USART2中断处理函数
{
	u8 temp;
	if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE))
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE); 
		if(USART2_RX_STA<USART2_RX_LEN)		//还可以接收数据
		{
//			//USART_ClearFlag(USART1,USART_FLAG_TXE);		//为什么参数出错？？？？？
			temp=USART_ReceiveData(USART2);	
			USART2_RX_BUF[USART2_RX_STA++]=temp;			//记录接收到的值
		}								  
		else
		{
			USART2_RX_STA|=1<<15;							//强制标记接收完成
		}	
	}
	else
	{
	/**************
	添加代码
	**************/
	}	
}
