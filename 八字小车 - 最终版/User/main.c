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
void Right_Amend(void);
void Left_Amend(void);

int16_t i = 0;
/*  
	flag == 1  ִ��ת��
	flag == 2  ִ����ֱ��
	flag == 3  ִ��ѭ��
*/

//����,OLED���
//		OLED_Printf(0, 0, 8, "Kp:%+04.2f", Turn.Kp);
//		OLED_Printf(0, 16, 8, "Out:%+04.5f", Turn.Out);
//		OLED_Printf(0, 32, 8, "Act:%+04.2f", Turn.Actual);
//		OLED_Printf(0, 48, 8, "Num:%+04.2f", Num);
//		
//		Serial_Printf("%f, %f, %f\r\n", R_Actual, R_Target, R_Out);

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
		
		//小车任务执行函数均在Pid_User.h当中
		
		//走直线
		zouzhixian();
		//左转圆弧循迹
		Zuo_xunji();
		//角度转向
		zhuanxiang(56, 0.2);
		//走直线
		zouzhixian();
		//右转圆弧转向
		You_xunji();
		//角度转向
		zhuanxiang(-56, 0.2);
		//走直线
		zouzhixian();

	}
}

