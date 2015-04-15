#ifndef __TIM_H
#define __TIM_H
#include "stm32f10x.h"

#define TIM_COUNTER     2000					  	//计数个数							
#define TIM_PSC         36000                     	//分频系数

void Tim1_Init(void);					 			//TIM1初始化函数声明

#endif
