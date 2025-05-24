#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_13 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	PWM_Init();
}

void Motor_R(int16_t PWM)
{
	if (PWM >= 0)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);
		GPIO_SetBits(GPIOB, GPIO_Pin_14);
		PWM_SetCompare4(PWM);
	}
	else
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_15);
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);
		PWM_SetCompare4(-PWM);
	}
}

void Motor_L(int16_t PWM)
{
	if (PWM >= 0)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		PWM_SetCompare1(PWM);
	}
	else
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_13);
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		PWM_SetCompare1(-PWM);
	}
}
