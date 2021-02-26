/**
  ******************************************************************************
  * @file    $ProjectName/User/debug/debug.h 
  * @author  WX
  * @version V1.0.0
  * @date    2020-7-9
  * @brief   debug.c file configuration.
  ******************************************************************************
  */

#ifndef DEBUG_H
#define DEBUG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DEBUG_MCO_GPIOInit(void);

/* Exported variables --------------------------------------------------------*/
extern uint32_t debugVolTime;

#endif  // #ifndef DEBUG_H
