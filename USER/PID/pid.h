#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"

typedef struct 
{
	  float SetSpeed; //设定值
		float ActualSpeed; //实际值
		float err; //偏差值
		float err_last; //上一个偏差值
		float Kp,Ki,Kd; 
		float voltage; //定义电压值
		float integral; //定义积分值
		float umax;
		float umin;
}__pid;

extern __pid pid;

void PID_init(void);
float PID_realize(void);
void pid_keep(void);

#endif