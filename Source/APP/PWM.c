#include "PWM.h"
void PWM3_Configuration(void)
{
   	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO , ENABLE);  //使能GPIOC外设模块时钟使能
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH4->PC9

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* ---------------------------------------------------------------
	TIM3CLK 即PCLK1=36MHz
	TIM3 Configuration: generate 1 PWM signals :
	TIM3CLK = 36 MHz, Prescaler = 0x0, TIM3 counter clock = 36 MHz
	TIM3 ARR Register = 900 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
	TIM3 Frequency = 36 KHz.
	TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 
	TIM3CLK = 36 MHz, Prescaler = 0, TIM3 counter clock = 36MHz
	--------------------------------------------------------------- */
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 900; 	//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =0;	 	//设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 	//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	/* Output Compare Active Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;		 //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 		//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;		 //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  		//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  		//使能TIMx在CCR3上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); 		//使能TIMx在ARR上的预装载寄存器
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
}
