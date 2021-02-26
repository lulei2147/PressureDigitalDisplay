/**
  ******************************************************************************
  * @file    $ProjectName/User/74hc595/LG3641AH.c 
  * @author  WX
  * @version V1.0.0
  * @date    2020-7-9
  * @brief   LG3641AH function program
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "LG3641AH.h"
#include "adc.h"
#include "debug.h"
#include "user_menu.h"
#include "eeprom.h"
#include "math.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED_SEGMENT_DISPLAYS_REFRESH_TIME																	(50) // ms

#define PRESSURE_RANGE_UPPER																							(1.6f)
#define PRESSURE_RANGE_LOWER																							(0)

#define PRESSURE_VOL_ADC_UPPER															   						(3.05f)
#define PRESSURE_VOL_ADC_LOWER																						(0.05f)

#define GET_DISP_NEGATIVE_FLAG()																					(ledSegmentDisplayFlag & _BIT0)
#define SET_DISP_NEGATIVE_FLAG()																					(ledSegmentDisplayFlag |= _BIT0)
#define CLR_DISP_NEGATIVE_FLAG()																					(ledSegmentDisplayFlag &= ~_BIT0)

// Alarm flag
#define GET_AL1_ALARM_FLAG()																			    		(ledSegmentDisplayFlag & _BIT1)
#define SET_AL1_ALARM_FLAG()																	    				(ledSegmentDisplayFlag |= _BIT1)
#define CLR_AL1_ALARM_FLAG()																			    		(ledSegmentDisplayFlag &= ~_BIT1)

#define GET_ALARM_FUC_HI_FLAG()																	  				(ledSegmentDisplayFlag & _BIT2)
#define SET_ALARM_FUC_HI_FLAG()																				  	(ledSegmentDisplayFlag |= _BIT2)
#define CLR_ALARM_FUC_HI_FLAG()																		  			(ledSegmentDisplayFlag &= ~_BIT2)

#define GET_ALARM_FUC_WIN1_FLAG()																					(ledSegmentDisplayFlag & _BIT3)
#define SET_ALARM_FUC_WIN1_FLAG()																					(ledSegmentDisplayFlag |= _BIT3)
#define CLR_ALARM_FUC_WIN1_FLAG()																					(ledSegmentDisplayFlag &= ~_BIT3)

#define GET_ALARM_FUC_WIN2_FLAG()																					(ledSegmentDisplayFlag & _BIT4)
#define SET_ALARM_FUC_WIN2_FLAG()																					(ledSegmentDisplayFlag |= _BIT4)
#define CLR_ALARM_FUC_WIN2_FLAG()																					(ledSegmentDisplayFlag &= ~_BIT4)

#define LED_A1_ALARM																											(0x10)
#define LED_A2_ALARM																											(0x20)
#define LED_UNIT_BAR																											(0x40)
#define LED_UNIT_PSI																											(0x80)

#define BITSELE_AND_LED_STATUS(x)                                         (high8BitData[x] | ledAlarmFlag)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint8_t low8BitData[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
const uint8_t high8BitData[5] = {0x0E, 0x0D, 0x0B, 0x07, 0x00};

const uint8_t UserMenuDispPL[3] = {0x73, 0x40, 0x38};            // P-L
const uint8_t UserMenuDispPH[3] = {0x73, 0x40, 0x76};            // P-H
const uint8_t UserMenuDispUNIT[4] = {0x3E, 0x37, 0x10, 0x78};    // UNIT
const uint8_t UserMenuDispDAP[3] = {0x5E, 0x77, 0x73};           // DAP
const uint8_t UserMenuDispFUC1[4] = {0x71, 0x3E, 0x39, 0x06};    // FUC1
const uint8_t UserMenuDispAL1[3] = {0x77, 0x38, 0x06};           // AL1
const uint8_t UserMenuDispAH1[3] = {0x77, 0x76, 0x06};           // AH1
const uint8_t UserMenuDispDL1[3] = {0x5E, 0x38, 0x06};           // DL1
const uint8_t UserMenuDispFUC2[4] = {0x71, 0x3E, 0x39, 0x5B};    // FUC2
const uint8_t UserMenuDispAL2[3] = {0x77, 0x38, 0x5B};           // AL2
const uint8_t UserMenuDispAH2[3] = {0x77, 0x76, 0x5B};           // AH2
const uint8_t UserMenuDispDL2[3] = {0x5E, 0x38, 0x5B};           // DL2

// unit
const uint8_t UserMenuDispUnit_bar[3] = {0x7C, 0x77, 0x72};      // bar
const uint8_t UserMenuDispUnit_psi[3] = {0x73, 0x6D, 0x10};      // psi

// Func
const uint8_t UserMenuDispFunc_Hi[2] = {0x76, 0x10};             // Hi
const uint8_t UserMenuDispFunc_Lo[2] = {0x38, 0x5C};             // Lo
const uint8_t UserMenuDispFunc_w1[3] = {0x63, 0x06};             // W1
const uint8_t UserMenuDispFunc_w2[3] = {0x63, 0x5B};             // W2

uint8_t ledSegmentDisplayFlag = 0;
uint8_t extLedSegmentDisplayFlag = 0;

uint32_t SegDispTimeCounter = 0;

float pressureVal = 0;
char strBuffer[10] = {0};
uint8_t pointPos = 0;
float dapCalcPreBuffer[20] = {0};
uint8_t dapCalcPreBufIndex = 0;

float calculatedAL1Val = 0;
float calculatedAH1Val = 0;
float calculatedAL2Val = 0;
float calculatedAH2Val = 0;

// LED
uint8_t ledAlarmFlag = 0;

/* Private function prototypes -----------------------------------------------*/
void LED_SegmentDisplay_Bar(float preVal);
void LED_SegmentDisplay(uint8_t negativeFlag, float preVal);
void LED_SegmentDealDisplay(char *pStr, uint8_t posIdx);
void LED_SegmentUserMenuStringDisplay(void);
void LED_SegmentUserMenuParamsDisplay(char *pString);
void LED_SegmentDisplay_R2L(uint16_t data, uint8_t decimal);
static void DAP_DisplayVal(void);

//#define DEBUG
uint32_t printDebugTimer = 0;

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  LED Segment Displays handler.
	* @param  None
  * @retval None
  */
void LED_SegmentDisplay_Handler(void)
{
	// display menu
	if(GET_USER_MENU_DISPLAY_ID() != eMENU_DISPLAY_NONE)
	{
		LED_SegmentUserMenuStringDisplay();
	}
	else // display pressure
	{
		if(SegDispTimeCounter >= LED_SEGMENT_DISPLAYS_REFRESH_TIME)
		{
			float tempPreVal = 0;
			uint8_t i = 0;
			double preSum = 0;
			
			SegDispTimeCounter = 0;
			
			ADC_Vol =(float)(ADC_ConvertedValue & 0xffc) * 3.3f / 4095;
		
			// [0.05V, 3.3V] 
			if((ADC_Vol >= PRESSURE_VOL_ADC_LOWER) && (ADC_Vol <= 3.3f))
			{
				tempPreVal = PRESSURE_RANGE_UPPER / PRESSURE_VOL_ADC_UPPER * ADC_Vol;
				
				CLR_DISP_NEGATIVE_FLAG();
			}
			else if((ADC_Vol >= 0) && (ADC_Vol <= PRESSURE_VOL_ADC_LOWER)) // [0V, 0.05V]
			{
				tempPreVal = PRESSURE_RANGE_UPPER / PRESSURE_VOL_ADC_UPPER * (PRESSURE_VOL_ADC_LOWER - ADC_Vol);
				
				SET_DISP_NEGATIVE_FLAG();
			}
			else
			{
				// do nothing
			}
			
			// Calculate the average value based on the historical pressurel value
			dapCalcPreBufIndex %= stSysAdjValSection1.dap;
			dapCalcPreBuffer[dapCalcPreBufIndex] = tempPreVal;
			
			if(dapCalcPreBufIndex == (stSysAdjValSection1.dap - 1))
			{
				for(i = 0; i < stSysAdjValSection1.dap; i++)
				{
					preSum += dapCalcPreBuffer[i];
				}
			
				pressureVal = (float)(preSum / stSysAdjValSection1.dap);
			}
			
			dapCalcPreBufIndex++;
					
			// distinguish units and get the display string, units "bar" or "psi"
			switch(stSysAdjValSection1.eUnit)
			{
				case UNIT_BAR:
					if(GET_DISP_NEGATIVE_FLAG())
					{
						sprintf(strBuffer, "-%f", pressureVal * 10);
					}
					else
					{
						sprintf(strBuffer, "%f", pressureVal * 10);
						pointPos = (uint8_t)((strchr(strBuffer, '.') - strBuffer) / sizeof(char));
					}
					break;
				
				case UNIT_PSI:
					if(GET_DISP_NEGATIVE_FLAG())
					{
						sprintf(strBuffer, "-%f", pressureVal * 145.0f);
					}
					else
					{
						sprintf(strBuffer, "%f", pressureVal * 145.0f);
						pointPos = (uint8_t)((strchr(strBuffer, '.') - strBuffer) / sizeof(char));
					}
					break;
				
				default:
					break;
			}
		}
		
		LED_SegmentUserMenuParamsDisplay(strBuffer);
	}
}

/**
  * @brief  Alarm handling function.
	* @param  None
  * @retval None
  */
void AlarmHandler(void)
{
	float pressTempVal = 0;
	
	if(GET_CALCULATE_AL1_FLAG() | GET_CALCULATE_AH1_FLAG() | GET_CALCULATE_AL2_FLAG() | GET_CALCULATE_AH2_FLAG())
	{
		uint16_t val = 0;
		
		if(GET_CALCULATE_AL1_FLAG())
		{
			CLR_CALCULATE_AL1_FLAG();
			
			val = ((uint16_t)stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AL1_H << 8) | stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AL1_L;
			calculatedAL1Val = (float)val / pow(10, stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AL1Decimal);
		}
		else if(GET_CALCULATE_AH1_FLAG())
		{
			CLR_CALCULATE_AH1_FLAG();
			
			val = ((uint16_t)stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AH1_H << 8) | stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AH1_L;
			calculatedAH1Val = (float)val / pow(10, stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AH1Decimal);
		}
		else if(GET_CALCULATE_AL2_FLAG())
		{
			CLR_CALCULATE_AL2_FLAG();
			
			val = ((uint16_t)stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AL2_H << 8) | stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AL2_L;
			calculatedAL2Val = (float)val / pow(10, stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AL2Decimal);
		}
		else if(GET_CALCULATE_AH2_FLAG())
		{
			CLR_CALCULATE_AH2_FLAG();
			
			val = ((uint16_t)stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AH2_H << 8) | stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AH2_L;
			calculatedAH2Val = (float)val / pow(10, stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AH2Decimal);
		}
		else
		{
			// do nothing
		}
	}
		
	ledAlarmFlag = 0;
	
	if(stSysAdjValSection1.eUnit == UNIT_BAR)
	{
		ledAlarmFlag |= LED_UNIT_BAR;
		
		pressTempVal = pressureVal * 10;
	}
	else
	{
		ledAlarmFlag |= LED_UNIT_PSI;
		
		pressTempVal = pressureVal * 145;
	}
	
	// A1
	switch(stSysAdjValSection1.eFunc1)
	{
		case FUNC_LO:
			if(pressTempVal < calculatedAL1Val)
			{
				ledAlarmFlag |= LED_A1_ALARM;
			}
			break;
		
		case FUNC_HI:
			if(pressTempVal > calculatedAH1Val)
			{
				ledAlarmFlag |= LED_A1_ALARM;
			}
			break;
		
		case FUNC_WIN1:
			if((pressTempVal > calculatedAL1Val) && (pressTempVal < calculatedAH1Val))
			{
				ledAlarmFlag |= LED_A1_ALARM;
			}
			break;
		
		case FUNC_WIN2:
			if((pressTempVal < calculatedAL1Val) || (pressTempVal > calculatedAH1Val))
			{
				ledAlarmFlag |= LED_A1_ALARM;
			}
			break;
		
		default:
			break;
	}
	
	// A2
	switch(stSysAdjValSection1.eFunc2)
	{
		case FUNC_LO:
			if(pressTempVal < calculatedAL2Val)
			{
				ledAlarmFlag |= LED_A2_ALARM;
			}
			break;
		
		case FUNC_HI:
			if(pressTempVal > calculatedAH2Val)
			{
				ledAlarmFlag |= LED_A2_ALARM;
			}
			break;
		
		case FUNC_WIN1:
			if((pressTempVal > calculatedAL2Val) && (pressTempVal < calculatedAH2Val))
			{
				ledAlarmFlag |= LED_A2_ALARM;
			}
			break;
		
		case FUNC_WIN2:
			if((pressTempVal < calculatedAL2Val) || (pressTempVal > calculatedAH2Val))
			{
				ledAlarmFlag |= LED_A2_ALARM;
			}
			break;
		
		default:
			break;
	}
}

/**
  * @brief  User menu parameters display.
	* @param  pString: the string to be displayed.
  * @retval None
  */
void LED_SegmentUserMenuParamsDisplay(char *pString)
{
	uint8_t dispBuffer[4] = {0};

	if(strBuffer[0] == '-')
	{
		dispBuffer[0] = 0x40;            // '-'
		dispBuffer[1] = low8BitData[pString[1] - '0'] | 0x80;
		dispBuffer[2] = low8BitData[pString[2 + 1] - '0'];
		dispBuffer[3] = low8BitData[pString[3 + 1] - '0'];	
	}
	else
	{
		switch(pointPos)
		{
			case 1: // x.xxx
				dispBuffer[0] = low8BitData[pString[0] - '0'] | 0x80;
				dispBuffer[1] = low8BitData[pString[1 + 1] - '0'];
				dispBuffer[2] = low8BitData[pString[2 + 1] - '0'];
				//dispBuffer[3] = low8BitData[pString[3 + 1] - '0'];
				dispBuffer[3] = 0x00;
				break;
			
			case 2: // xx.xx
				dispBuffer[0] = low8BitData[pString[0] - '0'];
				dispBuffer[1] = low8BitData[pString[1] - '0'] | 0x80;
				dispBuffer[2] = low8BitData[pString[2 + 1] - '0'];
				dispBuffer[3] = low8BitData[pString[3 + 1] - '0'];
				break;
			
			case 3: // xxx.x
				dispBuffer[0] = low8BitData[pString[0] - '0'];
				dispBuffer[1] = low8BitData[pString[1] - '0'];
				dispBuffer[2] = low8BitData[pString[2] - '0'] | 0x80;
				dispBuffer[3] = low8BitData[pString[3 + 1] - '0'];
				break;
			
			case 4: // xxxx
				dispBuffer[0] = low8BitData[pString[0] - '0'];
				dispBuffer[1] = low8BitData[pString[1] - '0'];
				dispBuffer[2] = low8BitData[pString[2] - '0'];
				dispBuffer[3] = low8BitData[pString[3] - '0'];
				break;
			
			default:
				break;
		}
	}
	
	LED_SegmentDisplay_SendData(dispBuffer[0], BITSELE_AND_LED_STATUS(0));
	LED_SegmentDisplay_SendData(dispBuffer[1], BITSELE_AND_LED_STATUS(1));
	LED_SegmentDisplay_SendData(dispBuffer[2], BITSELE_AND_LED_STATUS(2));
	LED_SegmentDisplay_SendData(dispBuffer[3], BITSELE_AND_LED_STATUS(3));
	LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
}

/**
  * @brief  User menu string display.
  * @param  None
  * @retval None
  */
void LED_SegmentUserMenuStringDisplay(void)
{
	uint8_t i = 0;
	
	switch(GET_USER_MENU_DISPLAY_ID())
	{
		case eMENU_DISPLAY_P_L:
			for(i = 0; i < sizeof(UserMenuDispPL); i++)
			{
				LED_SegmentDisplay_SendData(UserMenuDispPL[i], BITSELE_AND_LED_STATUS(i));
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;
		
		case eMENU_DISPLAY_P_H:
			for(i = 0; i < sizeof(UserMenuDispPH); i++)
			{
				LED_SegmentDisplay_SendData(UserMenuDispPH[i], BITSELE_AND_LED_STATUS(i));
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;
		
		case eMENU_DISPLAY_UNIT:
			for(i = 0; i < sizeof(UserMenuDispUNIT); i++)
			{
				LED_SegmentDisplay_SendData(UserMenuDispUNIT[i], BITSELE_AND_LED_STATUS(i));
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;
		
		case eMENU_DISPLAY_DAP:
			for(i = 0; i < sizeof(UserMenuDispDAP); i++)
			{
				LED_SegmentDisplay_SendData(UserMenuDispDAP[i], BITSELE_AND_LED_STATUS(i));
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;
		
		case eMENU_DISPLAY_FUC1:
			for(i = 0; i < sizeof(UserMenuDispFUC1); i++)
			{
				LED_SegmentDisplay_SendData(UserMenuDispFUC1[i], BITSELE_AND_LED_STATUS(i));
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;
		
		case eMENU_DISPLAY_AL1:
			for(i = 0; i < sizeof(UserMenuDispAL1); i++)
			{
				LED_SegmentDisplay_SendData(UserMenuDispAL1[i], BITSELE_AND_LED_STATUS(i));
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;
		
		case eMENU_DISPLAY_AH1:
			for(i = 0; i < sizeof(UserMenuDispAH1); i++)
			{
				LED_SegmentDisplay_SendData(UserMenuDispAH1[i], BITSELE_AND_LED_STATUS(i));
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;
		
		case eMENU_DISPLAY_DL1:
			for(i = 0; i < sizeof(UserMenuDispDL1); i++)
			{
				LED_SegmentDisplay_SendData(UserMenuDispDL1[i], BITSELE_AND_LED_STATUS(i));
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;
			
		case eMENU_DISPLAY_FUC2:
			for(i = 0; i < sizeof(UserMenuDispFUC2); i++)
			{
				LED_SegmentDisplay_SendData(UserMenuDispFUC2[i], BITSELE_AND_LED_STATUS(i));
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;
		
		case eMENU_DISPLAY_AL2:
			for(i = 0; i < sizeof(UserMenuDispAL2); i++)
			{
				LED_SegmentDisplay_SendData(UserMenuDispAL2[i], BITSELE_AND_LED_STATUS(i));
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;
		
		case eMENU_DISPLAY_AH2:
			for(i = 0; i < sizeof(UserMenuDispAH2); i++)
			{
				LED_SegmentDisplay_SendData(UserMenuDispAH2[i], BITSELE_AND_LED_STATUS(i));
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;
		
		case eMENU_DISPLAY_DL2:
			for(i = 0; i < sizeof(UserMenuDispDL2); i++)
			{
				LED_SegmentDisplay_SendData(UserMenuDispDL2[i], BITSELE_AND_LED_STATUS(i));
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;
			
		case eMENU_DISPLAY_SUB_P_L:
			LED_SegmentDisplay_R2L(stSysAdjValSection2[stSysAdjValSection1.eUnit].PL, stSysAdjValSection2[stSysAdjValSection1.eUnit].PLDecimal);
			break;
		
		case eMENU_DISPLAY_SUB_P_H:
			LED_SegmentDisplay_R2L(stSysAdjValSection2[stSysAdjValSection1.eUnit].PH, stSysAdjValSection2[stSysAdjValSection1.eUnit].PHDecimal);
			break;
		
		case eMENU_DISPLAY_SUB_UNIT:
			if(stSysAdjValSection1.eUnit == UNIT_BAR)
			{
				for(i = 0; i < sizeof(UserMenuDispUnit_bar); i++)
				{
					LED_SegmentDisplay_SendData(UserMenuDispUnit_bar[i], BITSELE_AND_LED_STATUS(i));
				}
			}
			else
			{
				for(i = 0; i < sizeof(UserMenuDispUnit_psi); i++)
				{
					LED_SegmentDisplay_SendData(UserMenuDispUnit_psi[i], BITSELE_AND_LED_STATUS(i));
				}
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;
		
		case eMENU_DISPLAY_SUB_DAP:
			DAP_DisplayVal();
			break;
		
		case eMENU_DISPLAY_SUB_FUC1:
			switch(stSysAdjValSection1.eFunc1)
			{
				case FUNC_LO:
					for(i = 0; i < sizeof(UserMenuDispFunc_Lo); i++)
					{
						LED_SegmentDisplay_SendData(UserMenuDispFunc_Lo[i], BITSELE_AND_LED_STATUS(i));
					}
					break;
				
				case FUNC_HI:
					for(i = 0; i < sizeof(UserMenuDispFunc_Hi); i++)
					{
						LED_SegmentDisplay_SendData(UserMenuDispFunc_Hi[i], BITSELE_AND_LED_STATUS(i));
					}	
					break;
				
				case FUNC_WIN1:
					for(i = 0; i < sizeof(UserMenuDispFunc_w1); i++)
					{
						LED_SegmentDisplay_SendData(UserMenuDispFunc_w1[i], BITSELE_AND_LED_STATUS(i));
					}
					break;
				
				case FUNC_WIN2:
					for(i = 0; i < sizeof(UserMenuDispFunc_w2); i++)
					{
						LED_SegmentDisplay_SendData(UserMenuDispFunc_w2[i], BITSELE_AND_LED_STATUS(i));
					}
					break;
				
				default:
					break;
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;

		case eMENU_DISPLAY_SUB_AL1:
			LED_SegmentDisplay_R2L(stSysAdjValSection2[stSysAdjValSection1.eUnit].AL1, stSysAdjValSection2[stSysAdjValSection1.eUnit].AL1Decimal);
			break;
		
		case eMENU_DISPLAY_SUB_AH1:
			LED_SegmentDisplay_R2L(stSysAdjValSection2[stSysAdjValSection1.eUnit].AH1, stSysAdjValSection2[stSysAdjValSection1.eUnit].AH1Decimal);
			break;
		
		case eMENU_DISPLAY_SUB_DL1:
			LED_SegmentDisplay_R2L(stSysAdjValSection2[stSysAdjValSection1.eUnit].AlDL1, 0);
			break;
		
		case eMENU_DISPLAY_SUB_FUC2:
			switch(stSysAdjValSection1.eFunc2)
			{
				case FUNC_LO:
					for(i = 0; i < sizeof(UserMenuDispFunc_Lo); i++)
					{
						LED_SegmentDisplay_SendData(UserMenuDispFunc_Lo[i], BITSELE_AND_LED_STATUS(i));
					}
					break;
				
				case FUNC_HI:
					for(i = 0; i < sizeof(UserMenuDispFunc_Hi); i++)
					{
						LED_SegmentDisplay_SendData(UserMenuDispFunc_Hi[i], BITSELE_AND_LED_STATUS(i));
					}	
					break;
				
				case FUNC_WIN1:
					for(i = 0; i < sizeof(UserMenuDispFunc_w1); i++)
					{
						LED_SegmentDisplay_SendData(UserMenuDispFunc_w1[i], BITSELE_AND_LED_STATUS(i));
					}
					break;
				
				case FUNC_WIN2:
					for(i = 0; i < sizeof(UserMenuDispFunc_w2); i++)
					{
						LED_SegmentDisplay_SendData(UserMenuDispFunc_w2[i], BITSELE_AND_LED_STATUS(i));
					}
					break;
				
				default:
					break;
			}
			LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
			break;
		
		case eMENU_DISPLAY_SUB_AL2:
			LED_SegmentDisplay_R2L(stSysAdjValSection2[stSysAdjValSection1.eUnit].AL2, stSysAdjValSection2[stSysAdjValSection1.eUnit].AL2Decimal);
			break;
		
		case eMENU_DISPLAY_SUB_AH2:
			LED_SegmentDisplay_R2L(stSysAdjValSection2[stSysAdjValSection1.eUnit].AH2, stSysAdjValSection2[stSysAdjValSection1.eUnit].AH2Decimal);
			break;
		
		case eMENU_DISPLAY_SUB_DL2:
			LED_SegmentDisplay_R2L(stSysAdjValSection2[stSysAdjValSection1.eUnit].A2DL2, 0);
			break;
		
		default:
			break;
	}
}

/**
  * @brief  Nixie tube GPIO initial.
  * @param  None
  * @retval None
  */
void LED_SegmentDisplay_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = NXETBE_SDI_GPIO_PIN | NXETBE_SCK_GPIO_PIN | NXETBE_RCK_GPIO_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  The digital tube displays numbers from right to left.
	* @param  data: value to be display
	* @param  decimal: the number of non-zero decimal places to be displayed
  * @retval None
  */
void LED_SegmentDisplay_R2L(uint16_t data, uint8_t decimal)
{
	uint8_t dispBuffer[4] = {0};
	
	uint16_t val = data;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	if(decimal != 0)
	{
		uint8_t tenPoint = 0;
		uint8_t hundredPoint = 0;
		uint8_t thousandPoint = 0;
		
		switch(decimal)
		{
			case 1:
				tenPoint = 0x80;
				break;
			
			case 2:
				hundredPoint = 0x80;
				break;
			
			case 3:
				thousandPoint = 0x80;
				break;
			
			default:
				break;
		}
		
		// bit 0
		if(GET_CURSOR_POSITION() & _BIT0)
		{
			if(GET_BLINK_CURSOR_TIMEROUT())
			{
				dispBuffer[0] = low8BitData[num_thousand] | thousandPoint;
			}
			else
			{
				dispBuffer[0] = 0x00 | thousandPoint;
			}
			
			dispBuffer[1] = low8BitData[num_hundred] | hundredPoint;
			dispBuffer[2] = low8BitData[num_ten] | tenPoint;
			dispBuffer[3] = low8BitData[num_unit];
		}
		else if(GET_CURSOR_POSITION() & _BIT1) // bit1
		{
			if(GET_BLINK_CURSOR_TIMEROUT())
			{
				dispBuffer[1] = low8BitData[num_hundred] | hundredPoint;
			}
			else
			{
				dispBuffer[1] = 0x00 | hundredPoint;
			}
			
			dispBuffer[0] = low8BitData[num_thousand] | thousandPoint;
			dispBuffer[2] = low8BitData[num_ten] | tenPoint;
			dispBuffer[3] = low8BitData[num_unit];
		}
		else if(GET_CURSOR_POSITION() & _BIT2) // bit2
		{
			if(GET_BLINK_CURSOR_TIMEROUT())
			{
				dispBuffer[2] = low8BitData[num_ten] | tenPoint;
			}
			else
			{
				dispBuffer[2] = 0x00 | tenPoint;
			}
			
			dispBuffer[0] = low8BitData[num_thousand] | thousandPoint;
			dispBuffer[1] = low8BitData[num_hundred] | hundredPoint;
			dispBuffer[3] = low8BitData[num_unit];
		}
		else if(GET_CURSOR_POSITION() & _BIT3) // bit3
		{
			if(GET_BLINK_CURSOR_TIMEROUT())
			{
				dispBuffer[3] = low8BitData[num_unit];
			}
			else
			{
				dispBuffer[3] = 0x00;
			}
			
			dispBuffer[0] = low8BitData[num_thousand] | thousandPoint;
			dispBuffer[1] = low8BitData[num_hundred] | hundredPoint;
			dispBuffer[2] = low8BitData[num_ten] | tenPoint;
		}
		else if(GET_CURSOR_POSITION() & _BIT4)
		{
			switch(decimal)
			{
				case 1:
					if(GET_BLINK_CURSOR_TIMEROUT())
					{
						dispBuffer[2] = low8BitData[num_ten] | 0x80;
					}
					else
					{
						dispBuffer[2] = low8BitData[num_ten];
					}
				
					dispBuffer[0] = low8BitData[num_thousand];
					dispBuffer[1] = low8BitData[num_hundred];
					dispBuffer[3] = low8BitData[num_unit];
					break;
				
				case 2:
					if(GET_BLINK_CURSOR_TIMEROUT())
					{
						dispBuffer[1] = low8BitData[num_hundred] | 0x80;
					}
					else
					{
						dispBuffer[1] = low8BitData[num_hundred];
					}
					
					dispBuffer[0] = low8BitData[num_thousand];
					dispBuffer[2] = low8BitData[num_ten];
					dispBuffer[3] = low8BitData[num_unit];
					break;
				
				case 3:
					if(GET_BLINK_CURSOR_TIMEROUT())
					{
						dispBuffer[0] = low8BitData[num_thousand] | 0x80;
					}
					else
					{
						dispBuffer[0] = low8BitData[num_thousand];
					}
					
					dispBuffer[1] = low8BitData[num_hundred];
					dispBuffer[2] = low8BitData[num_ten];
					dispBuffer[3] = low8BitData[num_unit];
					break;
				
				default:
					break;
			}
		}
		else
		{
			// do nothing
		}
	}
	else
	{
		// bit 0
		if(GET_CURSOR_POSITION() & _BIT0)
		{
			if(GET_BLINK_CURSOR_TIMEROUT())
			{
				dispBuffer[0] = low8BitData[num_thousand];
			}
			else
			{
				dispBuffer[0] = 0x00;
			}
			
			dispBuffer[1] = low8BitData[num_hundred];
			dispBuffer[2] = low8BitData[num_ten];
			dispBuffer[3] = low8BitData[num_unit];
		}
		else if(GET_CURSOR_POSITION() & _BIT1) // bit1
		{
			if(GET_BLINK_CURSOR_TIMEROUT())
			{
				dispBuffer[1] = low8BitData[num_hundred];
			}
			else
			{
				dispBuffer[1] = 0x00;
			}
			
			dispBuffer[0] = low8BitData[num_thousand];
			dispBuffer[2] = low8BitData[num_ten];
			dispBuffer[3] = low8BitData[num_unit];
		}
		else if(GET_CURSOR_POSITION() & _BIT2) // bit2
		{
			if(GET_BLINK_CURSOR_TIMEROUT())
			{
				dispBuffer[2] = low8BitData[num_ten];
			}
			else
			{
				dispBuffer[2] = 0x00;
			}
			
			dispBuffer[0] = low8BitData[num_thousand];
			dispBuffer[1] = low8BitData[num_hundred];
			dispBuffer[3] = low8BitData[num_unit];
		}
		else if(GET_CURSOR_POSITION() & _BIT3) // bit3
		{
			if(GET_BLINK_CURSOR_TIMEROUT())
			{
				dispBuffer[3] = low8BitData[num_unit];
			}
			else
			{
				dispBuffer[3] = 0x00;
			}
			
			dispBuffer[0] = low8BitData[num_thousand];
			dispBuffer[1] = low8BitData[num_hundred];
			dispBuffer[2] = low8BitData[num_ten];
		}
		else if(GET_CURSOR_POSITION() & _BIT4) // bit4
		{
			dispBuffer[0] = low8BitData[num_thousand];
			dispBuffer[1] = low8BitData[num_hundred];
			dispBuffer[2] = low8BitData[num_ten];
			dispBuffer[3] = low8BitData[num_unit];
		}
		else
		{
			// do nothing
		}
	}
	
	LED_SegmentDisplay_SendData(dispBuffer[0], BITSELE_AND_LED_STATUS(0));
	LED_SegmentDisplay_SendData(dispBuffer[1], BITSELE_AND_LED_STATUS(1));
	LED_SegmentDisplay_SendData(dispBuffer[2], BITSELE_AND_LED_STATUS(2));
	LED_SegmentDisplay_SendData(dispBuffer[3], BITSELE_AND_LED_STATUS(3));
	LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
}

/**
  * @brief  Display DAP value.
  * @param  None
  * @retval None
  */
static void DAP_DisplayVal(void)
{
	uint8_t ten = 0;
	uint8_t unit = 0;
	uint8_t tempVal = 0;
	
	ten = stSysAdjValSection1.dap / 10;
	unit = stSysAdjValSection1.dap % 10;
	
	if(ten == 0)
	{
		tempVal = 0x00;
	}
	else
	{
		tempVal = low8BitData[ten];
	}
	
	LED_SegmentDisplay_SendData(tempVal, BITSELE_AND_LED_STATUS(2));
	LED_SegmentDisplay_SendData(low8BitData[unit], BITSELE_AND_LED_STATUS(3));
	LED_SegmentDisplay_SendData(0x00, high8BitData[4]);
}

/**
  * @brief  Send data to the LED Segment displays for displaying.
  * @param  None
  * @retval None
  */
void LED_SegmentDisplay_SendData(uint8_t data, uint8_t data0)
{
	uint8_t i = 0;
	
	CLR_NXETBE_RCK();
	
	for(i = 0; i < 8; i++)
	{
		CLR_NXETBE_SCK();
		
		if((data & 0x80) != 0)
		{
			SET_NXETBE_SDI();
		}
		else
		{
			CLR_NXETBE_SDI();
		}
		
		data <<= 1;
		
		SET_NXETBE_SCK();
	}
	
	for(i = 0; i < 8; i++)
	{
		CLR_NXETBE_SCK();
		
		if((data0 & 0x80) != 0)
		{
			SET_NXETBE_SDI();
		}
		else
		{
			CLR_NXETBE_SDI();
		}
		
		data0 <<= 1;
		
		SET_NXETBE_SCK();
	}
	
	SET_NXETBE_RCK();
}
