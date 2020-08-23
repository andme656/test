#include "bsp_i2c.h"

void I2C_Delay()
{
	uint8_t i;
	
	for(i=0; i<10; i++);
}

void I2C_Start(void)
{
	EEPROM_I2C_SDA_1();
	EEPROM_I2C_SCL_1();
	I2C_Delay();
	EEPROM_I2C_SDA_0();
	I2C_Delay();
	EEPROM_I2C_SCL_0();
	I2C_Delay();
}

void I2C_Stop(void)
{
	EEPROM_I2C_SDA_0();
	EEPROM_I2C_SCL_1();
	I2C_Delay();
	EEPROM_I2C_SDA_1();
}

uint8_t I2C_Read_Byte(void)
{
	uint8_t i;
	uint8_t value;
	
	value = 0;
	for(i=0; i<8; i++)
	{
		value <<= 1;
		EEPROM_I2C_SCL_1();
		I2C_Delay();
		
		if(EEPROM_I2C_SDA_READ())
		{
			value++;
		}
		EEPROM_I2C_SCL_0();
		I2C_Delay();
	}
	return value;
}

void I2C_Send_Byte(uint8_t _ucByte)
{
	uint8_t i;
	
	for(i=0; i<8; i++)
	{
		if(_ucByte & 0x80)
		{
			EEPROM_I2C_SDA_1();
		}
		else
		{
			EEPROM_I2C_SDA_0();
		}
		I2C_Delay();
		EEPROM_I2C_SCL_1();
		I2C_Delay();
		EEPROM_I2C_SCL_0();
		if(i == 7)
		{
			EEPROM_I2C_SDA_1();
		}
		_ucByte <<= 1;
	  I2C_Delay();		
	}
}

void I2C_ACK(void)
{
	EEPROM_I2C_SDA_0();
	I2C_Delay();
	EEPROM_I2C_SCL_1();
	I2C_Delay();
	EEPROM_I2C_SCL_0();
	I2C_Delay();
	EEPROM_I2C_SDA_1();
}

void I2C_NACK(void)
{
	EEPROM_I2C_SDA_1();
	I2C_Delay();
	EEPROM_I2C_SCL_1();
	I2C_Delay();
	EEPROM_I2C_SCL_0();
	I2C_Delay();
}

uint8_t I2C_WaitAck(void)
{
	uint8_t re;
	
	EEPROM_I2C_SDA_1();
	I2C_Delay();
	EEPROM_I2C_SCL_1();
	I2C_Delay();
	
	if(EEPROM_I2C_SDA_READ())
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	EEPROM_I2C_SCL_0();
	I2C_Delay();
	
	return re;
}


void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	I2C_Stop();
}

//void I2C_MODE_Config(void)
//{
//	I2C_InitTypeDef   I2C_InitStruct;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
//	
//	I2C_InitStruct.I2C_ClockSpeed = 400000;
//	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
//	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
//	I2C_InitStruct.I2C_OwnAddress1 = 0x0a;
//	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
//	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//	I2C_Init(I2C1, &I2C_InitStruct);
//	
//	I2C_Cmd(I2C1, ENABLE);
//}

uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	I2C_GPIO_Config();		/* 配置GPIO */

	
	I2C_Start();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	I2C_Send_Byte(_Address | EEPROM_I2C_WR);
	ucAck = I2C_WaitAck();	/* 检测设备的ACK应答 */

	I2C_Stop();			/* 发送停止信号 */

	return ucAck;
}
