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
typedef struct{
	uint8_t lowLimit_H;
	uint8_t lowLimit_L;
	uint8_t lowLimitDecimal;
	
	uint8_t upperLimit_H;
	uint8_t upperLimit_L;
	uint8_t upperLimitDecimal;
	
	uint8_t Unit;
	uint8_t dap;
	
	uint8_t PL_H;
	uint8_t PL_L;
	uint8_t PLDecimal;
	
	uint8_t PH_H;
	uint8_t PH_L;
	uint8_t PHDecimal;
	
	uint8_t Func1;
	uint8_t AL1_H;
	uint8_t AL1_L;
	uint8_t AL1Decimal;
	uint8_t AH1_H;
	uint8_t AH1_L;
	uint8_t AH1Decimal;
	uint8_t DL1_H;
	uint8_t DL1_L;
	uint8_t DL1Decimal;
	
	uint8_t Func2;
	uint8_t AL2_H;
	uint8_t AL2_L;
	uint8_t AL2Decimal;
	uint8_t AH2_H;
	uint8_t AH2_L;
	uint8_t AH2Decimal;
	uint8_t DL2_H;
	uint8_t DL2_L;
	uint8_t DL2Decimal;
}STDataToBeSaved;
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t spiRevData = 0;
STDataToBeSaved stDataToBeSaved;

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
//			SPI_WriteByte(0x00);
//			SPI_WriteByte(0x30);
//			SPI_WriteByte(0x00);
//			
//			SPI_WriteByte(0x00);
//			SPI_WriteByte(0xCF);
//			SPI_WriteByte(usartRevCmdBuffer[1]);
//			
//			SPI_WriteByte(0x80);
//			SPI_WriteByte(0xCF);
//			spiRevData = SPI_WriteByte(0xFF);
//			printf("reg0xCF: 0x%X\n", spiRevData);
			uint8_t bitMapLow = usartRevCmdBuffer[1];
			uint8_t bitMapHigh = usartRevCmdBuffer[2];
			uint8_t unit = stSysAdjValSection1.eUnit;
			
			// Sensor Range Lower
			if(bitMapLow & _BIT0)
			{
				// data to be saved
				stSysSaveDataSection1.sensorRgeLower_H = usartRevCmdBuffer[3];
				stSysSaveDataSection1.sensorRgeLower_L = usartRevCmdBuffer[4];
				stSysSaveDataSection1.sensorRgeLowerDecimal = usartRevCmdBuffer[5];
				// adjust
				stSysAdjValSection1.sensorRgeLower = (uint16_t)stSysSaveDataSection1.sensorRgeLower_H << 8 | stSysSaveDataSection1.sensorRgeLower_L;
				stSysAdjValSection1.sensorRgeLowerDecimal = stSysSaveDataSection1.sensorRgeLowerDecimal;
				// save to eeprom
				EEPROM_WriteBytes(eSENSOR_RANGE_LOWER_ADDR, &stSysSaveDataSection1.sensorRgeLower_H, 2);
				EEPROM_WriteBytes(eSENSOR_RANGE_LOWER_DECIMAL_ADDR, &stSysSaveDataSection1.sensorRgeLowerDecimal, 1);
			}
			// Sensor Range Upper
			if(bitMapLow & _BIT1)
			{
				// data to be saved
				stSysSaveDataSection1.sensorRgeUpper_H = usartRevCmdBuffer[6];
				stSysSaveDataSection1.sensorRgeUpper_L = usartRevCmdBuffer[7];
				stSysSaveDataSection1.sensorRgeUpperDecimal = usartRevCmdBuffer[8];
				// adjust
				stSysAdjValSection1.sensorRgeUpper = (uint16_t)stSysSaveDataSection1.sensorRgeUpper_H << 8 | stSysSaveDataSection1.sensorRgeUpper_L;
				stSysAdjValSection1.sensorRgeUpperDecimal = stSysSaveDataSection1.sensorRgeUpperDecimal;
				// save to eeprom
				EEPROM_WriteBytes(eSENSOR_RANGE_UPPER_ADDR, &stSysSaveDataSection1.sensorRgeUpper_H, 2);
				EEPROM_WriteBytes(eSENSOR_RANGE_UPPER_DECIMAL_ADDR, &stSysSaveDataSection1.sensorRgeUpperDecimal, 1);
			}
			// Sensor Unit
			if(bitMapLow & _BIT2)
			{
				stSysSaveDataSection1.eUnit = (EnumValUnit)usartRevCmdBuffer[9];
				stSysAdjValSection1.eUnit = stSysSaveDataSection1.eUnit;
				EEPROM_WriteBytes(eSENSOR_P_UNIT_ADDR, (uint8_t *)&stSysAdjValSection1.eUnit, 1);
			}
			// DAP
			if(bitMapLow & _BIT3)
			{
				stSysSaveDataSection1.dap = usartRevCmdBuffer[10];
				stSysAdjValSection1.dap = stSysSaveDataSection1.dap;
				EEPROM_WriteBytes(eSENSOR_DAP, &stSysSaveDataSection1.dap, 1);
			}
			// P-L
			if(bitMapLow & _BIT4)
			{
				if(unit == UNIT_PSI)
				{
				
				}
				else
				{
					stSysSaveDataSection2[UNIT_BAR].PL_H = usartRevCmdBuffer[11];
					stSysSaveDataSection2[UNIT_BAR].PL_L = usartRevCmdBuffer[12];
					stSysSaveDataSection2[UNIT_BAR].PLDecimal = usartRevCmdBuffer[13];
					EEPROM_WriteBytes(eBAR_P_L_ADDR, &stSysSaveDataSection2[UNIT_BAR].PL_H, 2);
					EEPROM_WriteBytes(eBAR_P_L_DECIMAL_ADDR, &stSysSaveDataSection2[UNIT_BAR].PLDecimal, 1);
					
					stSysAdjValSection2[UNIT_BAR].PL = (uint16_t)stSysSaveDataSection2[UNIT_BAR].PL_H << 8 | stSysSaveDataSection2[UNIT_BAR].PL_L;
					stSysAdjValSection2[UNIT_BAR].PLDecimal = stSysSaveDataSection2[UNIT_BAR].PLDecimal;
				}
			}
			// P-H
			if(bitMapLow & _BIT5)
			{
				if(unit == UNIT_PSI)
				{
				
				}
				else
				{
					stSysSaveDataSection2[UNIT_BAR].PH_H = usartRevCmdBuffer[14];
					stSysSaveDataSection2[UNIT_BAR].PH_L = usartRevCmdBuffer[15];
					stSysSaveDataSection2[UNIT_BAR].PHDecimal = usartRevCmdBuffer[16];
					EEPROM_WriteBytes(eBAR_P_H_ADDR, &stSysSaveDataSection2[UNIT_BAR].PH_H, 2);
					EEPROM_WriteBytes(eBAR_P_H_DECIMAL_ADDR, &stSysSaveDataSection2[UNIT_BAR].PHDecimal, 1);
					
					stSysAdjValSection2[UNIT_BAR].PH = (uint16_t)stSysSaveDataSection2[UNIT_BAR].PH_H << 8 | stSysSaveDataSection2[UNIT_BAR].PH_L;
					stSysAdjValSection2[UNIT_BAR].PHDecimal = stSysSaveDataSection2[UNIT_BAR].PHDecimal;
				}
			}
			// Func1
			if(bitMapLow & _BIT6)
			{
				stSysSaveDataSection1.eFunc1 = (EnumFuncMode)usartRevCmdBuffer[17];
				EEPROM_WriteBytes(eALM1_FUNC1_ADDR, (uint8_t *)&stSysSaveDataSection1.eFunc1, 1);
				
				stSysAdjValSection1.eFunc1 = stSysSaveDataSection1.eFunc1;
			}
			// AL1
			if(bitMapLow & _BIT7)
			{
				if(unit == UNIT_PSI)
				{
				
				}
				else
				{
					stSysSaveDataSection2[UNIT_BAR].AL1_H = usartRevCmdBuffer[18];
					stSysSaveDataSection2[UNIT_BAR].AL1_L = usartRevCmdBuffer[19];
					stSysSaveDataSection2[UNIT_BAR].AL1Decimal = usartRevCmdBuffer[20];
					EEPROM_WriteBytes(eBAR_A1_L_ADDR, &stSysSaveDataSection2[UNIT_BAR].AL1_H, 2);
					EEPROM_WriteBytes(eBAR_A1_L_DECIMAL_ADDR, &stSysSaveDataSection2[UNIT_BAR].AL1Decimal, 1);
					
					stSysAdjValSection2[UNIT_BAR].AL1 = (uint16_t)stSysSaveDataSection2[UNIT_BAR].AL1_H << 8 | stSysSaveDataSection2[UNIT_BAR].AL1_L;
					stSysAdjValSection2[UNIT_BAR].AL1Decimal = stSysSaveDataSection2[UNIT_BAR].AL1Decimal;
				}
			}
			// AH1
			if(bitMapHigh & _BIT0)
			{
				if(unit == UNIT_PSI)
				{
				
				}
				else
				{
					stSysSaveDataSection2[UNIT_BAR].AH1_H = usartRevCmdBuffer[21];
					stSysSaveDataSection2[UNIT_BAR].AH1_L = usartRevCmdBuffer[22];
					stSysSaveDataSection2[UNIT_BAR].AH1Decimal = usartRevCmdBuffer[23];
					EEPROM_WriteBytes(eBAR_A1_H_ADDR, &stSysSaveDataSection2[UNIT_BAR].AH1_H, 2);
					EEPROM_WriteBytes(eBAR_A1_H_DECIMAL_ADDR, &stSysSaveDataSection2[UNIT_BAR].AH1Decimal, 1);
					
					stSysAdjValSection2[UNIT_BAR].AH1 = (uint16_t)stSysSaveDataSection2[UNIT_BAR].AH1_H << 8 | stSysSaveDataSection2[UNIT_BAR].AH1_L;
					stSysAdjValSection2[UNIT_BAR].AH1Decimal = stSysSaveDataSection2[UNIT_BAR].AH1Decimal;
				}
			}
			// DL1
			if(bitMapHigh & _BIT1)
			{
				if(unit == UNIT_PSI)
				{
				
				}
				else
				{
					stSysSaveDataSection2[UNIT_BAR].AlDL1_H = usartRevCmdBuffer[24];
					stSysSaveDataSection2[UNIT_BAR].A1DL1_L = usartRevCmdBuffer[25];
					stSysSaveDataSection2[UNIT_BAR].A1DL1Decimal = usartRevCmdBuffer[26];
					EEPROM_WriteBytes(eBAR_A1_DL1_ADDR, &stSysSaveDataSection2[UNIT_BAR].AlDL1_H, 2);
					EEPROM_WriteBytes(eBAR_A1_DL1_DECIMAL_ADDR, &stSysSaveDataSection2[UNIT_BAR].A1DL1Decimal, 1);
					
					stSysAdjValSection2[UNIT_BAR].AlDL1 = (uint16_t)stSysSaveDataSection2[UNIT_BAR].AlDL1_H << 8 | stSysSaveDataSection2[UNIT_BAR].A1DL1_L;
					stSysAdjValSection2[UNIT_BAR].A1DL1Decimal = stSysSaveDataSection2[UNIT_BAR].A1DL1Decimal;
				}
			}
			// Func2
			if(bitMapHigh & _BIT2)
			{		
				stSysSaveDataSection1.eFunc2 = (EnumFuncMode)usartRevCmdBuffer[27];
				EEPROM_WriteBytes(eALM2_FUNC2_ADDR, (uint8_t *)&stSysSaveDataSection1.eFunc2, 1);
				
				stSysAdjValSection1.eFunc2 = stSysSaveDataSection1.eFunc2;
			}
			// AL2
			if(bitMapHigh & _BIT3)
			{		
				if(unit == UNIT_PSI)
				{
				
				}
				else
				{
					stSysSaveDataSection2[UNIT_BAR].AL2_H = usartRevCmdBuffer[28];
					stSysSaveDataSection2[UNIT_BAR].AL2_L = usartRevCmdBuffer[29];
					stSysSaveDataSection2[UNIT_BAR].AL2Decimal = usartRevCmdBuffer[30];
					EEPROM_WriteBytes(eBAR_A2_L_ADDR, &stSysSaveDataSection2[UNIT_BAR].AL2_H, 2);
					EEPROM_WriteBytes(eBAR_A2_L_DECIMAL_ADDR, &stSysSaveDataSection2[UNIT_BAR].AL2Decimal, 1);
					
					stSysAdjValSection2[UNIT_BAR].AL2 = (uint16_t)stSysSaveDataSection2[UNIT_BAR].AL2_H << 8 | stSysSaveDataSection2[UNIT_BAR].AL2_L;
					stSysAdjValSection2[UNIT_BAR].AL2Decimal = stSysSaveDataSection2[UNIT_BAR].AL2Decimal;
				}
			}
			// AH2
			if(bitMapHigh & _BIT4)
			{
				if(unit == UNIT_PSI)
				{
				
				}
				else
				{
					stSysSaveDataSection2[UNIT_BAR].AH2_H = usartRevCmdBuffer[31];
					stSysSaveDataSection2[UNIT_BAR].AH2_L = usartRevCmdBuffer[32];
					stSysSaveDataSection2[UNIT_BAR].AH2Decimal = usartRevCmdBuffer[33];
					EEPROM_WriteBytes(eBAR_A2_H_ADDR, &stSysSaveDataSection2[UNIT_BAR].AH2_H, 2);
					EEPROM_WriteBytes(eBAR_A2_H_DECIMAL_ADDR, &stSysSaveDataSection2[UNIT_BAR].AH2Decimal, 1);
					
					stSysAdjValSection2[UNIT_BAR].AH2 = (uint16_t)stSysSaveDataSection2[UNIT_BAR].AH2_H << 8 | stSysSaveDataSection2[UNIT_BAR].AH2_L;
					stSysAdjValSection2[UNIT_BAR].AH2Decimal = stSysSaveDataSection2[UNIT_BAR].AH2Decimal;
				}
			}
			// DL2
			if(bitMapHigh & _BIT5)
			{		
				if(unit == UNIT_PSI)
				{
				
				}
				else
				{
					stSysSaveDataSection2[UNIT_BAR].A2DL2_H = usartRevCmdBuffer[34];
					stSysSaveDataSection2[UNIT_BAR].A2DL2_L = usartRevCmdBuffer[35];
					stSysSaveDataSection2[UNIT_BAR].A2DL2Decimal = usartRevCmdBuffer[36];
					EEPROM_WriteBytes(eBAR_A2_DL2_ADDR, &stSysSaveDataSection2[UNIT_BAR].A2DL2_H, 2);
					EEPROM_WriteBytes(eBAR_A2_DL2_DECIMAL_ADDR, &stSysSaveDataSection2[UNIT_BAR].A2DL2Decimal, 1);
					
					stSysAdjValSection2[UNIT_BAR].A2DL2 = (uint16_t)stSysSaveDataSection2[UNIT_BAR].A2DL2_H << 8 | stSysSaveDataSection2[UNIT_BAR].A2DL2_L;
					stSysAdjValSection2[UNIT_BAR].A2DL2Decimal = stSysSaveDataSection2[UNIT_BAR].A2DL2Decimal;
				}
			}
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
