/**
  ******************************************************************************
  * @file    $ProjectName/iwdg/iwdg.c 
  * @author  WX
  * @version V1.0.0
  * @date    2020-7-9
  * @brief   watch dog.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "iwdg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  IWDG Configuration.
  * @param  None
  * @retval None
  */
void IWDG_Config(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	IWDG_SetPrescaler(IWDG_Prescaler_128);
	
	IWDG_SetReload(20);
	
	IWDG_ReloadCounter();
	
	IWDG_Enable();
}

/**
  * @brief  IWDG feed.
  * @param  None
  * @retval None
  */
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}
