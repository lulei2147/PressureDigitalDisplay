/**
  ******************************************************************************
  * @file    $ProjectName/eeprom/eepromDefault.c 
  * @author  WX
  * @version V1.0.0
  * @date    2020-7-9
  * @brief   eeprom default value.
  ******************************************************************************
  */

//**********************
// |_| |_| |_| |_|
// | |.| |.| |.| |.
//    3   2   1   0       
//**********************


#include "eeprom.h"

STSysParamsSection1 stEepromSeg1DefaultVal = {
	EEPROM_VERSION_CODE, // EEPROM Ver
	
	0x06,	// Sensor Range Upper H
	0x40,	// Sensor Range Upper L
	0x02, // Sensor Range Upper decimal
	0x00, // Sensor Range Lower H
	0x00, // Sensor Range Lower L
	0x03, // Sensor Range Lower decimal
	0x00, // Sensor Unit
	0x08, // DAP
	
	UNIT_BAR, // Unit (0:psi, 1:bar)
	FUNC_WIN1, // Func1
	FUNC_WIN1, // Func2
};

STSysParamsSection2 stEepromSeg2DefaultVal[2] =
{
	// psi
	0x09, // P-H_H
	0x10, // P-H_L
	0x00, // Decimal places
	0x00, // P-L_H
	0x00, // P-L_L
	0x03, // Decimal places
	
	0x06, // AH1_H
	0xCC, // AH1_L
	0x00, // Decimal places
	0x05, // AL1_H
	0xAA, // AL1_L
	0x00, // Decimal places
	0x00, // DL1_H
	0x00, // DL1_L
	0x03, // Decimal places
	
	0x06, // AH2_H
	0xCC, // AH2_L
	0x00, // Decimal places
	0x05, // AL2_H
	0xAA, // AL2_L
	0x00, // Decimal places
	0x00, // DL2_H
	0x00, // DL2_L
	0x03, // Decimal places
	
	// bar
	0x06, // P-H_H
	0x40, // P-H_L
	0x02, // Decimal places
	0x00, // P-L_H
	0x00, // P-L_L
	0x03, // Decimal places
	
	0x04, // AH1_H
	0xB0, // AH1_L
	0x02, // Decimal places
	0x03, // AL1_H
	0xE8, // AL1_L
	0x02, // Decimal places
	0x00, // DL1_H
	0x00, // DL1_L
	0x03, // Decimal places
	
	0x04, // AH2_H
	0xB0, // AH2_L
	0x02, // Decimal places
	0x03, // AL2_H
	0xE8, // AL2_L
	0x02, // Decimal places
	0x00, // DL2_H
	0x00, // DL2_L
	0x03, // Decimal places
};
