#include "TIM.h"

extern u8 Tim1_Time;			//TIM1��ʱ���������

void Tim1_Init(void)		//TIM1��ʼ������
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* TIMʱ��ʹ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	/* ����ʱ����ʼ�� */
	TIM_TimeBaseStructure.TIM_Period = TIM_COUNTER;				//����ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_PSC;				//��Ƶϵ��		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//ʱ�ӷָ�Ĵ������������ֲ����ʱ��δ������
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//�ظ�����ֵ
	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);				//��ʼ��TIM1
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);					//�� �����¼� �ж�
	/* ʹ��TIM�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);								//��ʼ��TIM�ж�
	TIM_Cmd(TIM1, ENABLE);										//ʹ��TIM
}

void TIM1_UP_IRQHandler(void)		//TIM1���������
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)					//�ж��Ƿ�Ϊ �����¼� ��־λ
	{
	    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);						//��� 	�����¼� ��־

		GPIO_WriteBit(GPIOD, GPIO_Pin_11, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_11)));
		Tim1_Time++;
//		if(Tim1_Time>=255)
//		{
//			Tim1_Time=0;
//		}
	}
}
