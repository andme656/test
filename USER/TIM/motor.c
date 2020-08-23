#include "motor.h"
#include "key.h"
#include "pid.h"

extern uint32_t set_speed_flag;

static void Motor_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

    /*���PWM�������  PA6*/
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH1_PORT, &GPIO_InitStructure);
	
		/*- ���A�������� PB14��PB15*/
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
	
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	 GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);
	
	
/*--------------------ʱ���ṹ���ʼ��-------------------------*/

	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=Period;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= Prescaler;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

	/*--------------------����ȽϽṹ���ʼ��-------------------*/	
	
	// ����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// ���ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// ���ͨ����ƽ��������	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// ����Ƚ�ͨ�� 1
	TIM_OCInitStructure.TIM_Pulse = pid.voltage;
	TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// ʹ�ܼ�����
	TIM_Cmd(GENERAL_TIM, ENABLE);
}

void motor_init(void)
{
		Motor_GPIO();
		MOTOR_PWM(9,71);			// �������ڣ���������Ϊ100K
	  Motor_State(Stop);
		pid.SetSpeed = 0;
		pid.ActualSpeed = 0;
}

void Motor_State(u8 sta)
{
	switch(sta)
	{
		case 0:																																			//ֹͣ
			AIN1_LOW;
			AIN2_LOW;
		break;
		
		case 1:																																			//��ת
			AIN1_HIGH;
			AIN2_LOW;
		break;
		
		case 2:																																			//��ת
			AIN1_HIGH;
			AIN2_LOW;
		break;
	}			
}
