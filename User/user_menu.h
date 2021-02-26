/**
  ******************************************************************************
  * @file    $ProjectName/User/user_menu.h 
  * @author  WX
  * @version V1.0.0
  * @date    2020-7-9
  * @brief   user_menu.c file configuration.
  ******************************************************************************
  */

#ifndef USER_MENU_H
#define USER_MENU_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "commonInclude.h"
#include "eeprom.h"

/* Exported macro ------------------------------------------------------------*/
#define GET_USER_MENU_ID()																				(userMenuId)
#define SET_USER_MENU_ID(x)																				(userMenuId = (x))

#define GET_USER_MENU_DISPLAY_ID()																(userMenuDispId)
#define SET_USER_MENU_DISPLAY_ID(x)																(userMenuDispId = (x))

#define GET_CURSOR_POSITION()																			(cursorPos)
#define SET_CURSOR_POSITION() 																		(cursorPos = 0x01)

// flags
#define GET_BLINK_CURSOR_START_TIMER()														(userMenuFlag & _BIT0)
#define SET_BLINK_CURSOR_START_TIMER()														(userMenuFlag |= _BIT0)
#define CLR_BLINK_CURSOR_START_TIMER()														(userMenuFlag &= ~_BIT0)

#define GET_BLINK_CURSOR_TIMEROUT()																(userMenuFlag & _BIT1)
#define SET_BLINK_CURSOR_TIMEROUT()																(userMenuFlag |= _BIT1)
#define CLR_BLINK_CURSOR_TIMEROUT()																(userMenuFlag &= ~_BIT1)

/* Exported types ------------------------------------------------------------*/
enum
{
	eMENU_MAIN = 0x00,
	eMENU_P_L,
	eMENU_P_H,
	eMENU_UNIT,
	eMENU_DAP,
	eMENU_FUC1,
	eMENU_AL1,
	eMENU_AH1,
	eMENU_DL1,
	eMENU_FUC2,
	eMENU_AL2,
	eMENU_AH2,
	eMENU_DL2,
	
	eMENU_SUB_P_L,
	eMENU_SUB_P_H,
	eMENU_SUB_UNIT,
	eMENU_SUB_DAP,
	eMENU_SUB_FUC1,
	eMENU_SUB_AL1,
	eMENU_SUB_AH1,
	eMENU_SUB_DL1,
	eMENU_SUB_FUC2,
	eMENU_SUB_AL2,
	eMENU_SUB_AH2,
	eMENU_SUB_DL2,
	
	eMENU_SUM,
	eMENU_NONE = 0xff,
};

enum
{
	eMENU_DISPLAY_P_L = 0x01,
	eMENU_DISPLAY_P_H,
	eMENU_DISPLAY_UNIT,
	eMENU_DISPLAY_DAP,
	eMENU_DISPLAY_FUC1,
	eMENU_DISPLAY_AL1,
	eMENU_DISPLAY_AH1,
	eMENU_DISPLAY_DL1,
	eMENU_DISPLAY_FUC2,
	eMENU_DISPLAY_AL2,
	eMENU_DISPLAY_AH2,
	eMENU_DISPLAY_DL2,
	
	eMENU_DISPLAY_SUB_P_L,
	eMENU_DISPLAY_SUB_P_H,
	eMENU_DISPLAY_SUB_UNIT,
	eMENU_DISPLAY_SUB_DAP,
	eMENU_DISPLAY_SUB_FUC1,
	eMENU_DISPLAY_SUB_AL1,
	eMENU_DISPLAY_SUB_AH1,
	eMENU_DISPLAY_SUB_DL1,
	eMENU_DISPLAY_SUB_FUC2,
	eMENU_DISPLAY_SUB_AL2,
	eMENU_DISPLAY_SUB_AH2,
	eMENU_DISPLAY_SUB_DL2,
	
	eMENU_DISPLAY_SUM,
	eMENU_DISPLAY_NONE = 0xff,
};

/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void UserMenuCall(void);

/* Exported variables --------------------------------------------------------*/
extern uint8_t userMenuId;
extern uint8_t userMenuDispId;
extern uint8_t cursorPos;
extern uint8_t userMenuFlag;

#endif // #ifndef USER_MENU_H
