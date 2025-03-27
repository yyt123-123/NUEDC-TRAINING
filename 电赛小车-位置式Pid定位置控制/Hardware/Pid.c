#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "Encoder.h"
#include "PWM.h"
#include "Timer.h"

#define Loca   400   	//-1320~1320

float zuosudu, yousudu;

/**
 * @brief:位置式Pid位置环
 * @param:实际速度值，目标速度值（-230~230），输出值
 * @param:比例项，积分项，微分项
 * @param:第n次误差，第n+1次误差值，误差积累 
 * @note:电机驱动和编码器测速极性应该一致-顺时针为正，逆时针为负数
	   	  否则pid负反馈调控变为正反馈调控，轮子直接满转
		  
	     实际速度值：-300~300，输入占空比：-1000~1000
		 Kp = 5~19  Ki = 1~2  Kd = 0~8
 * 
 */
float Actual , Out ;
float Kp = 1, Ki = 0, Kd = 1;
float Error0, Error1, ErrorInt;

void Pid_Location(float Target)
{
	yousudu = Encoder_Right();
	Actual += yousudu;
			
	Error1 = Error0;
	//改为 Actual - Target可以反转极性
	Error0 = Target - Actual;
			
	if(Ki != 0){ErrorInt += Error0;}
	else{ErrorInt = 0;}

	Out = Kp * Error0 + Ki * ErrorInt + Kd * (Error0 - Error1);

	if(Out > 1000){Out = 1000;}
	if(Out < -1000){Out = -1000;}
	
	//改为-Out可以反转极性
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
			
			Pid_Location(Loca);
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}

