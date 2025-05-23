#include "stm32f10x.h"                  // Device header
#include "Serial.h"

static uint8_t RxBuffer[11];/*接收数据数组*/
static volatile uint8_t RxState = 0;/*接收状态标志位*/
static uint8_t RxIndex = 0;/*接受数组索引*/
float Ro,Pit,Ya;/*角度信息，如果只需要整数可以改为整数类型*/
float R, P, Y;

//通信串口初始化
void JY901_Init(void)
{
	// 使能 USART2 和 GPIOA 的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	// 配置 PA2 为复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 配置 PA3 为上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);
	
	// 使能 USART2 的接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	// NVIC 中断分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	// 使能 USART2
	USART_Cmd(USART2, ENABLE);
}

//角度值数据解算
void JY901_GetAngle(float *R, float *P, float *Y)
{
	*R = ((int16_t) ((int16_t) RxBuffer[3] << 8 | (int16_t) RxBuffer[2])) / 32768.0f * 180.0f;
	*P = ((int16_t) ((int16_t) RxBuffer[5] << 8 | (int16_t) RxBuffer[4])) / 32768.0f * 180.0f;
	*Y = ((int16_t) ((int16_t) RxBuffer[7] << 8 | (int16_t) RxBuffer[6])) / 32768.0f * 180.0f;
}



/**
 * @brief       数据包处理函数
 * @param       串口接收的数据RxData
 * @retval      无
 */
void jy901_ReceiveData(uint8_t RxData)
{
		uint8_t i,sum=0;
	if (RxState == 0)	//等待包头
	{
		if (RxData == 0x55)	//收到包头
		{
			RxBuffer[RxIndex] = RxData;
			RxState = 1;
			RxIndex = 1;	//进入下一状态
		}
	}
	else if (RxState == 1)
	{
		if (RxData == 0x53)	/*判断数据内容，修改这里可以改变要读的数据内容，0x53为角度输出*/
		{
			RxBuffer[RxIndex] = RxData;
			RxState = 2;
			RxIndex = 2;	//进入下一状态
		}
	}
	else if (RxState == 2)	//接收数据
	{
		RxBuffer[RxIndex++] = RxData;
		if(RxIndex == 11)	//接收完成
		{
			for(i=0;i<10;i++)
			{
				sum = sum + RxBuffer[i];	//计算校验和
			}
		if(sum == RxBuffer[10])		//校验成功
		{
			/*计算数据，根据数据内容选择对应的计算公式*/
			JY901_GetAngle(&P, &R, &Y);	
		}
			RxState = 0;
			RxIndex = 0;	//读取完成，回到最初状态，等待包头
		}
	}
}

void USART2_IRQHandler(void)
{
	uint8_t RxData;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		RxData = USART_ReceiveData(USART2);	/*读取接收到的数据*/
		
		jy901_ReceiveData(RxData);	/*调用数据包处理函数*/
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
