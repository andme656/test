#include "pid.h"
#include "usart.h"
#include "bsp_ee_i2c.h"
#include "delay.h"
#include "encoder.h"

uint8_t write_buf[5];
uint8_t read_buf[5];
__pid pid;

extern float speed;

void PID_init(void)
	{
			uint8_t num;
		
			pid.SetSpeed=0.0;
			pid.ActualSpeed=0.0;
			pid.err=0.0;
			pid.err_last=0.0;
			pid.voltage=0.0;
			pid.integral=0.0;
			pid.Kp=8;//0.2
			pid.Ki=5;//0.1
			pid.Kd=6;//0.2
			pid.umax=400;
			pid.umin=-200;
		
			num = ee_ReadByte(read_buf,0,5);
			if(read_buf[4])
			{
				  pid.Kp = read_buf[0];
					pid.Ki = read_buf[1];
					pid.Kd = read_buf[2];
					pid.SetSpeed  = read_buf[3];
			}
			printf("PID_init end \r\n");
}


float PID_realize(void)
{
			float index;

			pid.SetSpeed=speed;
			pid.ActualSpeed = encoder_read(); 
			pid.err=pid.SetSpeed-pid.ActualSpeed;
			if(pid.ActualSpeed>pid.umax)
			{
					if((pid.err)>200)
					{
							 index=0.0;
					}
					else 
					{
						  if(pid.err < 180)
							{
									index=1.0;
							}
							else 
							{
								  index=(200-(pid.err))/20;
							}
              if(pid.err<0)
              {
                   pid.integral+=pid.err;
              }
					}
      }
			else if(pid.ActualSpeed<pid.umin)
			{
           if((pid.err)>200)
           {
                index=0;
           }
					else
					{
              if(pid.err < 180)
							{
									index=1.0;
							}
							else 
							{
								  index=(200-(pid.err))/20;
							}
              if(pid.err>0)
              {
                  pid.integral+=pid.err;
              }
					}
      }
			else
			{
             if((pid.err)>200) 
             {
                 index=0;
             }
						 else
						 {
									 if(pid.err < 180)
									 {
											  index=1.0;
									 }
										else 
										{
												index=(200-(pid.err))/20;
										}
									pid.integral+=pid.err;
						 } 
			}
			pid.voltage=pid.Kp*pid.err+index*pid.Ki*pid.integral+pid.Kd*(pid.
			err-pid.err_last);
			pid.err_last=pid.err;
			
			return pid.voltage;
}

void pid_keep(void)
{		
		uint8_t keep_flag = 1;
	
	  if (ee_CheckOk() == 0)
	  {
		    printf("没有检测到串行EEPROM!\r\n");
	  }
	  write_buf[0] = pid.Kp;
	  write_buf[1] = pid.Ki;
		write_buf[2] = pid.Kd;
		write_buf[3] = pid.SetSpeed;
	  write_buf[4] = keep_flag;
    ee_WriteByte(write_buf, 0, 5);
		ee_Delay(0x0FFFFF);
}
