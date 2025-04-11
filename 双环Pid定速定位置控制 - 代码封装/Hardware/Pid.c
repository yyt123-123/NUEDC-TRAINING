#include "stm32f10x.h"                  // Device header
#include "pid.h"

float zuosudu, yousudu;

/**
 * @brief:位置式Pid速度环
 * @param:实际速度值，目标速度值（-230~230），输出值
 * @param:比例项，积分项，微分项
 * @param:第n次误差，第n+1次误差值，误差积累 
 * @note:电机驱动和编码器测速极性应该一致-顺时针为正，逆时针为负数
	   	  否则pid负反馈调控变为正反馈调控，轮子直接满转
		  
	     实际速度值：-300~300，输入占空比：-1000~1000
		 Kp = 5~19  Ki = 1~2  Kd = 0~8
 * @note:位置式PID使用误差积分形式，由自动切换手动时ErrorInt ++;会影响运动状态
 */
 
int16_t Speed, Location; 


PID_t Inner = {
	.Kp = 7,
	.Ki = 1.8,
	.Kd = 0,
};

PID_t Outer = {
	.Kp = 0.2,
	.Ki = 0,
	.Kd = 0.2,
	.Target = 1000
};

void TIM1_UP_IRQHandler(void)
{
	static uint16_t Count1, Count2, Change;
	Change++;
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		Count1 ++;
		Count2 ++;
		
		if(Count1 >= 40)
		{
			Count1 = 0;
			
			Speed = Encoder_Right();
			Location += Speed;
			
			Inner.Actual = Speed;
			
			PID_Update(&Inner, 1000, -1000);
			
			Motor_SetPWM(Inner.Out);
			
		}
		
		if(Count2 >= 40)
		{
			Count2 = 0;
			
			Outer.Actual = Location;
			
			PID_Update(&Outer, 23, -23);
			
			Inner.Target = Outer.Out;
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}

