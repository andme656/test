#ifndef  __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>

void USART_Config(void);
void Usart_SetByte(USART_TypeDef* pUSARTx,uint8_t data);
void UsartSendStr(USART_TypeDef* pUSARTx,char *str);
void usart_action(void);

#endif

