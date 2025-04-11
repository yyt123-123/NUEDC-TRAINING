#include "stm32f10x.h"                  // Device header
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
#include "Pid.h"
#include "User_PID.h"

extern int16_t zuosudu, yousudu;

int main(void)
{
	OLED_Init();
    Encoder_Init();
	
	Timer_Init();
	PWM_Init();
	Motor_Init();
	Serial_Init();
		
	while(1)
	{
		OLED_Printf(0, 0, 8, "2*PID Control");		
		OLED_Printf(0, 16, 8, "Act:%+04.0f", Outer.Actual);
		OLED_Printf(0, 32, 8, "Tar:%+04.0f", Outer.Target);
		OLED_Printf(0, 48, 8, "Out:%+04.0f", Outer.Out);

		OLED_Update();
		
		Serial_Printf("%f,%f,%f\r\n", Outer.Actual, Outer.Target,Outer.Out);
//		
//		OLED_Printf(0, 0, 8, "2*PID Control");		
//		OLED_Printf(0, 16, 8, "Act:%+04.0f", InnerActual);
//		OLED_Printf(0, 32, 8, "Tar:%+04.0f", InnerTarget);
//		OLED_Printf(0, 48, 8, "Out:%+04.0f", InnerOut);

//		OLED_Update();
//		
//		Serial_Printf("%f,%f,%f\r\n", InnerActual, InnerTarget,InnerOut);
//		
	}
}


