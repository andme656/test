#ifndef __BSP_EE_I2C_H
#define __BSP_EE_I2C_H

#include "stm32f10x.h" 

#define EEPROM_DEV_ADDR			0xA0
#define EEPROM_PAGE_SIZE    8
#define EEPROM_SIZE				  256

uint8_t ee_ReadByte(uint8_t*_pReadBuf,uint16_t _usAddress,uint16_t _usSize);
uint8_t ee_WriteByte(uint8_t* _pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
 void ee_Delay(__IO uint32_t nCount);
uint8_t ee_CheckOk(void);

#endif

