/**
  ******************************************************************************
  * @file    $ProjectName/User/eeprom/eeprom.h 
  * @author  WX
  * @version V1.0.0
  * @date    2020-7-9
  * @brief   eeprom.c file configuration.
  ******************************************************************************
  */

#ifndef EEPROM_H
#define EEPROM_H
	
/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "commonInclude.h"

/* Exported macro ------------------------------------------------------------*/
#define EEPROM_BASE_ADDR  																					(0x08080000)
#define EEPROM_BYTE_SIZE																						(0x0fff)

#define EEPROM_VERSION_CODE																			    (0x32)

/* Exported types ------------------------------------------------------------*/
enum
{
	eEEPROM_VERSION_ADDR = 0x00,
	
	eSENSOR_RANGE_UPPER_ADDR,
	eSENSOR_RANGE_UPPER_DECIMAL_ADDR = eSENSOR_RANGE_UPPER_ADDR + 2,
	eSENSOR_RANGE_LOWER_ADDR,
	eSENSOR_RANGE_LOWER_DECIMAL_ADDR = eSENSOR_RANGE_LOWER_ADDR + 2,
	eSENSOR_UNIT,
	eSENSOR_DAP,
	
	eSENSOR_P_UNIT_ADDR,
	eALM1_FUNC1_ADDR,
	eALM2_FUNC2_ADDR,
	
	// psi
	ePSI_P_H_ADDR,
	ePSI_P_H_DECIMAL_ADDR = ePSI_P_H_ADDR + 2,
	ePSI_P_L_ADDR,
	ePSI_P_L_DECIMAL_ADDR = ePSI_P_L_ADDR + 2,
	
	ePSI_A1_H_ADDR,
	ePSI_A1_H_DECIMAL_ADDR = ePSI_A1_H_ADDR + 2,
	ePSI_A1_L_ADDR,
	ePSI_A1_L_DECIMAL_ADDR = ePSI_A1_L_ADDR + 2,
	ePSI_A1_DL1_ADDR,
	ePSI_A1_DL1_DECIMAL_ADDR = ePSI_A1_DL1_ADDR + 2,
	
	ePSI_A2_H_ADDR,
	ePSI_A2_H_DECIMAL_ADDR = ePSI_A2_H_ADDR + 2,
	ePSI_A2_L_ADDR,
	ePSI_A2_L_DECIMAL_ADDR = ePSI_A2_L_ADDR + 2,
	ePSI_A2_DL2_ADDR,
	ePSI_A2_DL2_DECIMAL_ADDR = ePSI_A2_DL2_ADDR + 2,
	
	// bar
	eBAR_P_H_ADDR,
	eBAR_P_H_DECIMAL_ADDR = eBAR_P_H_ADDR + 2,
	eBAR_P_L_ADDR,
	eBAR_P_L_DECIMAL_ADDR = eBAR_P_L_ADDR + 2,
	
	eBAR_A1_H_ADDR,
	eBAR_A1_H_DECIMAL_ADDR = eBAR_A1_H_ADDR + 2,
	eBAR_A1_L_ADDR,
	eBAR_A1_L_DECIMAL_ADDR = eBAR_A1_L_ADDR + 2,
	eBAR_A1_DL1_ADDR,
	eBAR_A1_DL1_DECIMAL_ADDR = eBAR_A1_DL1_ADDR + 2,
	
	eBAR_A2_H_ADDR,
	eBAR_A2_H_DECIMAL_ADDR = eBAR_A2_H_ADDR + 2,
	eBAR_A2_L_ADDR,
	eBAR_A2_L_DECIMAL_ADDR = eBAR_A2_L_ADDR + 2,
	eBAR_A2_DL2_ADDR,
	eBAR_A2_DL2_DECIMAL_ADDR = eBAR_A2_DL2_ADDR + 2,
};

typedef enum
{
	UNIT_PSI = 0,
	UNIT_BAR,
}EnumValUnit;

typedef enum
{
	FUNC_LO = 0,
	FUNC_HI,
	FUNC_WIN1,
	FUNC_WIN2,
}EnumFuncMode;

// calculate data
typedef struct
{
	uint8_t eepromVer;
	uint16_t sensorRgeUpper;
	uint8_t sensorRgeUpperDecimal;
	uint16_t sensorRgeLower;
	uint8_t sensorRgeLowerDecimal;
	uint8_t sensorUnit;
	uint8_t dap;
	
	EnumValUnit eUnit;
	EnumFuncMode eFunc1;
	EnumFuncMode eFunc2;
}STSysAdjValSection1;

typedef struct
{
	uint16_t PH;
	uint8_t PHDecimal;
	uint16_t PL;
	uint8_t PLDecimal;
	
	uint16_t AH1;
	uint8_t AH1Decimal;
	uint16_t AL1;
	uint8_t AL1Decimal;
	uint16_t AlDL1;
	uint8_t A1DL1Decimal;
	
	uint16_t AH2;
	uint8_t AH2Decimal;
	uint16_t AL2;
	uint8_t AL2Decimal;
	uint16_t A2DL2;
	uint8_t A2DL2Decimal;
}STSysAdjValSection2;

// restore format
typedef struct
{
	uint8_t eepromVer;
	uint8_t sensorRgeUpper_H;
	uint8_t sensorRgeUpper_L;
	uint8_t sensorRgeUpperDecimal;
	uint8_t sensorRgeLower_H;
	uint8_t sensorRgeLower_L;
	uint8_t sensorRgeLowerDecimal;
	uint8_t sensorUnit;
	uint8_t dap;
	
	EnumValUnit eUnit;
	EnumFuncMode eFunc1;
	EnumFuncMode eFunc2;
}STSysParamsSection1;

typedef struct
{
	uint8_t PH_H;
	uint8_t PH_L;
	uint8_t PHDecimal;
	
	uint8_t PL_H;
	uint8_t PL_L;
	uint8_t PLDecimal;
	
	uint8_t AH1_H;
	uint8_t AH1_L;
	uint8_t AH1Decimal;
	uint8_t AL1_H;
	uint8_t AL1_L;
	uint8_t AL1Decimal;
	uint8_t AlDL1_H;
	uint8_t A1DL1_L;
	uint8_t A1DL1Decimal;
	
	uint8_t AH2_H;
	uint8_t AH2_L;
	uint8_t AH2Decimal;
	uint8_t AL2_H;
	uint8_t AL2_L;
	uint8_t AL2Decimal;
	uint8_t A2DL2_H;
	uint8_t A2DL2_L;
	uint8_t A2DL2Decimal;
}STSysParamsSection2;

/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void EEPROM_ReadBytes(uint32_t Address, uint8_t *Buffer, uint16_t Length);
void EEPROM_ReadWords(uint32_t Address, uint32_t *Buffer, uint16_t Length);
void EEPROM_WriteBytes(uint32_t Address, uint8_t *Buffer, uint16_t Length);
void EEPROM_PowerOnLoad(void);
void EEPROM_RestoreFactoryDefaultValue(void);

/* Exported variables --------------------------------------------------------*/
extern STSysParamsSection1 stSysSaveDataSection1;
extern STSysAdjValSection1 stSysAdjValSection1;
extern STSysParamsSection2 stSysSaveDataSection2[2];
extern STSysAdjValSection2 stSysAdjValSection2[2];

#endif // #ifndef EEPROM_H
