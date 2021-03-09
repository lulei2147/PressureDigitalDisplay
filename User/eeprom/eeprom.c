/**
  ******************************************************************************
  * @file    $ProjectName/User/eeprom/eeprom.c 
  * @author  WX
  * @version V1.0.0
  * @date    2020-7-9
  * @brief   Read or Write function program
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "eeprom.h"
#include "math.h"
#include "user_menu.h"
#include "LG3641AH.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// eeprom data(low limmit ~ DAP) -> stSysSaveDataSection1 -> stSysAdjValSection1
STSysParamsSection1 stSysSaveDataSection1;
STSysAdjValSection1 stSysAdjValSection1;
// eeprom data(P-L ~ DL2) -> stSysSaveDataSection2[2] -> stSysAdjValSection2[2]
STSysParamsSection2 stSysSaveDataSection2[2];
STSysAdjValSection2 stSysAdjValSection2[2];

/* Private function prototypes -----------------------------------------------*/
void EEPROM_LoadAdjParams(void);

/* Private functions ---------------------------------------------------------*/

extern STSysParamsSection1 stEepromSeg1DefaultVal;
extern STSysParamsSection2 stEepromSeg2DefaultVal;

/**
  * @brief  Read bytes data from EEPROM according to address.
  * @param  Address: specifies the address to be read.
	* @param  Buffer: storage space to save the read data.
	* @param  Length: data length.
  * @retval None
  */
void EEPROM_ReadBytes(uint32_t Address, uint8_t *Buffer, uint16_t Length)
{
	uint8_t *wAddr = NULL;
	
	if(Length >= EEPROM_BYTE_SIZE)
	{
		Length = EEPROM_BYTE_SIZE;
	}
	
	wAddr = (uint8_t *)(EEPROM_BASE_ADDR + Address);
	
	while(Length--)
	{
		*Buffer++ = *wAddr++;
	}
}

/**
  * @brief  Read words data from EEPROM according to address.
  * @param  Address: specifies the address to be read.
	* @param  Buffer: storage space to save the read data.
	* @param  Length: data length.
  * @retval None
  */
void EEPROM_ReadWords(uint32_t Address, uint32_t *Buffer, uint16_t Length)
{
	uint32_t *wAddr = NULL;
	
	if(Length >= EEPROM_BYTE_SIZE / 4)
	{
		Length = EEPROM_BYTE_SIZE / 4;
	}
	
	wAddr = (uint32_t *)(EEPROM_BASE_ADDR + Address);
	
	while(Length--)
	{
		*Buffer++ = *wAddr++;
	}
}

/**
  * @brief  Write words data to EEPROM according to address.
  * @param  Address: specifies the address to be written.
	* @param  Buffer: storage space to be written.
	* @param  Length: data length.
  * @retval None
  */
void EEPROM_WriteBytes(uint32_t Address, uint8_t *Buffer, uint16_t Length)
{
	uint32_t i = 0;
	uint8_t *pTemp = (uint8_t *)Buffer;
	uint32_t wAddr = Address + EEPROM_BASE_ADDR;
	uint8_t value = 0;
	
	if(Length >= EEPROM_BYTE_SIZE)
	{
		Length = EEPROM_BYTE_SIZE;
	}
	
	__disable_irq();
	DATA_EEPROM_Unlock();
	
	for(i = 0; i < Length; i++)
	{
		value = *(pTemp + i);
		//*(__IO uint8_t *)(wAddr + i) = value;
		DATA_EEPROM_FastProgramByte(wAddr + i, value);
		//DATA_EEPROM_EraseByte(wAddr);
	}

	
	DATA_EEPROM_Lock();
	__enable_irq();
}

/**
  * @brief  Erase specified area data.
  * @param  None
  * @retval None
  */
void EEPROM_EraseSpecifiedAreaBytes(uint32_t startAddress, uint16_t Length)
{
	uint16_t i = 0;
	uint32_t wAddr = startAddress + EEPROM_BASE_ADDR;
	
	__disable_irq();
	DATA_EEPROM_Unlock();
	
	for(i = 0; i < Length; i++)
	{
		DATA_EEPROM_ProgramByte(wAddr + i, 0xff);
	}
	
	DATA_EEPROM_Lock();
	__enable_irq();
}

/**
  * @brief  Restore factory parameters.
  * @param  None
  * @retval None
  */
void EEPROM_RestoreFactoryDefaultValue(void)
{
	EEPROM_EraseSpecifiedAreaBytes(eEEPROM_VERSION_ADDR, 0xff);
	EEPROM_WriteBytes(eEEPROM_VERSION_ADDR, (uint8_t *)&stEepromSeg1DefaultVal, sizeof(stEepromSeg1DefaultVal));
	EEPROM_WriteBytes(ePSI_P_H_ADDR, (uint8_t *)&stEepromSeg2DefaultVal, 2 * sizeof(stEepromSeg2DefaultVal));
	
	memcpy((uint8_t *)&stSysSaveDataSection1, (uint8_t *)&stEepromSeg1DefaultVal, sizeof(stEepromSeg1DefaultVal));
	memcpy((uint8_t *)stSysSaveDataSection2, (uint8_t *)&stEepromSeg2DefaultVal, 2 * sizeof(stEepromSeg2DefaultVal));
	
	// Load the parameters needed by the system
	EEPROM_LoadAdjParams();
}

/**
  * @brief  Power on, load eeprom value.
  * @param  None
  * @retval None
  */
void EEPROM_PowerOnLoad(void)
{
	uint8_t eepromVer = 0;
	
	EEPROM_ReadBytes(eEEPROM_VERSION_ADDR, &eepromVer, 1);
	
	if(eepromVer != EEPROM_VERSION_CODE)
	{
		EEPROM_EraseSpecifiedAreaBytes(eEEPROM_VERSION_ADDR, 0xFF);
		EEPROM_WriteBytes(eEEPROM_VERSION_ADDR, (uint8_t *)&stEepromSeg1DefaultVal, sizeof(stEepromSeg1DefaultVal));
		EEPROM_WriteBytes(ePSI_P_H_ADDR, (uint8_t *)&stEepromSeg2DefaultVal, 2 * sizeof(stEepromSeg2DefaultVal));
		
		memcpy((uint8_t *)&stSysSaveDataSection1, (uint8_t *)&stEepromSeg1DefaultVal, sizeof(stEepromSeg1DefaultVal));
		memcpy((uint8_t *)stSysSaveDataSection2, (uint8_t *)&stEepromSeg2DefaultVal, 2 * sizeof(stEepromSeg2DefaultVal));
	}
	else
	{
		EEPROM_ReadBytes(eEEPROM_VERSION_ADDR, (uint8_t *)&stSysSaveDataSection1, sizeof(stSysSaveDataSection1));
		EEPROM_ReadBytes(ePSI_P_H_ADDR, (uint8_t *)stSysSaveDataSection2, sizeof(stSysSaveDataSection2));
	}
	
	// Load the parameters needed by the system
	EEPROM_LoadAdjParams();
	
	// flag init
	SET_CALCULATE_AL1_FLAG();
	SET_CALCULATE_AH1_FLAG();
	SET_CALCULATE_AL2_FLAG();
	SET_CALCULATE_AH2_FLAG();
}

// Load the parameters needed by the system
void EEPROM_LoadAdjParams(void)
{
	// unit
	stSysAdjValSection1.eUnit = stSysSaveDataSection1.eUnit;

	// DAP
	stSysAdjValSection1.dap = stSysSaveDataSection1.dap;
	
	// fuc
	stSysAdjValSection1.eFunc1 = stSysSaveDataSection1.eFunc1;
	stSysAdjValSection1.eFunc2 = stSysSaveDataSection1.eFunc2;
	
	// bar
	// upper limit
	stSysAdjValSection1.stSensorAdjVal[UNIT_BAR].sensorRgeUpper = (uint16_t)stSysSaveDataSection1.stSensorRange[UNIT_BAR].sensorRgeUpper_H << 8 | stSysSaveDataSection1.stSensorRange[UNIT_BAR].sensorRgeUpper_L;
	stSysAdjValSection1.stSensorAdjVal[UNIT_BAR].sensorRgeUpperDecimal = stSysSaveDataSection1.stSensorRange[UNIT_BAR].sensorRgeUpperDecimal;
	// low limit
	stSysAdjValSection1.stSensorAdjVal[UNIT_BAR].sensorRgeLower = (uint16_t)stSysSaveDataSection1.stSensorRange[UNIT_BAR].sensorRgeLower_H << 8 | stSysSaveDataSection1.stSensorRange[UNIT_BAR].sensorRgeLower_L;
	stSysAdjValSection1.stSensorAdjVal[UNIT_BAR].sensorRgeLowerDecimal = stSysSaveDataSection1.stSensorRange[UNIT_BAR].sensorRgeLowerDecimal;
	
	// P-L
	stSysAdjValSection2[UNIT_BAR].PL = (uint16_t)stSysSaveDataSection2[UNIT_BAR].PL_H << 8 | stSysSaveDataSection2[UNIT_BAR].PL_L;
	stSysAdjValSection2[UNIT_BAR].PLDecimal = stSysSaveDataSection2[UNIT_BAR].PLDecimal;
	
	// P-H
	stSysAdjValSection2[UNIT_BAR].PH = (uint16_t)stSysSaveDataSection2[UNIT_BAR].PH_H << 8 | stSysSaveDataSection2[UNIT_BAR].PH_L;
	stSysAdjValSection2[UNIT_BAR].PHDecimal = stSysSaveDataSection2[UNIT_BAR].PHDecimal;
	
	// AL1
	stSysAdjValSection2[UNIT_BAR].AL1 = (uint16_t)stSysSaveDataSection2[UNIT_BAR].AL1_H << 8 | stSysSaveDataSection2[UNIT_BAR].AL1_L;
	stSysAdjValSection2[UNIT_BAR].AL1Decimal = stSysSaveDataSection2[UNIT_BAR].AL1Decimal;
	
	// AH1
	stSysAdjValSection2[UNIT_BAR].AH1 = (uint16_t)stSysSaveDataSection2[UNIT_BAR].AH1_H << 8 | stSysSaveDataSection2[UNIT_BAR].AH1_L;
	stSysAdjValSection2[UNIT_BAR].AH1Decimal = stSysSaveDataSection2[UNIT_BAR].AH1Decimal;
	
	// DL1
	stSysAdjValSection2[UNIT_BAR].AlDL1 = (uint16_t)stSysSaveDataSection2[UNIT_BAR].AlDL1_H << 8 | stSysSaveDataSection2[UNIT_BAR].A1DL1_L;
	stSysAdjValSection2[UNIT_BAR].A1DL1Decimal = stSysSaveDataSection2[UNIT_BAR].A1DL1Decimal;
	
	// AL2
	stSysAdjValSection2[UNIT_BAR].AL2 = (uint16_t)stSysSaveDataSection2[UNIT_BAR].AL2_H << 8 | stSysSaveDataSection2[UNIT_BAR].AL2_L;
	stSysAdjValSection2[UNIT_BAR].AL2Decimal = stSysSaveDataSection2[UNIT_BAR].AL2Decimal;
	
	// AH2
	stSysAdjValSection2[UNIT_BAR].AH2 = (uint16_t)stSysSaveDataSection2[UNIT_BAR].AH2_H << 8 | stSysSaveDataSection2[UNIT_BAR].AH2_L;
	stSysAdjValSection2[UNIT_BAR].AH2Decimal = stSysSaveDataSection2[UNIT_BAR].AH2Decimal;
	
	// DL2
	stSysAdjValSection2[UNIT_BAR].A2DL2 = (uint16_t)stSysSaveDataSection2[UNIT_BAR].A2DL2_H << 8 | stSysSaveDataSection2[UNIT_BAR].A2DL2_L;
	stSysAdjValSection2[UNIT_BAR].A2DL2Decimal = stSysSaveDataSection2[UNIT_BAR].A2DL2Decimal;
	
	// psi
	// upper limit
	stSysAdjValSection1.stSensorAdjVal[UNIT_PSI].sensorRgeUpper = (uint16_t)stSysSaveDataSection1.stSensorRange[UNIT_PSI].sensorRgeUpper_H << 8 | stSysSaveDataSection1.stSensorRange[UNIT_PSI].sensorRgeUpper_L;
	stSysAdjValSection1.stSensorAdjVal[UNIT_PSI].sensorRgeUpperDecimal = stSysSaveDataSection1.stSensorRange[UNIT_PSI].sensorRgeUpperDecimal;
	// low limit
	stSysAdjValSection1.stSensorAdjVal[UNIT_PSI].sensorRgeLower = (uint16_t)stSysSaveDataSection1.stSensorRange[UNIT_PSI].sensorRgeLower_H << 8 | stSysSaveDataSection1.stSensorRange[UNIT_PSI].sensorRgeLower_L;
	stSysAdjValSection1.stSensorAdjVal[UNIT_PSI].sensorRgeLowerDecimal = stSysSaveDataSection1.stSensorRange[UNIT_PSI].sensorRgeLowerDecimal;
	
	// P-L
	stSysAdjValSection2[UNIT_PSI].PL = (uint16_t)stSysSaveDataSection2[UNIT_PSI].PL_H << 8 | stSysSaveDataSection2[UNIT_PSI].PL_L;
	stSysAdjValSection2[UNIT_PSI].PLDecimal = stSysSaveDataSection2[UNIT_PSI].PLDecimal;
	
	// P-H
	stSysAdjValSection2[UNIT_PSI].PH = (uint16_t)stSysSaveDataSection2[UNIT_PSI].PH_H << 8 | stSysSaveDataSection2[UNIT_PSI].PH_L;
	stSysAdjValSection2[UNIT_PSI].PHDecimal = stSysSaveDataSection2[UNIT_PSI].PHDecimal;
	
	// AL1
	stSysAdjValSection2[UNIT_PSI].AL1 = (uint16_t)stSysSaveDataSection2[UNIT_PSI].AL1_H << 8 | stSysSaveDataSection2[UNIT_PSI].AL1_L;
	stSysAdjValSection2[UNIT_PSI].AL1Decimal = stSysSaveDataSection2[UNIT_PSI].AL1Decimal;
	
	// AH1
	stSysAdjValSection2[UNIT_PSI].AH1 = (uint16_t)stSysSaveDataSection2[UNIT_PSI].AH1_H << 8 | stSysSaveDataSection2[UNIT_PSI].AH1_L;
	stSysAdjValSection2[UNIT_PSI].AH1Decimal = stSysSaveDataSection2[UNIT_PSI].AH1Decimal;
	
	// DL1
	stSysAdjValSection2[UNIT_PSI].AlDL1 = (uint16_t)stSysSaveDataSection2[UNIT_PSI].AlDL1_H << 8 | stSysSaveDataSection2[UNIT_PSI].A1DL1_L;
	stSysAdjValSection2[UNIT_PSI].A1DL1Decimal = stSysSaveDataSection2[UNIT_PSI].A1DL1Decimal;
	
	// AL2
	stSysAdjValSection2[UNIT_PSI].AL2 = (uint16_t)stSysSaveDataSection2[UNIT_PSI].AL2_H << 8 | stSysSaveDataSection2[UNIT_PSI].AL2_L;
	stSysAdjValSection2[UNIT_PSI].AL2Decimal = stSysSaveDataSection2[UNIT_PSI].AL2Decimal;
	
	// AH2
	stSysAdjValSection2[UNIT_PSI].AH2 = (uint16_t)stSysSaveDataSection2[UNIT_PSI].AH2_H << 8 | stSysSaveDataSection2[UNIT_PSI].AH2_L;
	stSysAdjValSection2[UNIT_PSI].AH2Decimal = stSysSaveDataSection2[UNIT_PSI].AH2Decimal;
	
	// DL2
	stSysAdjValSection2[UNIT_PSI].A2DL2 = (uint16_t)stSysSaveDataSection2[UNIT_PSI].A2DL2_H << 8 | stSysSaveDataSection2[UNIT_PSI].A2DL2_L;
	stSysAdjValSection2[UNIT_PSI].A2DL2Decimal = stSysSaveDataSection2[UNIT_PSI].A2DL2Decimal;
}

