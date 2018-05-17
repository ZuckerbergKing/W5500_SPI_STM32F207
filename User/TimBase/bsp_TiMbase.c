#include "bsp_TiMbase.h" 
#include "stm32f2xx_tim.h"
#include  "stm32f2xx.h"
#include  "stm32f2xx_conf.h"

unsigned char bl_main_loop_F = 0;
unsigned char main_loop_cnt = 0;
/* TIM2中断优先级配置 */
void TIM6_NVIC_Configuration(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //?????0?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //????0?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * 中断周期为 = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */
void TIM6_Configuration(void)
{
	  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
		

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//???????6 
	TIM_DeInit(TIM6);
	TIM_TimeBaseStructure.TIM_Period =999;//ARR ??????PWM??????  
	TIM_TimeBaseStructure.TIM_Prescaler =59;//PSC ??????TIMx???????????   60M/(TIM_TimeBaseStructure.TIM_Prescaler+1)=1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 2 ;  
 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
 	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); 
	TIM_ARRPreloadConfig(TIM6, ENABLE);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); 
	TIM_Cmd(TIM6,ENABLE); 
	TIM6_NVIC_Configuration();
}
/*********************************************END OF FILE**********************/
