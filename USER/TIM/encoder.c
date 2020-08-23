#include "encoder.h"

void encoder_gpio_config(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;												//�˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 											//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);					      											//�����趨������ʼ��GPIOA
}

void Encoder_mode_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);												//ʹ�ܶ�ʱ��2��ʱ��

  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 																	//Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period = 65535; 																	//�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;											//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;									//TIM���ϼ���  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  TIM_SetCounter(TIM2,0);
  TIM_Cmd(TIM2, ENABLE); 
}

void encoder_init_tim2(void)
{
	  encoder_gpio_config;
		Encoder_mode_config;
}

float encoder_read(void)
{
	  float encoder_value;
	
	  encoder_value = TIM2->CNT;
		TIM2->CNT = 0;
	
		return encoder_value;
}

/*--------------------1ms��ʱ--------------------------*/

static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void basic_tim_mode_config(void)
{
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	
		BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK,ENABLE);
	
	//	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
//	  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
//	  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInitStruct.TIM_Period = BASIC_TIM_Period;
		TIM_TimeBaseInitStruct.TIM_Prescaler = BASIC_TIM_Prescaler;
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseInitStruct);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(BASIC_TIM, ENABLE);	
}

void basic_tim_init(void)
{
		BASIC_TIM_NVIC_Config();
	  basic_tim_mode_config();
}

