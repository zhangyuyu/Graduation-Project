#include "PWM.h"
void PWM3_Configuration(void)
{
   	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO , ENABLE);  //ʹ��GPIOC����ģ��ʱ��ʹ��
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH4->PC9

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* ---------------------------------------------------------------
	TIM3CLK ��PCLK1=36MHz
	TIM3 Configuration: generate 1 PWM signals :
	TIM3CLK = 36 MHz, Prescaler = 0x0, TIM3 counter clock = 36 MHz
	TIM3 ARR Register = 900 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
	TIM3 Frequency = 36 KHz.
	TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 
	TIM3CLK = 36 MHz, Prescaler = 0, TIM3 counter clock = 36MHz
	--------------------------------------------------------------- */
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 900; 	//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =0;	 	//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 	//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	/* Output Compare Active Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;		 //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 		//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;		 //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//�������:TIM����Ƚϼ��Ը�
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  		//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  		//ʹ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); 		//ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
}
