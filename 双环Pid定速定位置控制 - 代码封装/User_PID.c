#include "stm32f10x.h"                  // Device header
#include "User_Pid.h"

void PID_Update(PID_t *p, float OutMax, float OutMin)
{
	p->Error1 = p->Error0;
	p->Error0 = p->Target - p->Actual;
	
	if(p->Ki != 0)
	{
		p->ErrorInt += p->Error0;
	}
	else
	{
		p->ErrorInt = 0;
	}
	
	p->Out = p->Kp * p->Error0
		   + p->Ki * p->ErrorInt
		   + p->Kd * (p->Error0 - p->Error1);
	
	if(p->Out > OutMax){p->Out = OutMax;}
	if(p->Out < OutMin){p->Out = OutMin;}
	
}
