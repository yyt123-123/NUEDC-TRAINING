#include "stm32f10x.h"                  // Device header
#include "Pid.h"
#include "Encoder.h"
#include "Motor.h"
#include "JY901.h"
#include "RP.h"
#include "math.h"
#include "Pid_User.h"

int16_t Speed_R, Speed_L;
extern float Yaw,Num;
float V = 20;
//内环速度环
//占空比-1000~1000
//最大速度-150~150
float zuosudu, yousudu;
float R_Actual , R_Out, R_Target;
float R_Kp = 2, R_Ki = 3, R_Kd = 2;
float R_Error0, R_Error1, R_ErrorInt;

void PID_R(float Target)
{
	yousudu = Encoder_R();
	R_Actual = yousudu;
	R_Target = Target;

	if(fabs(Target) < 3)
	{
		R_Out = 0;
	}
	
	else
	{
		if(R_Target > 0)
		{
			R_Target += 3; 
		}
		if(R_Target < 0)
		{
			R_Target -= 3;
		}
			
		R_Error1 = R_Error0;
		//改为 Actual - Target可以反转极性
		R_Error0 = R_Target - R_Actual;
			
		if(R_Ki != 0){R_ErrorInt += R_Error0;}
		else{R_ErrorInt = 0;}
		if(R_ErrorInt > 600){R_ErrorInt = 600;}
		if(R_ErrorInt < -600){R_ErrorInt = -600;}	

		R_Out = R_Kp * R_Error0 + R_Ki * R_ErrorInt + R_Kd * (R_Error0 - R_Error1);
	}

	if(R_Out > 1000){R_Out = 1000;}
	if(R_Out < -1000){R_Out = -1000;}
	
	//改为-Out可以反转极性
	Motor_R(R_Out);
}

float L_Actual , L_Out, L_Target;
float L_Kp = 2, L_Ki = 3, L_Kd = 3;
float L_Error0, L_Error1, L_ErrorInt;

void PID_L(float Target)
{
	zuosudu = Encoder_L();
	L_Actual = zuosudu;
	L_Target = Target;
	
	if(fabs(Target) < 3)
	{
		L_Out = 0;
	}
	else
	{
		if(Target > 0)
		{
			L_Target += 3; 
		}
		if(Target < 0)
		{
			L_Target -= 3;
		}
		L_Error1 = L_Error0;
		//改为 Actual - Target可以反转极性
		L_Error0 = L_Target - L_Actual;
			
		if(L_Ki != 0){L_ErrorInt += L_Error0;}
		else{L_ErrorInt = 0;}
		if(L_ErrorInt > 600){L_ErrorInt = 600;}
		if(L_ErrorInt < -600){L_ErrorInt = -600;}	

		L_Out = L_Kp * L_Error0 + L_Ki * L_ErrorInt + L_Kd * (L_Error0 - L_Error1);
	}
	if(L_Out > 1000){L_Out = 1000;}
	if(L_Out < -1000){L_Out = -1000;}

	//改为-Out可以反转极性
	Motor_L(L_Out);
}

float T_Actual , T_Out, T_Target;
float T_Kp = 2, T_Ki , T_Kd ;
float T_Error0, T_Error1, T_ErrorInt;
extern uint16_t flag;
//转向环
int16_t PID_Turn(void)
{
	T_Actual = Yaw;
	T_Target = T_Target;
			
	T_Error1 = T_Error0;
	//改为 Actual - Target可以反转极性
	T_Error0 = T_Target - T_Actual;
	
	if(fabs(T_Error0) < 1)
	{
		return 0;
	}
	else
	{
		if(T_Ki != 0){T_ErrorInt += T_Error0;}
		else{T_ErrorInt = 0;}
		if(T_ErrorInt > 60){T_ErrorInt = 60;}
		if(T_ErrorInt < -60){T_ErrorInt = -60;}	

		T_Out = T_Kp * T_Error0 + T_Ki * T_ErrorInt + T_Kd * (T_Error0 - T_Error1);

		if(T_Out > 40){T_Out = 40;}
		if(T_Out < -40){T_Out = -40;}
			
		return T_Out;
	}
}

int16_t Outer;
void TIM1_UP_IRQHandler(void)
{
	static uint16_t Count1 ,Count2 ;
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{		
		Count1 ++;
		Count2 ++;
		if(flag == 1)
		{
			if(Count1 >= 40 )
			{
				Count1 = 0;
				Outer = PID_Turn();
			}
			if(Count2 >= 40)
			{
				Count2 = 0;
				PID_R(20+Outer);		
				PID_L(20-Outer);
			}
		}
		
		if(flag == 2)
		{
			if(Count2 >= 40)
			{
				Count2 = 0;
				PID_R(35);		
				PID_L(35);
			}
		}
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}


