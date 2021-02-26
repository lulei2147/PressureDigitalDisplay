/**
  ******************************************************************************
  * @file    $ProjectName/spi/NSA2860.h 
  * @author  WX
  * @version V1.0.0
  * @date    2021-1-8
  * @brief   NSA2860.c file configuration.
  ******************************************************************************
  */

#ifndef NSA2860_H
#define NSA2860_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void NSA2860_Init(void);
void NSA2860_ReadDataFromGeneralReg(uint8_t regAddr, uint8_t *pBuffer, uint8_t length);
void NSA2860_LoadTempFromIntSensor(uint8_t *pData);
void NSA2860_DACWrite(uint8_t data_L, uint8_t data_H);

/* Exported variables --------------------------------------------------------*/

#endif // #ifndef NSA2860_H
