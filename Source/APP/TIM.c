#include "TIM.h"

extern u8 Tim1_Time;			//TIM1定时器溢出计数

void Tim1_Init(void)		//TIM1初始化函数
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* TIM时钟使能 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	/* 基定时器初始化 */
	TIM_TimeBaseStructure.TIM_Period = TIM_COUNTER;				//计数值
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_PSC;				//分频系数		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//时钟分割：寄存器（技术）手册基定时器未讲到。
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//重复计数值
	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);				//初始化TIM1
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);					//打开 更新事件 中断
	/* 使能TIM中断 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);								//初始化TIM中断
	TIM_Cmd(TIM1, ENABLE);										//使能TIM
}

void TIM1_UP_IRQHandler(void)		//TIM1溢出处理函数
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)					//判断是否为 更新事件 标志位
	{
	    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);						//清除 	更新事件 标志

		GPIO_WriteBit(GPIOD, GPIO_Pin_11, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_11)));
		Tim1_Time++;
//		if(Tim1_Time>=255)
//		{
//			Tim1_Time=0;
//		}
	}
}
