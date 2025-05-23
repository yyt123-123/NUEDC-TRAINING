#include "stm32f10x.h"                  // Device header
#include "Motor.h"

void Move_Forward(void)
{
	Motor_R(400);
	Motor_L(400);
}

void Turn_SLeft(void)
{
	Motor_R(400);
	Motor_L(300);
}
void Turn_SRight(void)
{
	Motor_R(300);
	Motor_L(400);
}

void Turn_LLeft(void)
{
	Motor_R(400);
	Motor_L(200);
}
void Turn_LRight(void)
{
	Motor_R(200);
	Motor_L(400);
}
