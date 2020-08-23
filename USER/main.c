#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "bsp_lcd.h"
#include "usart.h"
#include "pid.h"
#include "motor.h"
#include "bsp_ee_i2c.h"
#include "adc.h"


uint8_t key_flag = 0;
uint8_t key = 0;
float speed = 0;
volatile uint32_t time = 0; 

extern	uint8_t write_buf[5];
extern uint8_t read_buf[5];

void wave_show(void);
void window_value_show(void);

 int main(void)
 {	 
		  delay_init();	    	 //延时函数初始化	  
		  USART_Config();	 //串口初始化为115200
		  LED_Init();	 //LED端口初始化
		  LCD_Init();
		  KEY_Init();	
		  motor_init();
			PID_init();
			Adc_Init();
		  LCD_Clear(WHITE);
			 
			while(1)
			{
					window_value_show();
					key = KEY_Scan(0);
					 if(key_flag == 1)
						{
								 key_flag = 0;
								 key_action();
						}
						wave_show();
						pid_keep();
			}
}
 
void window_value_show(void)
{
		POINT_COLOR=BLUE;//设置字体为蓝色
		LCD_ShowString(30,40,210,16,16,"Kp:");
		LCD_ShowString(30,70,210,16,16,"Ki:");
		LCD_ShowString(30,90,210,16,16,"Kd:");
		LCD_ShowString(30,120,210,16,16,"Speed:");
	
		POINT_COLOR=RED;//设置字体为红色
	  LCD_ShowxNum(50,40,pid.Kp,16,16,0);
		LCD_ShowxNum(50,70,pid.Ki,16,16,0);
		LCD_ShowxNum(50,90,pid.Kd,16,16,0);
		LCD_ShowxNum(50,120,pid.SetSpeed,16,16,0);
}

void wave_show(void)
{
		uint8_t i = 0;
		uint16_t value[160];
	
		while(i < 160)
		{
			  value[i] = Get_Adc_Average(ADC_Channel_1,5);
				delay_ms(10);
			
				POINT_COLOR=BLUE;//设置字体为蓝色
				LCD_DrawLine(i,((3.3/325)*value[i]+475),(i+1),((3.3/325)*value[i+1]+475));
				delay_ms(5);
		}
}	

