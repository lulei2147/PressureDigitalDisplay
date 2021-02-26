/**
  ******************************************************************************
  * @file    $ProjectName/spi/spi.c 
  * @author  WX
  * @version V1.0.0
  * @date    2021-1-8
  * @brief   SPI driver.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  SPI Configuration.
  * @param  None
  * @retval None
  */
void SPI_InitConfig(void)
{
	GPIO_InitTypeDef GPIOInitSturct;
	SPI_InitTypeDef SPIInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	// SCK PA5
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	// MISO PA6
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	// MOSI PA7
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	// GPIO Init
	GPIOInitSturct.GPIO_Mode = GPIO_Mode_AF;
	GPIOInitSturct.GPIO_OType = GPIO_OType_PP;
	GPIOInitSturct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOInitSturct.GPIO_Speed = GPIO_Speed_40MHz;
	
	// SCK PA5
	GPIOInitSturct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIOInitSturct);
	
	// SCK PA6
	GPIOInitSturct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIOInitSturct);
	
	// SCK PA7
	GPIOInitSturct.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIOInitSturct);
	
	// NNS
	GPIOInitSturct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOInitSturct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIOInitSturct);
	
	// SPI
	SPIInitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPIInitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPIInitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPIInitStruct.SPI_CRCPolynomial = 0;
	SPIInitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPIInitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPIInitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPIInitStruct.SPI_Mode = SPI_Mode_Master;
	SPIInitStruct.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI1, &SPIInitStruct);
	
	SPI_Cmd(SPI1, ENABLE);
}

/**
  * @brief  SPI write operation.
  * @param  Write data buffer.
  * @retval Data received.
  */
uint8_t SPI_WriteByte(uint8_t data)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, data);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}

/**
  * @brief  SPI Read operation.
  * @param  Write data buffer.
  * @retval Data received.
  */
uint8_t SPI_ReadByte(uint8_t data)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, data);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}
