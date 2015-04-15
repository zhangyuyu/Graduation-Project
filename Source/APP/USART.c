#include "USART.h"
#include "ili9320.h"
extern u32 USART2_RX_STA;
extern u8 USART1_RX_BUF[USART1_RX_LEN]; 				//USART1,���ջ�����
extern u8 USART1_TX_BUF[USART1_TX_LEN];
extern u8 USART2_RX_BUF[USART2_RX_LEN]; 				//USART2,���ջ�����				//USART1,���ջ�����

void Usart1_Init(void)		//USART1��ʼ������
{  	
	GPIO_InitTypeDef GPIO_InitStructure;							//GPIO��ʼ���ṹ��
	USART_InitTypeDef USART_InitStructure;							//USART��ʼ���ṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
	/*����IO��ʼ��	  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); //TX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); //RX	

	/*���ڳ�ʼ��  */
	USART_InitStructure.USART_BaudRate  =115200 ;
	USART_InitStructure.USART_WordLength  = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits  = USART_StopBits_1;
	USART_InitStructure.USART_Parity  = USART_Parity_No;
	USART_InitStructure.USART_Mode  = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_InitStructure);
	 /*ʹ�ܽ����ж�	*/

	USART_Cmd(USART1,ENABLE);
}

void Usart_SendChar(USART_TypeDef* USARTx,char dat)		//USART�����ַ�����
{
	while(!USART_GetFlagStatus(USARTx,USART_FLAG_TXE));	
	USART_SendData(USARTx,dat);

}

char Usart_GetCahr(USART_TypeDef* USARTx)		//USART�������ݺ���
{
	char dat;
	while(!USART_GetFlagStatus(USARTx,USART_FLAG_RXNE));
	dat = (char)(USART_ReceiveData(USARTx)&0xff);
	
	return dat;		
}

int Send(int ch)		//���ͺ�����stdio�ӿڣ�
{

while (!(USART1->SR & USART_FLAG_TXE)); // USART1 �ɻ����������ͨ�ŵĴ���

USART1->DR = (ch & 0x1FF);

return (ch);

}

int Get(void)		//���պ�����stdio�ӿڣ�	 
{															  

while (!(USART1->SR & USART_FLAG_RXNE));

return ((int)(USART1->DR & 0x1FF));

}

void Usart2_Init(void)		//USART2��ʼ������
{  	
	GPIO_InitTypeDef GPIO_InitStructure;							//GPIO��ʼ���ṹ��
	USART_InitTypeDef USART_InitStructure;							//USART��ʼ���ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;							//NVIC��ʼ���ṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);			//ʹ��GPIOAʱ��						 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);		    //ʹ��USART2ʱ��
	/*����IO��ʼ��	  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); //TX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); //RX	

	/*���ڳ�ʼ��  */
	USART_InitStructure.USART_BaudRate  = 38400;
	USART_InitStructure.USART_WordLength  = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits  = USART_StopBits_1;
	USART_InitStructure.USART_Parity  = USART_Parity_No;
	USART_InitStructure.USART_Mode  = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
	USART_Init(USART2,&USART_InitStructure);
	 /*ʹ�ܽ����ж�	*/
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2,ENABLE);
}

void USART2_IRQHandler(void)		//USART2�жϴ�����
{
	u8 temp;
	if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE))
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE); 
		if(USART2_RX_STA<USART2_RX_LEN)		//�����Խ�������
		{
//			//USART_ClearFlag(USART1,USART_FLAG_TXE);		//Ϊʲô����������������
			temp=USART_ReceiveData(USART2);	
			USART2_RX_BUF[USART2_RX_STA++]=temp;			//��¼���յ���ֵ
		}								  
		else
		{
			USART2_RX_STA|=1<<15;							//ǿ�Ʊ�ǽ������
		}	
	}
	else
	{
	/**************
	��Ӵ���
	**************/
	}	
}
