/**
  ******************************************************************************
  * @file    $ProjectName/User/debug/debug.c 
  * @author  WX
  * @version V1.0.0
  * @date    2020-7-9
  * @brief   Debug function code.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "debug.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t debugVolTime = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Sys clock output.
  * @param  None
  * @retval None
  */
void DEBUG_MCO_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	RCC_MCOConfig(RCC_MCOSource_SYSCLK, RCC_MCODiv_1);
}
