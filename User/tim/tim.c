#include "tim.h"
#include "key.h"
#include "LG3641AH.h"
#include "user_menu.h"
#include "usart.h"

#define CURSOR_BLINK_TIME																(450)

uint16_t cursorBlinkTimeCnt = 0;
uint16_t usartRevTimeCnt = 0;

uint32_t timerCnt = 0;
uint8_t timerStartFlag = 0;

extern uint8_t usartRevStartFlag;
extern uint8_t usartRevBufferIdx;

void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0x00;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = SystemCoreClock / 1000 - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);
}

//uint8_t flag = 0;
extern uint32_t printDebugTimer;
uint16_t timeTest = 50;
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{		
		KEY_Scan();
		
		SegDispTimeCounter++;
		
		// cursor blink timer
		if(GET_BLINK_CURSOR_START_TIMER())
		{
			cursorBlinkTimeCnt++;
			
			if(cursorBlinkTimeCnt >= CURSOR_BLINK_TIME)
			{
				cursorBlinkTimeCnt = 0;
				
				if(GET_BLINK_CURSOR_TIMEROUT())
				{
					CLR_BLINK_CURSOR_TIMEROUT();
				}
				else
				{
					SET_BLINK_CURSOR_TIMEROUT();
				}
			}
		}
		
		// usart receive data timer
		if(usartRevStartFlag == 1)
		{
			usartRevTimeCnt++;
			
			if(usartRevTimeCnt >= 60) // usart receive finished.
			{			
				usartRevStartFlag = 0;
			}
		}
		
		// timer
		if(timerStartFlag == 1)
		{
			timerCnt--;
			if(timerCnt == 0)
			{
				timerStartFlag = 0;
			}
		}
		
		printDebugTimer++;
		
		if(timeTest > 0)
		{
			timeTest--;
		}
		
//		if(flag)
//		{
//			flag = 0;
//			GPIO_SetBits(GPIOA, GPIO_Pin_4);
//		}
//		else
//		{
//			flag = 1;
//			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
//		}
//		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void Delay_ms(uint32_t time)
{
	timerStartFlag = 1;
	timerCnt = time;
	
	while(timerCnt);
}
