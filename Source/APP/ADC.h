#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x.h"

#define ADC1_DR_Address    ((u32)0x4001244C)		//ADC1转换结果寄存器地址

void ADC1_Init(void);								//ADC1初始化
#endif
