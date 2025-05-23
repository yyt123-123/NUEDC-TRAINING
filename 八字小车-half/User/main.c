#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Timer.h"
#include "Key.h"
#include "RP.h"
#include "Motor.h"
#include "Encoder.h"
#include "Serial.h"
#include "PWM.h"
#include "JY901.h"
#include "Pid.h"
#include "Pid_User.h"
#include "math.h"
#include "Car.h"

#define D1			GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)
#define D2			GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)
#define D3			GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)
#define D4			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)
#define D5			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)
#define D6			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)
#define D7			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)


float Roll, Pitch, Yaw, Num;
extern float L_Actual , L_Out, L_Target, L_ErrorInt;
extern float R_Actual , R_Out, R_Target, R_ErrorInt;
extern float T_Actual , T_Out, T_Target, T_ErrorInt;
extern float zuosudu, yousudu;
extern float V;
extern int16_t Outer;
uint16_t flag ;
void Car_Amend(void);

int16_t i = 0;
/*  
	flag == 1  执行转向
	flag == 2  执行走直线
	flag == 3  执行循迹
*/

int main(void)
{
	OLED_Init();
    Encoder_Init();
	
	Timer_Init();
	PWM_Init();
	Motor_Init();
	Serial_Init();
	JY901_Init();
	RP_Init();

	while(1)
	{	
		T_Target = T_Actual;
		JY901_GetAngle(&Roll, &Pitch, &Yaw);		
		OLED_Printf(0, 0, 8, "Roll:%3.2f", Roll);
		OLED_Printf(0, 16, 8, "Pitch:%3.2f", Pitch);
		OLED_Printf(0, 32, 8, "Yaw:%3.2f", Yaw);	
		OLED_Update();
		
		//小车转向
		flag = 1;
		T_Target = -30+T_Actual;
		while(flag == 1)
		{
			OLED_Printf(0, 0, 8, "Roll:%3.2f", Roll);
			OLED_Printf(0, 16, 8, "Pitch:%3.2f", Pitch);
			OLED_Printf(0, 32, 8, "Yaw:%3.2f", Yaw);
			OLED_Printf(0, 48, 8, "flag = %d", flag);	
			
			OLED_Update();
			JY901_GetAngle(&Roll, &Pitch, &Yaw);
			Outer = 0;
			while(fabs(T_Actual - T_Target) < 0.5)
			{
				flag = 2;
				break;
			}
		}
		//小车走直线
		flag = 2;
		while(flag == 2)
		{
			OLED_Printf(0, 0, 8, "Roll:%3.2f", Roll);
			OLED_Printf(0, 16, 8, "Pitch:%3.2f", Pitch);
			OLED_Printf(0, 32, 8, "Yaw:%3.2f", Yaw);
			OLED_Printf(0, 48, 8, "flag = %d", flag);	
			
			OLED_Update();
			JY901_GetAngle(&Roll, &Pitch, &Yaw);
			while(D1 == 1)
			{
				i = 1;
				flag = 0;
				break;
			}
		}
		
		//小车循迹
		flag = 3;
		while(flag == 3)
		{
			OLED_Printf(0, 0, 8, "Roll:%3.2f", Roll);
			OLED_Printf(0, 16, 8, "Pitch:%3.2f", Pitch);
			OLED_Printf(0, 32, 8, "Yaw:%3.2f", Yaw);
			OLED_Printf(0, 48, 8, "flag = %d", flag);	
			JY901_GetAngle(&Roll, &Pitch, &Yaw);
			OLED_Update();
			Car_Amend();
			if(D1 == 0 && D2 == 0 && D3 == 0 && D4 == 0 && D5 == 0)
			{
				Delay_ms(50);
				if(D1 == 0 && D2 == 0 && D3 == 0 && D4 == 0 && D5 == 0)
					flag = 0;
			}
		}
		
			//小车转向
		Turn_circle();
		Delay_ms(300);
		T_Target = -130;
		PID_R(0);		
		PID_L(0);
		L_ErrorInt = 0;
		R_ErrorInt = 0;
		flag = 1;
		while(flag == 1)
		{
			JY901_GetAngle(&Roll, &Pitch, &Yaw);
			OLED_Printf(0, 0, 8, "Roll:%3.2f", Roll);
			OLED_Printf(0, 16, 8, "Pitch:%3.2f", Pitch);
			OLED_Printf(0, 32, 8, "Yaw:%3.2f", Yaw);	
			OLED_Printf(0, 48, 8, "flag = %d", flag);	
			OLED_Update();
			while(fabs(T_Actual - T_Target) < 0.2)
			{
				flag = 2;
				break;
			}
		}
		Car_Stop();
		Delay_s(1);
		//小车走直线
		flag = 2;
		while(flag == 2)
		{
			OLED_Printf(0, 0, 8, "Roll:%3.2f", Roll);
			OLED_Printf(0, 16, 8, "Pitch:%3.2f", Pitch);
			OLED_Printf(0, 32, 8, "Yaw:%3.2f", Yaw);
			OLED_Printf(0, 48, 8, "flag = %d", flag);	
			
			OLED_Update();
			JY901_GetAngle(&Roll, &Pitch, &Yaw);
			while(D1 == 1)
			{
				i = 1;
				flag = 0;
				break;
			}
		}
		
//		while(1)
//		{
//			Turn_LRight();
//			while(D5 == 1)
//			{
//				flag = 3;
//				break;
//			}
//			if(flag == 3)
//				break;
//		}
			
		//小车循迹
		flag = 3;
		while(flag == 3)
		{
			OLED_Printf(0, 48, 8, "flag = %d", flag);	
			OLED_Update();
			
			Car_Amend();
			if(D1 == 0 && D2 == 0 && D3 == 0 && D4 == 0 && D5 == 0)
				break;
		}

		//小车转向
//		flag = 1;
//		while(flag == 1)
//		{
//			OLED_Printf(0, 0, 8, "Roll:%3.2f", Roll);
//			OLED_Printf(0, 16, 8, "Pitch:%3.2f", Pitch);
//			OLED_Printf(0, 32, 8, "Yaw:%3.2f", Yaw);	
//			OLED_Update();
//			JY901_GetAngle(&Roll, &Pitch, &Yaw);
//			T_Target = 45;
//			while(fabs(T_Actual - T_Target) < 1)
//			{
//				flag = 0;
//				break;
//			}
//		}
//		
//		//小车走直线
//		flag = 2;
//		while(flag == 2)
//		{
//			OLED_Printf(0, 0, 8, "Roll:%3.2f", Roll);
//			OLED_Printf(0, 16, 8, "Pitch:%3.2f", Pitch);
//			OLED_Printf(0, 32, 8, "Yaw:%3.2f", Yaw);	
//			OLED_Update();
//			JY901_GetAngle(&Roll, &Pitch, &Yaw);
//			while(D1 == 1 || D2 == 1 || D3 == 1 || D4 == 1 || D5 == 1)
//			{
//				flag = 0;
//				break;
//			}
//		}

//		//小车循迹
//		flag = 3;
//		while(flag == 3)
//		{
//			Car_Amend();
//			while(D1 == 0 || D2 == 0 || D3 == 0 || D4 == 0 || D5 == 0)
//			{
//				flag = 0;
//				break;
//			}
//		}


//		Angle_turn(90);
//		OLED_Printf(0, 0, 8, "Kp:%+04.2f", Turn.Kp);
//		OLED_Printf(0, 16, 8, "Out:%+04.5f", Turn.Out);
//		OLED_Printf(0, 32, 8, "Act:%+04.2f", Turn.Actual);
//		OLED_Printf(0, 48, 8, "Num:%+04.2f", Num);
//		
//		Serial_Printf("%f, %f, %f\r\n", R_Actual, R_Target, R_Out);
//		break;
	}
}

void Car_Amend(void)
{
	if (D3 == 1)
	{
		Move_Forward();
	}
    else if(D4 == 1)
	{
        Turn_SRight();
    }
    else if(D2 == 1)
    {
	    Turn_SLeft();
    }
    else if(D1 == 1)
	{
        Turn_LLeft();
    }
    else if(D5 == 1)
    {
	    Turn_LRight();
    }
}

