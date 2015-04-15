#include "ADC.h"

extern __IO u16 ADC_ConvertedValue;

void ADC1_Init(void)				  //ADC1��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
	
	/* Configure PC.01  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);				// PC1,����ʱ������������
	/* DMAͨ��1���� */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* ʹ��DMAͨ��1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
		  
	/* ADC1���� */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 ����ͨ��11����*/ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_55Cycles5);
	
	/* ʹ��ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* ʹ�� ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/* ʹ�� ADC1 ��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/* ��� ADC1 ��λУ׼�Ĵ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ��ʼADC1 У׼ */
	ADC_StartCalibration(ADC1);
	/* ��� ADC1 У׼�Ĵ��� */
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* ��ʼ ADC1 ת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
