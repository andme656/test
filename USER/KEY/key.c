#include "key.h"
#include "delay.h"
#include "pid.h"
#include "motor.h"

uint8_t move_flag = 0;
uint8_t motor_sta = 0;
extern uint8_t key;
extern uint8_t key_flag;
extern float speed;

//按键初始化//
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStruct);//初始化GPIOA.0

}

//按键扫描函数//
uint8_t KEY_Scan(uint8_t mode)
{
	static uint8_t key_up = 1;
	
	if(mode)  key_up = 1;
	
	if(key_up && (KEY0==0|KEY1==0|KEY2==0))
	{
  	delay_ms(10);
		key_up = 0;
		key_flag = 1;
		if(KEY0 == 0)
			return KEY0_PRES;
		else if(KEY1 == 0)
			return KEY1_PRES;
		else if(KEY2 == 0)
			return KEY2_PRES;
		else if(WK_UP==1)
			return WKUP_PRES;
	}
	else if(KEY0==1 && KEY1==1 && KEY2==1 && WK_UP==0)
	{
		key_up = 1;
		key_flag = 0;
	  return 0;
	}
}


void key_action(void)
{
	  switch(key)
		{
			case WKUP_PRES:
			{
				  motor_sta++;
					switch(motor_sta)
					{
						case 0:Motor_State(Stop);pid.voltage  = 0;break;
						case 1:Motor_State(Front);pid.voltage  = 1;break;
						case 2:Motor_State(Back);pid.voltage = 1;break;
					}
					if(motor_sta >= 3)
					{
							motor_sta = 0;
					}
			}
			case KEY0_PRES:
			{
					 move_flag++;
					if(move_flag >= 4)
					{
							move_flag = 0;
					}
					break;
			}
			case KEY1_PRES:
			{
				  if(move_flag == 0)
					{
						  speed += 1;
					}
					else if(move_flag == 1)
					{
							pid.Kp += 1;
					}
					else if(move_flag == 2)
					{
							pid.Ki += 1;
					}
					else if(move_flag == 3)
					{
							pid.Kd += 1;
					}	
					break;
			}
			case KEY2_PRES:
			{
					if(move_flag == 0)
					{
							speed = speed - 1;
					}
					else if(move_flag == 1)
					{
							pid.Kp = pid.Kp - 1;
					}
					else if(move_flag == 2)
					{
							pid.Ki = pid.Ki - 1;
					}
					else if(move_flag == 3)
					{
							pid.Kd = pid.Kd - 1;
					}				
					break;
			}
		}
}


