#include "Pid_User.h"

extern float Roll, Pitch, Yaw, Num;
extern float L_Actual , L_Out, L_Target, L_ErrorInt;
extern float R_Actual , R_Out, R_Target, R_ErrorInt;
extern float T_Actual , T_Out, T_Target, T_ErrorInt;
extern float zuosudu, yousudu;
extern float V;
extern int16_t Outer;
extern uint16_t flag ;
void Right_Amend(void);
void Left_Amend(void);

void zouzhixian(void)
{
		flag = 2;
		while(flag == 2)
		{
			OLED_Printf(0, 0, 8, "Roll:%3.2f", Roll);
			OLED_Printf(0, 16, 8, "Pitch:%3.2f", Pitch);
			OLED_Printf(0, 32, 8, "Yaw:%3.2f", Yaw);
			OLED_Printf(0, 48, 8, "flag = %d", flag);	
			
			OLED_Update();
			JY901_GetAngle(&Roll, &Pitch, &Yaw);
			while(D1 == 1)
			{
				flag = 0;
				break;
			}
		}
}

void Zuo_xunji(void)
{
		flag = 3;
		while(flag == 3)
		{
			OLED_Printf(0, 0, 8, "Roll:%3.2f", Roll);
			OLED_Printf(0, 16, 8, "Pitch:%3.2f", Pitch);
			OLED_Printf(0, 32, 8, "Yaw:%3.2f", Yaw);
			OLED_Printf(0, 48, 8, "flag = %d", flag);	
			JY901_GetAngle(&Roll, &Pitch, &Yaw);
			OLED_Update();
			Left_Amend();
			if(D1 == 0 && D2 == 0 && D3 == 0 && D4 == 0 && D5 == 0)
			{
				Delay_ms(50);
				if(D1 == 0 && D2 == 0 && D3 == 0 && D4 == 0 && D5 == 0)
					flag = 0;
			}
		}
}
void You_xunji(void)
{
		flag = 3;
		while(flag == 3)
		{
			OLED_Printf(0, 0, 8, "Roll:%3.2f", Roll);
			OLED_Printf(0, 16, 8, "Pitch:%3.2f", Pitch);
			OLED_Printf(0, 32, 8, "Yaw:%3.2f", Yaw);
			OLED_Printf(0, 48, 8, "flag = %d", flag);	
			JY901_GetAngle(&Roll, &Pitch, &Yaw);
			OLED_Update();
			Right_Amend();
			if(D1 == 0 && D2 == 0 && D3 == 0 && D4 == 0 && D5 == 0)
			{
				Delay_ms(50);
				if(D1 == 0 && D2 == 0 && D3 == 0 && D4 == 0 && D5 == 0)
					flag = 0;
			}
		}
}

void zhuanxiang(float Target, float error)
{
		JY901_GetAngle(&Roll, &Pitch, &Yaw);
		T_Target = Yaw + Target;
		PID_R(0);		
		PID_L(0);
		L_ErrorInt = 0;
		R_ErrorInt = 0;
		flag = 1;
		while(flag == 1)
		{
			JY901_GetAngle(&Roll, &Pitch, &Yaw);
			OLED_Printf(0, 0, 8, "Roll:%3.2f", Roll);
			OLED_Printf(0, 16, 8, "Pitch:%3.2f", Pitch);
			OLED_Printf(0, 32, 8, "Yaw:%3.2f", Yaw);	
			OLED_Printf(0, 48, 8, "flag = %d", flag);	
			OLED_Update();
			while(fabs(T_Actual - T_Target) < error)
			{
				flag = 0;
				break;
			}
		}
}
void Left_Amend(void)
{
	if (D3 == 1)
	{
		Move_Forward();
	}
   else if(D4 == 1)
	{
    Move_Forward();
  }
  else if(D2 == 1)
  {
		Turn_SLeft();
  }
  else if(D1 == 1)
	{
     Turn_LLeft();
  }
  else if(D5 == 1)
  {
	  Move_Forward();
  }
}

void Right_Amend(void)
{
	if (D3 == 1)
	{
		Move_Forward();
	}
    else if(D4 == 1)
	{
        Turn_SRight();
    }
    else if(D2 == 1)
    {
	    Move_Forward();
    }
    else if(D1 == 1)
	{
        Move_Forward();
    }
    else if(D5 == 1)
    {
	    Turn_LRight();
    }
}
