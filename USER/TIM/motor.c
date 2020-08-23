#include "motor.h"
#include "key.h"
#include "pid.h"

extern uint32_t set_speed_flag;

static void Motor_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

    /*电机PWM输出引脚  PA6*/
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH1_PORT, &GPIO_InitStructure);
	
		/*- 电机A换向引脚 PB14、PB15*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}


static void MOTOR_PWM(uint16_t Period,uint16_t Prescaler)
{
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
	
  // 开启定时器时钟,即内部时钟CK_INT=72M
	 GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);
	
	
/*--------------------时基结构体初始化-------------------------*/

	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=Period;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= Prescaler;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

	/*--------------------输出比较结构体初始化-------------------*/	
	
	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 输出通道电平极性配置	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// 输出比较通道 1
	TIM_OCInitStructure.TIM_Pulse = pid.voltage;
	TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// 使能计数器
	TIM_Cmd(GENERAL_TIM, ENABLE);
}

void motor_init(void)
{
		Motor_GPIO();
		MOTOR_PWM(9,71);			// 配置周期，这里配置为100K
	  Motor_State(Stop);
		pid.SetSpeed = 0;
		pid.ActualSpeed = 0;
}

void Motor_State(u8 sta)
{
	switch(sta)
	{
		case 0:																																			//停止
			AIN1_LOW;
			AIN2_LOW;
		break;
		
		case 1:																																			//正转
			AIN1_HIGH;
			AIN2_LOW;
		break;
		
		case 2:																																			//反转
			AIN1_HIGH;
			AIN2_LOW;
		break;
	}			
}
