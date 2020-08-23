#include "bsp_ee_i2c.h"
#include "bsp_i2c.h"
#include "usart.h"
#include "pid.h"

uint8_t ee_CheckOk(void)
{
	if (i2c_CheckDevice(EEPROM_DEV_ADDR) == 0)
	{
		return 1;
	}
	else
	{
		/* ʧ�ܺ��мǷ���I2C����ֹͣ�ź� */
		I2C_Stop();		
		return 0;
	}
}

uint8_t ee_ReadByte(uint8_t*_pReadBuf,uint16_t _usAddress,uint16_t _usSize)
{
	uint16_t i;
	
	I2C_Start();
	I2C_Send_Byte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);
	if(I2C_WaitAck() != 0)
	{
		goto cmd_fail;
	}
	I2C_Send_Byte((uint8_t)_usAddress);
	if(I2C_WaitAck() != 0)
	{
		goto cmd_fail;
	}
	I2C_Start();
	I2C_Send_Byte(EEPROM_DEV_ADDR | EEPROM_I2C_RD);
	if(I2C_WaitAck() != 0)
	{
		goto cmd_fail;
	}
	
	for(i=0; i<_usSize; i++)
	{
		_pReadBuf[i] = I2C_Read_Byte();
		
		if(i != _usSize - 1)
		{
			I2C_ACK();
		}
		else
		{
			I2C_NACK();
		}
	}
	I2C_Stop();
	return 1;
	
	cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	I2C_Stop();
	return 0;
}

uint8_t ee_WriteByte(uint8_t* _pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;
	
	usAddr = _usAddress;
	for(i=0; i<_usSize; i++)
	{
		if((i==0) || (usAddr & (EEPROM_PAGE_SIZE - 1)) == 0)
		{
			I2C_Stop();
			
			for(m=0; m<1000; m++)
			{
				I2C_Start();
				I2C_Send_Byte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);
				if(I2C_WaitAck() == 0)
				{
					break;
				}			
			}
			if(m == 1000)
			{
				goto cmd_fail;
			}
			I2C_Send_Byte((uint8_t)usAddr);
			if(I2C_WaitAck() != 0)
			{
				goto cmd_fail;
			}		
		}
		I2C_Send_Byte(_pWriteBuf[i]);
		if(I2C_WaitAck() != 0)
		{
			goto cmd_fail;
		}		
		usAddr++;	
	}
	I2C_Stop();	
	return 1;
	
	cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	I2C_Stop();
	return 0;
}

//void ee_Erase(void)
//{
//	uint16_t i;
//	uint8_t buf[EEPROM_SIZE];
//	
//	for(i=0; i<(EEPROM_SIZE - 1); i++)
//	{
//		buf[i] = 0xff;
//	}
//	
//	if (ee_WriteByte(buf,0,EEPROM_SIZE) == 0)
//	{
//		printf("����eeprom����\r\n");
//	}
//	else
//	{
//		printf("����eeprom�ɹ���\r\n");
//	}
//}

 void ee_Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

//uint8_t ee_Test(void) 
//{
//  uint16_t i;
//	uint8_t write_buf[EEPROM_SIZE];
//  uint8_t read_buf[EEPROM_SIZE];
//  
///*-----------------------------------------------------------------------------------*/  
//  if (ee_CheckOk() == 0)
//	{
//		/* û�м�⵽EEPROM */
//		printf("û�м�⵽����EEPROM!\r\n");
//				
//		return 0;
//	}
///*------------------------------------------------------------------------------------*/  
//  /* �����Ի����� */
//	for (i = 0; i < EEPROM_SIZE; i++)
//	{		
//		write_buf[i] = i;
//	}
///*------------------------------------------------------------------------------------*/  
//  if (ee_WriteByte(write_buf, 0, EEPROM_SIZE) == 0)
//	{
//		printf("дeeprom����\r\n");
//		return 0;
//	}
//	else
//	{		
//		printf("дeeprom�ɹ���\r\n");
//	}
//  
//  /*д��֮����Ҫ�ʵ�����ʱ��ȥ������Ȼ�����*/
//  ee_Delay(0x0FFFFF);
///*-----------------------------------------------------------------------------------*/
//  if (ee_ReadByte(read_buf,0,EEPROM_SIZE) == 0)
//	{
//		printf("��eeprom����\r\n");
//		return 0;
//	}
//	else
//	{		
//		printf("��eeprom�ɹ����������£�\r\n");
//	}
///*-----------------------------------------------------------------------------------*/  
//  for (i = 0; i < EEPROM_SIZE; i++)
//	{
//		if(read_buf[i] != write_buf[i])
//		{
//			printf("0x%X ", read_buf[i]);
//			printf("����:EEPROM������д������ݲ�һ��");
//			return 0;
//		}
//    printf(" %02X", read_buf[i]);
//		
//		if ((i & 15) == 15)
//		{
//			printf("\r\n");	
//		}		
//	}

//  printf("eeprom��д���Գɹ�\r\n");
//  return 1;
//}



