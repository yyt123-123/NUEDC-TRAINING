#ifndef __PID_H
#define __PID_H
#include "stdint.h"
#include "User_PID.h"
#include "Motor.h"
#include "Encoder.h"
#include "PWM.h"
#include "Timer.h"

void Pid_Position(float Target);//在定时器里面调用
extern PID_t Inner;
extern PID_t Outer;

#endif
