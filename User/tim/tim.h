/**
  ******************************************************************************
  * @file    $ProjectName/User/tim/tim.h 
  * @author  WX
  * @version V1.0.0
  * @date    2020-7-9
  * @brief   tim.c file configuration.
  ******************************************************************************
  */

#ifndef TIM_H
#define TIM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void TIM3_Init(void);
void Delay_ms(uint32_t time);

/* Exported variables --------------------------------------------------------*/

#endif // #ifndef TIM_H