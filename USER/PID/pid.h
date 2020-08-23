#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"

typedef struct 
{
	  float SetSpeed; //�趨ֵ
		float ActualSpeed; //ʵ��ֵ
		float err; //ƫ��ֵ
		float err_last; //��һ��ƫ��ֵ
		float Kp,Ki,Kd; 
		float voltage; //�����ѹֵ
		float integral; //�������ֵ
		float umax;
		float umin;
}__pid;

extern __pid pid;

void PID_init(void);
float PID_realize(void);
void pid_keep(void);

#endif