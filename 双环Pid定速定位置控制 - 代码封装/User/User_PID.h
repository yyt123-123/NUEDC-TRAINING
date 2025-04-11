#ifndef __USER_PID_H
#define __USER_PID_H

typedef struct {
	float Target;
	float Actual;
	float Out;
	
	float Kp;
	float Ki;
	float Kd;
	
	float Error0;
	float Error1;
	float ErrorInt;
		
}PID_t;

void PID_Update(PID_t *p, float OutMax, float OutMin);

#endif
