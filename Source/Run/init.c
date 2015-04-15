#include "init.h"

/*******************************************************************************
* Function Name  : RCC_Configuration	  ϵͳʱ�ӳ�ʼ������
* Description    : Configures the different system clocks.
*******************************************************************************/
void RCC_Configuration(void)
{   
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  if(RCC_WaitForHSEStartUp() == SUCCESS)   /* Wait till HSE is ready �ȴ��ⲿ��������*/
  {
     /* Enable Prefetch Buffer */
     FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

     /* Flash 2 wait state */
     FLASH_SetLatency(FLASH_Latency_2);
    
     /* HCLK = SYSCLK */
     RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
     /* PCLK2 = HCLK */
     RCC_PCLK2Config(RCC_HCLK_Div1); 
 
     /* PCLK1 = HCLK/2 */
     RCC_PCLK1Config(RCC_HCLK_Div2);
 
     /* PLLCLK = 8MHz * 9 = 72 MHz */
     RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);		 //9��Ƶ  8M*9=72M
 
     /* Enable PLL */ 
     RCC_PLLCmd(ENABLE);
 
     /* Wait till PLL is ready */
     while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
     {
     }
 
     /* Select PLL as system clock source */
     RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 
     /* Wait till PLL is used as system clock source */
     while(RCC_GetSYSCLKSource() != 0x08)
     {
     }
   }
}



/*******************************************************************************
* Function Name  : NVIC_Configuration     �ж��������ʼ������
* Description    : Configures Vector Table base location.   
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	//�������ȼ����飺��ռ���ȼ�0λ,�����ȼ�4λ
	
	//�����������λ�ú�ƫ��
	#ifdef  VECT_TAB_RAM  
		/* Set the Vector Table base location at 0x20000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 		//������λ��RAM
	#else  /* VECT_TAB_FLASH  */
		/* Set the Vector Table base location at 0x08000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   //������λ��FLASH
	#endif

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//USART1�ж�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1
}



/*******************************************************************************
* Function Name  : GPIO_Configuration     GPIO��ʼ������
*******************************************************************************/
void GPIO_Configuration(void)		
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* ��1������GPIO��USART������ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ��3������USART Rx��GPIO����Ϊ��������ģʽ          */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ��1������GPIO��USART2������ʱ�� */
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/* ��2������USART2 Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ��3������USART2 Rx��GPIO����Ϊ��������ģʽ  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//�ó�ʼ���Ľṹ���ʼ��GPIOA
}
