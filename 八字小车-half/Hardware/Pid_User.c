#include "Pid.h"
#include "math.h"

extern float T_Actual, T_Target;
float Angle, Angle_flag; 
void Angle_turn(float Num)
{
	while(1)
	{
		Angle_flag = 1;
		Angle = Num;
		if(fabs(T_Actual - T_Target) < 1)
			break;
	}
	Angle_flag = 0;
}