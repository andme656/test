#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

#define KEY0    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define KEY1    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KEY2    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

#define KEY0_PRES   10
#define KEY1_PRES   11
#define KEY2_PRES   12
#define WKUP_PRES   13

void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);
void key_action(void);

#endif

