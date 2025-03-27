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

extern int16_t zuosudu, yousudu;
extern float Actual, Out, Target;

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
		OLED_Printf(0, 0, 8, "Tar:%+04.0f", Target);
		OLED_Printf(0, 16, 8, "Act:%+04.0f", Actual);
		OLED_Printf(0, 32, 8, "Out:%+04.0f", Out);

		OLED_Update();
		
		Serial_Printf("%f,%f,%f\r\n",Target, Actual, Out);
		
	}
}


