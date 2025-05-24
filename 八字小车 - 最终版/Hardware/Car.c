#include "stm32f10x.h"                  // Device header
#include "Motor.h"

void Move_Forward(void)
{
	Motor_R(150);
	Motor_L(150);
}

void Turn_SLeft(void)
{
	Motor_R(300);
	Motor_L(70);
}

void Turn_SRight(void)
{
	Motor_R(70);
	Motor_L(300);
}

void Turn_LLeft(void)
{
	Motor_R(300);
	Motor_L(50);
}
void Turn_LRight(void)
{
	Motor_R(50);
	Motor_L(300);
}
void Car_Stop(void)
{
	Motor_R(0);
	Motor_L(0);
}	
void Turn_circle(void)
{
	Motor_R(120);
	Motor_L(-120);
}
