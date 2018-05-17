#ifndef TIME_TEST_H
#define TIME_TEST_H

#include "stm32f2xx.h"
#define TRUE 1
#define FALSE 0
void TIM6_NVIC_Configuration(void);
void TIM6_Configuration(void);
extern unsigned char bl_main_loop_F;
extern unsigned char main_loop_cnt; 
#endif	/* TIME_TEST_H */
