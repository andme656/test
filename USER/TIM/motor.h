#ifndef __MOTOR_H
#define __MOTOR_H


#include "stm32f10x.h"

#define            GENERAL_TIM                   TIM3
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM3
//#define            GENERAL_TIM_Period            9
//#define            GENERAL_TIM_Prescaler         71
// TIM3 CH1
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          GPIOA
#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_6

#define 						AIN1_HIGH	         GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define 						AIN1_LOW	         GPIO_ResetBits(GPIOB,GPIO_Pin_14)           
#define 						AIN2_HIGH	         GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define 						AIN2_LOW           GPIO_ResetBits(GPIOB,GPIO_Pin_15)

#define Stop				0
#define Front				1
#define Back				2

/**************************????********************************/

void motor_init(void);
void Motor_State(u8 sta);

#endif	


