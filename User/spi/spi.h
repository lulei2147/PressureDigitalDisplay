/**
  ******************************************************************************
  * @file    $ProjectName/spi/spi.h 
  * @author  WX
  * @version V1.0.0
  * @date    2021-1-8
  * @brief   spi.c file configuration.
  ******************************************************************************
  */

#ifndef SPI_H
#define SPI_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "usart.h"

/* Exported macro ------------------------------------------------------------*/
#define SPI_CSB_ENABLE()                                      (GPIO_ResetBits(GPIOA, GPIO_Pin_4))
#define SPI_CSB_DISABLE()                                     (GPIO_SetBits(GPIOA, GPIO_Pin_4))

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SPI_InitConfig(void);
uint8_t SPI_WriteByte(uint8_t data);
uint8_t SPI_ReadByte(uint8_t data);

/* Exported variables --------------------------------------------------------*/
extern uint8_t usartRevBuffer[USART_REV_BUFFER_SIZE];

#endif // #ifndef SPI_H
