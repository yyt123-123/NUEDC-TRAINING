#ifndef __PID_USER_H
#define __PID_USER_H
#include "stm32f10x.h"                  // Device header
#include "Pid.h"
#include "math.h"
#include "OLED.h"
#include "JY901.h"
#include "Delay.h"
#include "Car.h"

#define D1			GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)
#define D2			GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)
#define D3			GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)
#define D4			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)
#define D5			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)
#define D6			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)
#define D7			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)

/*----------------------------------------------------------------------------
 * 直线行走控制模块
 *
 * 功能描述：使设备保持直线行走状态
 * 显示信息：实时更新Roll/Pitch/Yaw角度和运行标志flag
 * 退出条件：检测到D1 或 D5引脚状态为高电平(1)时退出
 *----------------------------------------------------------------------------*/
void zouzhixian(void);

/*----------------------------------------------------------------------------
 * 左寻迹控制模块
 *
 * 功能描述：基于D1-D5传感器信号实现左转优先的循迹策略
 * 信号处理：
 *   - D3/D4/D5=1时直行（Move_Forward）
 *   - D2=1时小左转（Turn_SLeft）
 *   - D1=1时大左转（Turn_LLeft）
 * 退出条件：所有传感器(D1-D5)均无信号时延时退出
 *----------------------------------------------------------------------------*/
void Zuo_xunji(void);

/*----------------------------------------------------------------------------
 * 右寻迹控制模块
 *
 * 功能描述：基于D1-D5传感器信号实现右转优先的循迹策略
 * 信号处理：
 *   - D3/D1/D2=1时直行（Move_Forward）
 *   - D4=1时小右转（Turn_SRight）
 *   - D5=1时大右转（Turn_LRight）
 * 退出条件：所有传感器(D1-D5)均无信号时延时退出
 *----------------------------------------------------------------------------*/
void You_xunji(void);

/*----------------------------------------------------------------------------
 * 角度转向控制模块
 *
 * @param Target 目标偏航角偏移量(°)，目标角度=当前Yaw+Target
 *
 * 功能描述：控制设备转向至指定角度
 * 工作流程：
 *   1. 获取当前Yaw值，计算绝对目标角度T_Target
 *   2. 重置PID控制器参数及误差积分项
 *   3. 循环监测角度误差，误差< error 时完成转向
 *----------------------------------------------------------------------------*/
void zhuanxiang(float Target, float error);

/*----------------------------------------------------------------------------
 * 左路径修正模块
 *
 * 功能描述：根据传感器信号执行左转方向修正
 * 传感器逻辑：
 *   D1=1 → 大左转（Turn_LLeft）
 *   D2=1 → 小左转（Turn_SLeft）
 *   D3/D4/D5=1 → 直行（Move_Forward）
 *----------------------------------------------------------------------------*/
void Left_Amend(void);

/*----------------------------------------------------------------------------
 * 右路径修正模块
 *
 * 功能描述：根据传感器信号执行右转方向修正
 * 传感器逻辑：
 *   D4=1 → 小右转（Turn_SRight）
 *   D5=1 → 大右转（Turn_LRight）
 *   D1/D2/D3=1 → 直行（Move_Forward）
 *----------------------------------------------------------------------------*/
#endif
