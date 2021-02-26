/**
  ******************************************************************************
  * @file    $ProjectName/usart/receiveCmd.c 
  * @author  WX
  * @version V1.0.0
  * @date    2021-2-23
  * @brief   Command handler from host computer.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "receiveCmd.h"
#include "usart.h"
#include "eeprom.h"
#include "spi.h"
#include "NSA2860.h"
#include "key.h"
#include "math.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t spiRevData = 0;

/* Private functions ---------------------------------------------------------*/
void CmdFromHostComputerHandler(void)
{
	USART_UpdateCmdBuffer();
	if(usartRevCmdBuffer[0] != 0x00)
	{
		spiRevData = 0;
		
		SPI_CSB_ENABLE();
		
		if(usartRevCmdBuffer[0] == 0xA0)
		{
//				spiRevData = SPI_WriteByte(0x80);
//				spiRevData = SPI_WriteByte(usartRevCmdBuffer[1]);
//				spiRevData = SPI_WriteByte(0xFF);
			
			uint8_t cmdHeader = 0xA0;
			Usart_WriteData(&cmdHeader, 1);
			
			// lower and upper limit
			Usart_WriteData((uint8_t *)&stSysAdjValSection1.sensorRgeUpper, 2);
			Usart_WriteData(&stSysAdjValSection1.sensorRgeUpperDecimal, 1);
			Usart_WriteData((uint8_t *)&stSysAdjValSection1.sensorRgeLower, 2);
			Usart_WriteData(&stSysAdjValSection1.sensorRgeLowerDecimal, 1);
			// unit
			Usart_WriteData((uint8_t *)&stSysAdjValSection1.eUnit, 1);
			// DAP
			Usart_WriteData(&stSysAdjValSection1.dap, 1);
			// P-L
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].PL, 2);
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].PLDecimal, 1);
			// P-H
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].PH, 2);
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].PHDecimal, 1);
			// Func1
			Usart_WriteData((uint8_t *)&stSysAdjValSection1.eFunc1, 1);
			// AL1
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].AL1, 2);
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].AL1Decimal, 1);
			// AH1
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].AH1, 2);
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].AH1Decimal, 1);
			// DL1
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].AlDL1, 2);
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].A1DL1Decimal, 1);
			// Func2
			Usart_WriteData((uint8_t *)&stSysAdjValSection1.eFunc2, 1);
			// AL2
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].AL2, 2);
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].AL2Decimal, 1);
			// AH2
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].AH2, 2);
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].AH2Decimal, 1);
			// DL2
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].A2DL2, 2);
			Usart_WriteData((uint8_t *)&stSysAdjValSection2[stSysAdjValSection1.eUnit].A2DL2Decimal, 1);
			printf("\r\n");
			//printf("cmd: 0x%X,   4.received data: 0x%X \n", usartRevCmdBuffer[1], spiRevData);
		}
		else if(usartRevCmdBuffer[0] == 0xA1)
		{
			SPI_WriteByte(0x00);
			SPI_WriteByte(0x30);
			SPI_WriteByte(0x00);
			
			SPI_WriteByte(0x00);
			SPI_WriteByte(0xCF);
			SPI_WriteByte(usartRevCmdBuffer[1]);
			
			SPI_WriteByte(0x80);
			SPI_WriteByte(0xCF);
			spiRevData = SPI_WriteByte(0xFF);
			printf("reg0xCF: 0x%X\n", spiRevData);
		}
		else if(usartRevCmdBuffer[0] == 0xA2) // Temperature
		{
			NSA2860_LoadTempFromIntSensor(NULL);
		}
		else if(usartRevCmdBuffer[0] == 0xA3) // DAC Write
		{
			NSA2860_DACWrite(usartRevCmdBuffer[1], usartRevCmdBuffer[2]);
		}
		else if(usartRevCmdBuffer[0] == 0xA4) // DAC Read
		{
			uint8_t dacVal = 0;
			
			SPI_WriteByte(0x80);
			SPI_WriteByte(0x13);
			dacVal = SPI_WriteByte(0xFF);
			printf("DAC_DATA [7:0]: 0x%X\n", dacVal);
			
			SPI_WriteByte(0x80);
			SPI_WriteByte(0x12);
			dacVal = SPI_WriteByte(0xFF);
			printf("DAC_DATA [15:8]: 0x%X\n", dacVal);
		}
		else if(usartRevCmdBuffer[0] == 0xA5) // ADC
		{
			uint8_t adc_L = 0;
			uint8_t adc_M = 0;
			uint8_t adc_H = 0;
			uint32_t adcVal = 0;
			//float voltage = 0;
			
			SPI_WriteByte(0x80);
			SPI_WriteByte(0x06);
			adc_H = SPI_WriteByte(0xFF);
			
			SPI_WriteByte(0x80);
			SPI_WriteByte(0x07);
			adc_M = SPI_WriteByte(0xFF);
			
			SPI_WriteByte(0x80);
			SPI_WriteByte(0x08);
			adc_L = SPI_WriteByte(0xFF);
			
			printf("PDATA [7:0]: 0x%X\n", adc_L);
			printf("PDATA [15:8]: 0x%X\n", adc_M);
			printf("PDATA [23:16]: 0x%X\n", adc_H);
			if(adc_H & 0x80)
			{
				adcVal = 0xFF000000 | (((uint32_t)adc_H) << 16) | (((uint32_t)adc_M) << 8) | adc_L;
				printf("ADC Value: %d\n", adcVal);			
				printf("voltage: %f V\n", 0 - ((~(adcVal - 1)) * 2500 / pow(2, 23) / 1000.0f));
			}
			else
			{
				adcVal = (((uint32_t)adc_H) << 16) | (((uint32_t)adc_M) << 8) | adc_L;
				printf("ADC Value: %d\n", adcVal);
				printf("voltage: %f V\n", (double)adcVal * 2500 / pow(2, 23) / 1000.0f);
			}
		}
		else if(usartRevCmdBuffer[0] == 0xA6)
		{
			extern uint32_t keyUpTimeCounter;
			if(usartRevCmdBuffer[1] == 0xB0) // UP
			{
				SET_KEY_STATUS(eKEY_UP_PRESSED);
			}
			else if(usartRevCmdBuffer[1] == 0xB1) // DOWN
			{
				SET_KEY_STATUS(eKEY_DOWN_PRESSED);
			}
			else if(usartRevCmdBuffer[1] == 0xB2) // M
			{
				SET_KEY_STATUS(eKEY_M_PRESSED);
			}
			else if(usartRevCmdBuffer[1] == 0xB3) // S
			{
				SET_KEY_STATUS(eKEY_S_PRESSED);
			}
		}
		else if(usartRevCmdBuffer[0] == 0xA7)
		{
			EEPROM_RestoreFactoryDefaultValue();
		}
		
		SPI_CSB_DISABLE();
		
		memset(usartRevCmdBuffer, 0, USART_REV_BUFFER_SIZE);
	}
}
