#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "Encoder.h"
#include "PWM.h"
#include "Timer.h"

#define Loca   400    //-1320~1320

float zuosudu, yousudu;

/**
 * @brief:?Pid?û
 * @param:??????-230~230?
 * @param:???
 * @param:n?n+1? n+2?
 * @note:????-?????
	   	  pid?????
		  
	     ???-300~300???-1000~1000
		 Kp = 5~19  Ki = 1~2  Kd = 0~8
 * @note:?PID?Out??Ki?Kp, Kd?????
 */
float Actual , Out, Target = 400;
float Kp = 1, Ki = 0.15, Kd = 1;
float Error0, Error1, Error2;

void Pid_AddLoca()
{
	yousudu = Encoder_Right();
	Actual += yousudu;
			
	Error2 = Error1;
	Error1 = Error0;
	Error0 = Target - Actual;
			
	Out += Kp * (Error0 - Error1) + Ki * Error0
					+Kd * (Error0 - 2 * Error1 + Error2);

	if(Out > 1000){Out = 1000;}
	if(Out < -1000){Out = -1000;}
	
	//?-Out??
	Motor_SetPWM(Out);
	
}

void TIM1_UP_IRQHandler(void)
{
	static uint16_t Count, Change;
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		Count ++;
		Change++;
		if(Count >= 40)
		{
			Count = 0;
			
			Pid_AddLoca();
		}
		if(Change >= 5000)
		{
			Target = 1000;
		}
		if(Change >= 10000)
		{
			Target = 0;
		}
		if(Change >= 15000)
		{
			Target = 800;
		}
		if(Change >= 20000)
		{
			Target = 0;
		}
		if(Change >= 25000)
		{
			Target = 500;
		}
		
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}

