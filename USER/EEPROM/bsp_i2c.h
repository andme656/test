#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "stm32f10x.h"

#define EEPROM_I2C_WR	          0		
#define EEPROM_I2C_RD	          1		

#define EEPROM_I2C_SCL_1()   		GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define EEPROM_I2C_SCL_0()  		GPIO_ResetBits(GPIOB,GPIO_Pin_6)

#define EEPROM_I2C_SDA_1()   		GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define EEPROM_I2C_SDA_0()   		GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define EEPROM_I2C_SDA_READ()   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)

void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_Read_Byte(void);
void I2C_Send_Byte(uint8_t _ucByte);
void I2C_ACK(void);
void I2C_NACK(void);
uint8_t I2C_WaitAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);
	
#endif

