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
#include "JY901.h"

extern int16_t zuosudu, yousudu;
extern float Actual, Out;
extern uint8_t JY901_Flag;
float Pitch, Roll, Yaw;
	
int main(void)
{
	OLED_Init();
    Encoder_Init();
	
	Timer_Init();
	PWM_Init();
	Motor_Init();
	Serial_Init();
	JY901_Init();
	while(1)
	{		
		JY901_GetAngle(&Pitch, &Roll, &Yaw);
		OLED_Printf(0, 0, 8, "Roll:%3.2f", Roll);
		OLED_Printf(0, 16, 8, "Pitch:%3.2f", Pitch);
		OLED_Printf(0, 32, 8, "Yaw:%3.2f", Yaw);		
		
		OLED_Update();
	}
}


