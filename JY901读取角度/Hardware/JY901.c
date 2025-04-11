#include "stm32f10x.h"                  // Device header
#include "Serial.h"

extern uint8_t Serial_RxData;
extern uint8_t Serial_RxFlag;
uint8_t Angle_Packet[10];
uint8_t JY901_Flag;
uint8_t RollH, RollL ,PitchH, PitchL, YawH, YawL; 
int16_t Roll_B, Pitch_B, Yaw_B;
float P, R, Y;

//通信串口初始化
void JY901_Init(void)
{
	Serial_Init();
}

//角度值数据解算
void JY901_GetAngle(float *Pitch, float *Roll, float *Yaw)
{
	if(Angle_Packet[0] == 0x53)
	{
		 RollL = Angle_Packet[1];
		 RollH = Angle_Packet[2];
		
		 PitchL = Angle_Packet[3];
		 PitchH = Angle_Packet[4];
                   
		 YawL= Angle_Packet[5];
		 YawH = Angle_Packet[6];

		Roll_B = (int16_t)((RollH << 8) | RollL);
		Pitch_B = (int16_t)((PitchH << 8) | PitchL);
		Yaw_B = (int16_t)((YawH << 8) | YawL);
		
		*Roll = ((float)Roll_B / 32768.0) * 180.0;
		*Pitch = ((float)Pitch_B / 32768.0) * 180.0;
		*Yaw = ((float)Yaw_B / 32768.0) * 180.0;

	}
}

void USART3_IRQHandler(void)
{
	static uint8_t AddState = 0;
	static uint8_t pRxState = 0;
	
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
		uint8_t RxData = USART_ReceiveData(USART3);
		if(AddState == 0)
		{
			if(RxData == 0x55)
			{
				AddState = 1;
			}
		}
		else if(AddState == 1)
		{
			Angle_Packet[pRxState++] = USART_ReceiveData(USART3);
			if(pRxState == 1 && Angle_Packet[0] != 0x53)
			{
				if(Angle_Packet[0] == 0x55)
					pRxState = 0;	
				else
				{
					AddState = 0;
					pRxState = 0;
				}
			}
			else if(pRxState == 10)
			{
				JY901_Flag = 1;
				AddState = 0;
				pRxState = 0;
			}
			else
				JY901_Flag = 0;
		}
			JY901_GetAngle(&P, &R, &Y);    
	}
}    

