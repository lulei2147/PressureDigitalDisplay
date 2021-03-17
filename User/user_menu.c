/**
  ******************************************************************************
  * @file    $ProjectName/User/user_menu.c 
  * @author  WX
  * @version V1.0.0
  * @date    2020-7-9
  * @brief   Menu function program
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "user_menu.h"
#include "key.h"
#include "LG3641AH.h"
#include "eeprom.h"
#include "math.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DAP_ADJ_VAL_MIN                                            (3)
#define DAP_ADJ_VAL_MAX                                            (10)

typedef void (*pMenuFuncArray)(void);

/* Private function prototypes -----------------------------------------------*/
void UserMenuCall(void);

// eMENU_MAIN
static void UserMenuMainUp(void);
static void UserMenuMainDown(void);
static void UserMenuMainM(void);
static void UserMenuMainS(void);
// eMENU_P_L
static void UserMenuPLUp(void);
static void UserMenuPLDown(void);
static void UserMenuPLM(void);
static void UserMenuPLS(void);
// eMENU_P_H
static void UserMenuPHUp(void);
static void UserMenuPHDown(void);
static void UserMenuPHM(void);
static void UserMenuPHS(void);
// eMENU_UNIT
static void UserMenuUnitUp(void);
static void UserMenuUnitDown(void);
static void UserMenuUnitM(void);
static void UserMenuUnitS(void);
// eMENU_DAP
static void UserMenuDapUp(void);
static void UserMenuDapDown(void);
static void UserMenuDapM(void);
static void UserMenuDapS(void);
// eMENU_FUC1
static void UserMenuFuc1Up(void);
static void UserMenuFuc1Down(void);
static void UserMenuFuc1M(void);
static void UserMenuFuc1S(void);
// eMENU_AL1
static void UserMenuAL1Up(void);
static void UserMenuAL1Down(void);
static void UserMenuAL1M(void);
static void UserMenuAL1S(void);
// eMENU_AH1
static void UserMenuAH1Up(void);
static void UserMenuAH1Down(void);
static void UserMenuAH1M(void);
static void UserMenuAH1S(void);
// eMENU_DL1
static void UserMenuDL1Up(void);
static void UserMenuDL1Down(void);
static void UserMenuDL1M(void);
static void UserMenuDL1S(void);
// eMENU_FUC2
static void UserMenuFuc2Up(void);
static void UserMenuFuc2Down(void);
static void UserMenuFuc2M(void);
static void UserMenuFuc2S(void);
// eMENU_AL2
static void UserMenuAL2Up(void);
static void UserMenuAL2Down(void);
static void UserMenuAL2M(void);
static void UserMenuAL2S(void);
// eMENU_AH2
static void UserMenuAH2Up(void);
static void UserMenuAH2Down(void);
static void UserMenuAH2M(void);
static void UserMenuAH2S(void);
// eMENU_DL2
static void UserMenuDL2Up(void);
static void UserMenuDL2Down(void);
static void UserMenuDL2M(void);
static void UserMenuDL2S(void);
// eMENU_SUB_P_L
static void UserMenuSubPLUp(void);
static void UserMenuSubPLDown(void);
static void UserMenuSubPLM(void);
static void UserMenuSubPLS(void);
// eMENU_SUB_P_H
static void UserMenuSubPHUp(void);
static void UserMenuSubPHDown(void);
static void UserMenuSubPHM(void);
static void UserMenuSubPHS(void);
// eMENU_DISPLAY_SUB_UNIT
static void UserMenuSubUnitUp(void);
static void UserMenuSubUnitDown(void);
static void UserMenuSubUnitM(void);
static void UserMenuSubUnitS(void);
// eMENU_DISPLAY_SUB_DAP
static void UserMenuSubDapUp(void);
static void UserMenuSubDapDown(void);
static void UserMenuSubDapM(void);
static void UserMenuSubDapS(void);
// eMENU_DISPLAY_SUB_FUC1
static void UserMenuSubFunc1Up(void);
static void UserMenuSubFunc1Down(void);
static void UserMenuSubFunc1M(void);
static void UserMenuSubFunc1S(void);
// eMENU_DISPLAY_SUB_AL1
static void UserMenuSubAL1Up(void);
static void UserMenuSubAL1Down(void);
static void UserMenuSubAL1M(void);
static void UserMenuSubAL1S(void);
// eMENU_DISPLAY_SUB_AH1
static void UserMenuSubAH1Up(void);
static void UserMenuSubAH1Down(void);
static void UserMenuSubAH1M(void);
static void UserMenuSubAH1S(void);
// eMENU_DISPLAY_SUB_DL1
static void UserMenuSubDL1Up(void);
static void UserMenuSubDL1Down(void);
static void UserMenuSubDL1M(void);
static void UserMenuSubDL1S(void);
// eMENU_DISPLAY_SUB_FUC2
static void UserMenuSubFuc2Up(void);
static void UserMenuSubFuc2Down(void);
static void UserMenuSubFuc2M(void);
static void UserMenuSubFuc2S(void);
// eMENU_DISPLAY_SUB_AL2
static void UserMenuSubAL2Up(void);
static void UserMenuSubAL2Down(void);
static void UserMenuSubAL2M(void);
static void UserMenuSubAL2S(void);
// eMENU_DISPLAY_SUB_AH2
static void UserMenuSubAH2Up(void);
static void UserMenuSubAH2Down(void);
static void UserMenuSubAH2M(void);
static void UserMenuSubAH2S(void);
// eMENU_DISPLAY_SUB_DL2
static void UserMenuSubDL2Up(void);
static void UserMenuSubDL2Down(void);
static void UserMenuSubDL2M(void);
static void UserMenuSubDL2S(void);

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pMenuFuncArray callMenu[eMENU_SUM][4] = {
// UP                    DOWM                     M                         S
{UserMenuMainUp,         UserMenuMainDown,        UserMenuMainM,            UserMenuMainS}, // eMENU_MAIN
{UserMenuPLUp,           UserMenuPLDown,          UserMenuPLM,              UserMenuPLS}, // eMENU_P_L
{UserMenuPHUp,           UserMenuPHDown,          UserMenuPHM,              UserMenuPHS}, // eMENU_P_H
{UserMenuUnitUp,         UserMenuUnitDown,        UserMenuUnitM,            UserMenuUnitS}, // eMENU_UNIT
{UserMenuDapUp,          UserMenuDapDown,         UserMenuDapM,             UserMenuDapS}, // eMENU_DAP
{UserMenuFuc1Up,         UserMenuFuc1Down,        UserMenuFuc1M,            UserMenuFuc1S}, // eMENU_FUC1
{UserMenuAL1Up,          UserMenuAL1Down,         UserMenuAL1M,             UserMenuAL1S}, // eMENU_AL1
{UserMenuAH1Up,          UserMenuAH1Down,         UserMenuAH1M,             UserMenuAH1S}, // eMENU_AH1
{UserMenuDL1Up,          UserMenuDL1Down,         UserMenuDL1M,             UserMenuDL1S}, // eMENU_DL1
{UserMenuFuc2Up,         UserMenuFuc2Down,        UserMenuFuc2M,            UserMenuFuc2S}, // eMENU_FUC2
{UserMenuAL2Up,          UserMenuAL2Down,         UserMenuAL2M,             UserMenuAL2S}, // eMENU_AL2
{UserMenuAH2Up,          UserMenuAH2Down,         UserMenuAH2M,             UserMenuAH2S}, // eMENU_AH2
{UserMenuDL2Up,          UserMenuDL2Down,         UserMenuDL2M,             UserMenuDL2S}, // eMENU_DL2

{UserMenuSubPLUp,        UserMenuSubPLDown,       UserMenuSubPLM,           UserMenuSubPLS},	// eMENU_SUB_P_L
{UserMenuSubPHUp,        UserMenuSubPHDown,       UserMenuSubPHM,           UserMenuSubPHS},	// eMENU_SUB_P_H
{UserMenuSubUnitUp,      UserMenuSubUnitDown,     UserMenuSubUnitM,         UserMenuSubUnitS},	// eMENU_SUB_UNIT
{UserMenuSubDapUp,       UserMenuSubDapDown,      UserMenuSubDapM,          UserMenuSubDapS},	// eMENU_SUB_DAP
{UserMenuSubFunc1Up,     UserMenuSubFunc1Down,    UserMenuSubFunc1M,        UserMenuSubFunc1S},	// eMENU_SUB_FUC1
{UserMenuSubAL1Up,       UserMenuSubAL1Down,      UserMenuSubAL1M,          UserMenuSubAL1S},	// eMENU_SUB_AL1
{UserMenuSubAH1Up,       UserMenuSubAH1Down,      UserMenuSubAH1M,          UserMenuSubAH1S}, // eMENU_DISPLAY_SUB_AH1
{UserMenuSubDL1Up,       UserMenuSubDL1Down,      UserMenuSubDL1M,          UserMenuSubDL1S}, // eMENU_DISPLAY_SUB_DL1
{UserMenuSubFuc2Up,      UserMenuSubFuc2Down,     UserMenuSubFuc2M,         UserMenuSubFuc2S}, // eMENU_DISPLAY_SUB_FUC2
{UserMenuSubAL2Up,       UserMenuSubAL2Down,      UserMenuSubAL2M,          UserMenuSubAL2S}, // eMENU_DISPLAY_SUB_AL2
{UserMenuSubAH2Up,       UserMenuSubAH2Down,      UserMenuSubAH2M,          UserMenuSubAH2S}, // eMENU_DISPLAY_SUB_AH2
{UserMenuSubDL2Up,       UserMenuSubDL2Down,      UserMenuSubDL2M,          UserMenuSubDL2S} // eMENU_DISPLAY_SUB_DL2
};

uint8_t userMenuFlag = 0;
uint8_t userMenuId = eMENU_NONE;
uint8_t userMenuDispId = eMENU_DISPLAY_NONE;
uint8_t cursorPos = 0;

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Call User Menu.
  * @param  None
  * @retval None
  */
void UserMenuCall(void)
{
	if(GET_KEY_MESSAGE() != eKEY_MESSAGE_NONE)
	{
		if((GET_USER_MENU_ID() < eMENU_SUM) && (GET_KEY_MESSAGE() < eKEY_MESSAGE_SUM))
		{
			(callMenu[GET_USER_MENU_ID()][GET_KEY_MESSAGE()])();
		}
		
		SET_KEY_MESSAGE(eKEY_MESSAGE_NONE);
	}
}

/**
  * @brief  Right Move cursor position.
  * @param  None
  * @retval None
  */
void UserMenuRightMoveCursorPos(void)
{
	if(cursorPos & _BIT4)
	{
		cursorPos = 0x01;
	}
	else
	{
		cursorPos <<= 1;
	}
}

/**
  * @brief  Left Move cursor position.
  * @param  None
  * @retval None
  */
void UserMenuLeftMoveCursorPos(void)
{
	if(cursorPos & _BIT0)
	{
		cursorPos = 0x10;
	}
	else
	{
		cursorPos >>= 1;
	}
}

/**
  * @brief  Under the Main menu (eMENU_MAIN), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuMainUp(void)
{

}

/**
  * @brief  Under the Main menu (eMENU_MAIN), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuMainDown(void)
{
	
}

/**
  * @brief  Under the Main menu (eMENU_MAIN), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuMainM(void)
{
	SET_USER_MENU_ID(eMENU_P_L);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_P_L);
}

/**
  * @brief  Under the Main menu (eMENU_MAIN), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuMainS(void)
{

}

/**
  * @brief  Under the P-L menu (eMENU_P_L), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuPLUp(void)
{
	SET_USER_MENU_ID(eMENU_DL2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_DL2);
}

/**
  * @brief  Under the P-L menu (eMENU_P_L), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuPLDown(void)
{
	SET_USER_MENU_ID(eMENU_P_H);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_P_H);
}

/**
  * @brief  Under the P-L menu (eMENU_P_L), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuPLM(void)
{
	SET_USER_MENU_ID(eMENU_SUB_P_L);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_SUB_P_L);
	
	SET_BLINK_CURSOR_START_TIMER();
	SET_CURSOR_POSITION();
}

/**
  * @brief  Under the P-L menu (eMENU_P_L), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuPLS(void)
{
	SET_USER_MENU_ID(eMENU_MAIN);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_NONE);
}

/**
  * @brief  Under the P-H menu (eMENU_P_H), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuPHUp(void)
{
	SET_USER_MENU_ID(eMENU_P_L);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_P_L);
}

/**
  * @brief  Under the P-H menu (eMENU_P_H), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuPHDown(void)
{
	SET_USER_MENU_ID(eMENU_UNIT);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_UNIT);
}

/**
  * @brief  Under the P-H menu (eMENU_P_H), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuPHM(void)
{
	SET_USER_MENU_ID(eMENU_SUB_P_H);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_SUB_P_H);
	
	SET_BLINK_CURSOR_START_TIMER();
	SET_CURSOR_POSITION();
}

/**
  * @brief  Under the P-H menu (eMENU_P_H), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuPHS(void)
{
	SET_USER_MENU_ID(eMENU_MAIN);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_NONE);
}

/**
  * @brief  Under the UNIT menu (eMENU_UNIT), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuUnitUp(void)
{
	SET_USER_MENU_ID(eMENU_P_H);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_P_H);
}

/**
  * @brief  Under the UNIT menu (eMENU_UNIT), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuUnitDown(void)
{
	SET_USER_MENU_ID(eMENU_DAP);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_DAP);
}

/**
  * @brief  Under the UNIT menu (eMENU_UNIT), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuUnitM(void)
{
	SET_USER_MENU_ID(eMENU_SUB_UNIT);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_SUB_UNIT);
}

/**
  * @brief  Under the UNIT menu (eMENU_UNIT), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuUnitS(void)
{
	SET_USER_MENU_ID(eMENU_MAIN);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_NONE);
}

/**
  * @brief  Under the DAP menu (eMENU_DAP), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuDapUp(void)
{
	SET_USER_MENU_ID(eMENU_UNIT);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_UNIT);
}

/**
  * @brief  Under the DAP menu (eMENU_DAP), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuDapDown(void)
{
	SET_USER_MENU_ID(eMENU_FUC1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_FUC1);
}

/**
  * @brief  Under the DAP menu (eMENU_DAP), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuDapM(void)
{
	SET_USER_MENU_ID(eMENU_SUB_DAP);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_SUB_DAP);
	
	SET_CURSOR_POSITION();
}

/**
  * @brief  Under the DAP menu (eMENU_DAP), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuDapS(void)
{
	SET_USER_MENU_ID(eMENU_MAIN);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_NONE);
}

/**
  * @brief  Under the FUC1 menu (eMENU_FUC1), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuFuc1Up(void)
{
	SET_USER_MENU_ID(eMENU_DAP);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_DAP);
}

/**
  * @brief  Under the FUC1 menu (eMENU_FUC1), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuFuc1Down(void)
{
	SET_USER_MENU_ID(eMENU_AL1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_AL1);
}

/**
  * @brief  Under the FUC1 menu (eMENU_FUC1), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuFuc1M(void)
{
	SET_USER_MENU_ID(eMENU_SUB_FUC1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_SUB_FUC1);
}

/**
  * @brief  Under the FUC1 menu (eMENU_FUC1), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuFuc1S(void)
{
	SET_USER_MENU_ID(eMENU_MAIN);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_NONE);
}

/**
  * @brief  Under the AL1 menu (eMENU_AL1), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuAL1Up(void)
{
	SET_USER_MENU_ID(eMENU_FUC1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_FUC1);
}

/**
  * @brief  Under the AL1 menu (eMENU_AL1), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuAL1Down(void)
{
	SET_USER_MENU_ID(eMENU_AH1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_AH1);
}

/**
  * @brief  Under the AL1 menu (eMENU_AL1), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuAL1M(void)
{
	SET_USER_MENU_ID(eMENU_SUB_AL1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_SUB_AL1);
	
	SET_BLINK_CURSOR_START_TIMER();
	SET_CURSOR_POSITION();
}

/**
  * @brief  Under the AL1 menu (eMENU_AL1), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuAL1S(void)
{
	SET_USER_MENU_ID(eMENU_MAIN);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_NONE);	
}

/**
  * @brief  Under the AH1 menu (eMENU_AH1), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuAH1Up(void)
{
	SET_USER_MENU_ID(eMENU_AL1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_AL1);	
}

/**
  * @brief  Under the AH1 menu (eMENU_AH1), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuAH1Down(void)
{
	SET_USER_MENU_ID(eMENU_DL1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_DL1);
}

/**
  * @brief  Under the AH1 menu (eMENU_AH1), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuAH1M(void)
{
	SET_USER_MENU_ID(eMENU_SUB_AH1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_SUB_AH1);
	
	SET_BLINK_CURSOR_START_TIMER();
	SET_CURSOR_POSITION();
}

/**
  * @brief  Under the AH1 menu (eMENU_AH1), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuAH1S(void)
{
	SET_USER_MENU_ID(eMENU_MAIN);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_NONE);	
}

/**
  * @brief  Under the DL1 menu (eMENU_DL1), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuDL1Up(void)
{
	SET_USER_MENU_ID(eMENU_AH1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_AH1);
}

/**
  * @brief  Under the DL1 menu (eMENU_DL1), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuDL1Down(void)
{
	SET_USER_MENU_ID(eMENU_FUC2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_FUC2);
}

/**
  * @brief  Under the DL1 menu (eMENU_DL1), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuDL1M(void)
{
	SET_USER_MENU_ID(eMENU_SUB_DL1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_SUB_DL1);
	
	SET_BLINK_CURSOR_START_TIMER();
	SET_CURSOR_POSITION();
}

/**
  * @brief  Under the DL1 menu (eMENU_DL1), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuDL1S(void)
{
	SET_USER_MENU_ID(eMENU_MAIN);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_NONE);
}

/**
  * @brief  Under the FUC2 menu (eMENU_FUC2), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuFuc2Up(void)
{
	SET_USER_MENU_ID(eMENU_DL1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_DL1);
}

/**
  * @brief  Under the FUC2 menu (eMENU_FUC2), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuFuc2Down(void)
{
	SET_USER_MENU_ID(eMENU_AL2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_AL2);
}

/**
  * @brief  Under the FUC2 menu (eMENU_FUC2), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuFuc2M(void)
{
	SET_USER_MENU_ID(eMENU_SUB_FUC2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_SUB_FUC2);
}

/**
  * @brief  Under the FUC2 menu (eMENU_FUC2), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuFuc2S(void)
{
	SET_USER_MENU_ID(eMENU_MAIN);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_NONE);
}

/**
  * @brief  Under the AL2 menu (eMENU_AL2), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuAL2Up(void)
{
	SET_USER_MENU_ID(eMENU_FUC2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_FUC2);
}

/**
  * @brief  Under the AL2 menu (eMENU_AL2), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuAL2Down(void)
{
	SET_USER_MENU_ID(eMENU_AH2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_AH2);
}

/**
  * @brief  Under the AL2 menu (eMENU_AL2), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuAL2M(void)
{
	SET_USER_MENU_ID(eMENU_SUB_AL2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_SUB_AL2);
	
	SET_BLINK_CURSOR_START_TIMER();
	SET_CURSOR_POSITION();
}

/**
  * @brief  Under the AL2 menu (eMENU_AL2), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuAL2S(void)
{
	SET_USER_MENU_ID(eMENU_MAIN);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_NONE);
}

/**
  * @brief  Under the AH2 menu (eMENU_AH2), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuAH2Up(void)
{
	SET_USER_MENU_ID(eMENU_AL2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_AL2);
}

/**
  * @brief  Under the AH2 menu (eMENU_AH2), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuAH2Down(void)
{
	SET_USER_MENU_ID(eMENU_DL2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_DL2);
}

/**
  * @brief  Under the AH2 menu (eMENU_AH2), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuAH2M(void)
{
	SET_USER_MENU_ID(eMENU_SUB_AH2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_SUB_AH2);
	
	SET_BLINK_CURSOR_START_TIMER();
	SET_CURSOR_POSITION();
}

/**
  * @brief  Under the AH2 menu (eMENU_AH2), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuAH2S(void)
{
	SET_USER_MENU_ID(eMENU_MAIN);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_NONE);
}

/**
  * @brief  Under the DL2 menu (eMENU_DL2), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuDL2Up(void)
{
	SET_USER_MENU_ID(eMENU_AH2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_AH2);
}

/**
  * @brief  Under the DL2 menu (eMENU_DL2), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuDL2Down(void)
{
	SET_USER_MENU_ID(eMENU_P_L);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_P_L);
}

/**
  * @brief  Under the DL2 menu (eMENU_DL2), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuDL2M(void)
{
	SET_USER_MENU_ID(eMENU_SUB_DL2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_SUB_DL2);
	
	SET_BLINK_CURSOR_START_TIMER();
	SET_CURSOR_POSITION();	
}

/**
  * @brief  Under the DL2 menu (eMENU_DL2), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuDL2S(void)
{
	SET_USER_MENU_ID(eMENU_MAIN);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_NONE);
}

/**
  * @brief  Under the P-L sub menu (eMENU_SUB_P_L), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuSubPLUp(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].PL;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand < 9)
		{
			num_thousand++;
		}
		else
		{
			num_thousand = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred < 9)
		{
			num_hundred++;
		}
		else
		{
			num_hundred = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten < 9)
		{
			num_ten++;
		}
		else
		{
			num_ten = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit < 9)
		{
			num_unit++;
		}
		else
		{
			num_unit = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].PLDecimal;
		
		if(decimal < 3)
		{
			decimal++;
		}
		else
		{
			decimal = 0;
		}
		
		stSysAdjValSection2[stSysSaveDataSection1.eUnit].PLDecimal = decimal;
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].PL = val;
}

/**
  * @brief  Under the P-L sub menu (eMENU_SUB_P_L), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuSubPLDown(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].PL;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand > 0)
		{
			num_thousand--;
		}
		else
		{
			num_thousand = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred > 0)
		{
			num_hundred--;
		}
		else
		{
			num_hundred = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten > 0)
		{
			num_ten--;
		}
		else
		{
			num_ten = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit > 0)
		{
			num_unit--;
		}
		else
		{
			num_unit = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].PLDecimal;
		
		if(decimal > 0)
		{
			decimal--;
		}
		else
		{
			decimal = 3;
		}

		stSysAdjValSection2[stSysSaveDataSection1.eUnit].PLDecimal = decimal;
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].PL = val;
}

/**
  * @brief  Under the P-L sub menu (eMENU_SUB_P_L), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuSubPLM(void)
{
	UserMenuRightMoveCursorPos();
}

/**
  * @brief  Under the P-L sub menu (eMENU_SUB_P_L), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuSubPLS(void)
{
	SET_USER_MENU_ID(eMENU_P_L);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_P_L);
	
	CLR_BLINK_CURSOR_START_TIMER();
	
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].PL_L = stSysAdjValSection2[stSysSaveDataSection1.eUnit].PL & 0x00ff;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].PL_H = (stSysAdjValSection2[stSysSaveDataSection1.eUnit].PL & 0xff00) >> 8;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].PLDecimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].PLDecimal;
	
	switch(stSysSaveDataSection1.eUnit)
	{
		case UNIT_BAR:	
			EEPROM_WriteBytes(eBAR_P_L_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].PL_H, 2);
			EEPROM_WriteBytes(eBAR_P_L_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].PLDecimal, 1);
			break;
		
		case UNIT_PSI:
			EEPROM_WriteBytes(ePSI_P_L_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].PL_H, 2);
			EEPROM_WriteBytes(ePSI_P_L_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].PLDecimal, 1);
			break;
		
		default:
			break;
	}
}

/**
  * @brief  Under the P-H sub menu (eMENU_SUB_P_H), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuSubPHUp(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].PH;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand < 9)
		{
			num_thousand++;
		}
		else
		{
			num_thousand = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred < 9)
		{
			num_hundred++;
		}
		else
		{
			num_hundred = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten < 9)
		{
			num_ten++;
		}
		else
		{
			num_ten = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit < 9)
		{
			num_unit++;
		}
		else
		{
			num_unit = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].PHDecimal;
		
		if(decimal < 3)
		{
			decimal++;
		}
		else
		{
			decimal = 0;
		}
		
		stSysAdjValSection2[stSysSaveDataSection1.eUnit].PHDecimal = decimal;
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].PH = val;
}

/**
  * @brief  Under the P-H sub menu (eMENU_SUB_P_H), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuSubPHDown(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].PH;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand > 0)
		{
			num_thousand--;
		}
		else
		{
			num_thousand = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred > 0)
		{
			num_hundred--;
		}
		else
		{
			num_hundred = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten > 0)
		{
			num_ten--;
		}
		else
		{
			num_ten = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit > 0)
		{
			num_unit--;
		}
		else
		{
			num_unit = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].PHDecimal;
		
		if(decimal > 0)
		{
			decimal--;
		}
		else
		{
			decimal = 3;
		}

		stSysAdjValSection2[stSysSaveDataSection1.eUnit].PHDecimal = decimal;
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].PH = val;
}

/**
  * @brief  Under the P-H sub menu (eMENU_SUB_P_H), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuSubPHM(void)
{
	UserMenuRightMoveCursorPos();
}

/**
  * @brief  Under the P-H sub menu (eMENU_SUB_P_H), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuSubPHS(void)
{
	SET_USER_MENU_ID(eMENU_P_H);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_P_H);
	
	CLR_BLINK_CURSOR_START_TIMER();
	
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].PH_L = stSysAdjValSection2[stSysSaveDataSection1.eUnit].PH & 0x00ff;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].PH_H = (stSysAdjValSection2[stSysSaveDataSection1.eUnit].PH & 0xff00) >> 8;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].PHDecimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].PHDecimal;
	
	switch(stSysSaveDataSection1.eUnit)
	{
		case UNIT_BAR:	
			EEPROM_WriteBytes(eBAR_P_H_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].PH_H, 2);
			EEPROM_WriteBytes(eBAR_P_H_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].PHDecimal, 1);
			break;
		
		case UNIT_PSI:
			EEPROM_WriteBytes(ePSI_P_H_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].PH_H, 2);
			EEPROM_WriteBytes(ePSI_P_H_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].PHDecimal, 1);
			break;
		
		default:
			break;
	}
}

/**
  * @brief  Under the UNIT sub menu (eMENU_SUB_UNIT), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuSubUnitUp(void)
{
	if(stSysAdjValSection1.eUnit == UNIT_PSI)
	{
		stSysAdjValSection1.eUnit = UNIT_BAR;
	}
	else
	{
		stSysAdjValSection1.eUnit = UNIT_PSI;
	}
}

/**
  * @brief  Under the UNIT sub menu (eMENU_SUB_UNIT), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuSubUnitDown(void)
{
	if(stSysAdjValSection1.eUnit == UNIT_PSI)
	{
		stSysAdjValSection1.eUnit = UNIT_BAR;
	}
	else
	{
		stSysAdjValSection1.eUnit = UNIT_PSI;
	}
}

/**
  * @brief  Under the UNIT sub menu (eMENU_SUB_UNIT), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuSubUnitM(void)
{

}

/**
  * @brief  Under the UNIT sub menu (eMENU_SUB_UNIT), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuSubUnitS(void)
{
	stSysSaveDataSection1.eUnit = stSysAdjValSection1.eUnit;
	EEPROM_WriteBytes(eSENSOR_UNIT, (uint8_t *)&stSysSaveDataSection1.eUnit, 1);
	
	SET_USER_MENU_ID(eMENU_UNIT);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_UNIT);
}

/**
  * @brief  Under the DAP sub menu (eMENU_SUB_DAP), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuSubDapUp(void)
{
	if(stSysAdjValSection1.dap == DAP_ADJ_VAL_MAX)
	{
		stSysAdjValSection1.dap = DAP_ADJ_VAL_MIN;
	}
	else
	{
		stSysAdjValSection1.dap++;
	}
}

/**
  * @brief  Under the DAP sub menu (eMENU_SUB_DAP), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuSubDapDown(void)
{
	if(stSysAdjValSection1.dap == DAP_ADJ_VAL_MIN)
	{
		stSysAdjValSection1.dap = DAP_ADJ_VAL_MAX;
	}
	else
	{
		stSysAdjValSection1.dap--;
	}
}

/**
  * @brief  Under the DAP sub menu (eMENU_SUB_DAP), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuSubDapM(void)
{

}

/**
  * @brief  Under the DAP sub menu (eMENU_SUB_DAP), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuSubDapS(void)
{
	stSysSaveDataSection1.dap = stSysAdjValSection1.dap;
	EEPROM_WriteBytes(eSENSOR_DAP, (uint8_t *)&stSysSaveDataSection1.dap, 1);
	
	SET_USER_MENU_ID(eMENU_DAP);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_DAP);
}

/**
  * @brief  Under the FUNC1 sub menu (eMENU_SUB_FUNC1), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuSubFunc1Up(void)
{
	if(stSysAdjValSection1.eFunc1 == FUNC_HI)
	{
		stSysAdjValSection1.eFunc1 = FUNC_LO;
	}
	else if(stSysAdjValSection1.eFunc1 == FUNC_LO)
	{
		stSysAdjValSection1.eFunc1 = FUNC_WIN2;
	}
	else if(stSysAdjValSection1.eFunc1 == FUNC_WIN2)
	{
		stSysAdjValSection1.eFunc1 = FUNC_WIN1;
	}
	else if(stSysAdjValSection1.eFunc1 == FUNC_WIN1)
	{
		stSysAdjValSection1.eFunc1 = FUNC_HI;
	}
	else
	{
		// do nothing
	}
}

/**
  * @brief  Under the FUNC1 sub menu (eMENU_SUB_FUNC1), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuSubFunc1Down(void)
{
	if(stSysAdjValSection1.eFunc1 == FUNC_LO)
	{
		stSysAdjValSection1.eFunc1 = FUNC_HI;
	}
	else if(stSysAdjValSection1.eFunc1 == FUNC_HI)
	{
		stSysAdjValSection1.eFunc1 = FUNC_WIN1;
	}
	else if(stSysAdjValSection1.eFunc1 == FUNC_WIN1)
	{
		stSysAdjValSection1.eFunc1 = FUNC_WIN2;
	}
	else if(stSysAdjValSection1.eFunc1 == FUNC_WIN2)
	{
		stSysAdjValSection1.eFunc1 = FUNC_LO;
	}
	else
	{
		// do nothing
	}
}

/**
  * @brief  Under the FUNC1 sub menu (eMENU_SUB_FUNC1), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuSubFunc1M(void)
{

}

/**
  * @brief  Under the FUNC1 sub menu (eMENU_SUB_FUNC1), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuSubFunc1S(void)
{
	stSysSaveDataSection1.eFunc1 = stSysAdjValSection1.eFunc1;
	EEPROM_WriteBytes(eALM1_FUNC1_ADDR, (uint8_t *)&stSysSaveDataSection1.eFunc1, 1);
	
	SET_USER_MENU_ID(eMENU_FUC1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_FUC1);
}

/**
  * @brief  Under the AL1 sub menu (eMENU_SUB_AL1), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAL1Up(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL1;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand < 9)
		{
			num_thousand++;
		}
		else
		{
			num_thousand = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred < 9)
		{
			num_hundred++;
		}
		else
		{
			num_hundred = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten < 9)
		{
			num_ten++;
		}
		else
		{
			num_ten = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit < 9)
		{
			num_unit++;
		}
		else
		{
			num_unit = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL1Decimal;
		
		if(decimal < 3)
		{
			decimal++;
		}
		else
		{
			decimal = 0;
		}
		
		stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL1Decimal = decimal;
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL1 = val;
}

/**
  * @brief  Under the AL1 sub menu (eMENU_SUB_AL1), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAL1Down(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL1;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand > 0)
		{
			num_thousand--;
		}
		else
		{
			num_thousand = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred > 0)
		{
			num_hundred--;
		}
		else
		{
			num_hundred = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten > 0)
		{
			num_ten--;
		}
		else
		{
			num_ten = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit > 0)
		{
			num_unit--;
		}
		else
		{
			num_unit = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL1Decimal;
		
		if(decimal > 0)
		{
			decimal--;
		}
		else
		{
			decimal = 3;
		}

		stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL1Decimal = decimal;
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL1 = val;
}

/**
  * @brief  Under the AL1 sub menu (eMENU_SUB_AL1), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAL1M(void)
{
	UserMenuRightMoveCursorPos();
}

/**
  * @brief  Under the AL1 sub menu (eMENU_SUB_AL1), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAL1S(void)
{
	SET_USER_MENU_ID(eMENU_AL1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_AL1);
	
	SET_CALCULATE_AL1_FLAG();
	
	CLR_BLINK_CURSOR_START_TIMER();
	
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AL1_L = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL1 & 0x00ff;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AL1_H = (stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL1 & 0xff00) >> 8;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AL1Decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL1Decimal;
	
	switch(stSysSaveDataSection1.eUnit)
	{
		case UNIT_BAR:	
			EEPROM_WriteBytes(eBAR_A1_L_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].AL1_H, 2);
			EEPROM_WriteBytes(eBAR_A1_L_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].AL1Decimal, 1);
			break;
		
		case UNIT_PSI:
			EEPROM_WriteBytes(ePSI_A1_L_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].AL1_H, 2);
			EEPROM_WriteBytes(ePSI_A1_L_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].AL1Decimal, 1);
			break;
		
		default:
			break;
	}
}

/**
  * @brief  Under the AH1 sub menu (eMENU_SUB_AH1), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAH1Up(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH1;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand < 9)
		{
			num_thousand++;
		}
		else
		{
			num_thousand = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred < 9)
		{
			num_hundred++;
		}
		else
		{
			num_hundred = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten < 9)
		{
			num_ten++;
		}
		else
		{
			num_ten = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit < 9)
		{
			num_unit++;
		}
		else
		{
			num_unit = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH1Decimal;
		
		if(decimal < 3)
		{
			decimal++;
		}
		else
		{
			decimal = 0;
		}
		
		stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH1Decimal = decimal;
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH1 = val;
}

/**
  * @brief  Under the AH1 sub menu (eMENU_SUB_AH1), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAH1Down(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH1;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand > 0)
		{
			num_thousand--;
		}
		else
		{
			num_thousand = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred > 0)
		{
			num_hundred--;
		}
		else
		{
			num_hundred = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten > 0)
		{
			num_ten--;
		}
		else
		{
			num_ten = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit > 0)
		{
			num_unit--;
		}
		else
		{
			num_unit = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH1Decimal;
		
		if(decimal > 0)
		{
			decimal--;
		}
		else
		{
			decimal = 3;
		}

		stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH1Decimal = decimal;
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH1 = val;
}

/**
  * @brief  Under the AH1 sub menu (eMENU_SUB_AH1), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAH1M(void)
{
	UserMenuRightMoveCursorPos();
}

/**
  * @brief  Under the AL1 sub menu (eMENU_SUB_AL1), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAH1S(void)
{
	SET_USER_MENU_ID(eMENU_AH1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_AH1);
	
	SET_CALCULATE_AH1_FLAG();
	
	CLR_BLINK_CURSOR_START_TIMER();
	
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AH1_L = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH1 & 0x00ff;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AH1_H = (stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH1 & 0xff00) >> 8;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AH1Decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH1Decimal;
	
	switch(stSysSaveDataSection1.eUnit)
	{
		case UNIT_BAR:
			EEPROM_WriteBytes(eBAR_A1_H_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].AH1_H, 2);
			EEPROM_WriteBytes(eBAR_A1_H_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].AH1Decimal, 1);
			break;
		
		case UNIT_PSI:
			EEPROM_WriteBytes(ePSI_A1_H_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].AH1_H, 2);
			EEPROM_WriteBytes(ePSI_A1_H_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].AH1Decimal, 1);
			break;
		
		default:
			break;
	}
}

/**
  * @brief  Under the DL1 sub menu (eMENU_SUB_DL1), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuSubDL1Up(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AlDL1;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand < 9)
		{
			num_thousand++;
		}
		else
		{
			num_thousand = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred < 9)
		{
			num_hundred++;
		}
		else
		{
			num_hundred = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten < 9)
		{
			num_ten++;
		}
		else
		{
			num_ten = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit < 9)
		{
			num_unit++;
		}
		else
		{
			num_unit = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].A1DL1Decimal;
		
		if(decimal < 3)
		{
			decimal++;
		}
		else
		{
			decimal = 0;
		}

		stSysAdjValSection2[stSysSaveDataSection1.eUnit].A1DL1Decimal = decimal;
	}
	else
	{
		// do nothing
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].AlDL1 = val;
}

/**
  * @brief  Under the DL1 sub menu (eMENU_SUB_DL1), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuSubDL1Down(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AlDL1;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand > 0)
		{
			num_thousand--;
		}
		else
		{
			num_thousand = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred > 0)
		{
			num_hundred--;
		}
		else
		{
			num_hundred = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten > 0)
		{
			num_ten--;
		}
		else
		{
			num_ten = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit > 0)
		{
			num_unit--;
		}
		else
		{
			num_unit = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].A1DL1Decimal;
		
		if(decimal > 0)
		{
			decimal--;
		}
		else
		{
			decimal = 3;
		}

		stSysAdjValSection2[stSysSaveDataSection1.eUnit].A1DL1Decimal = decimal;
	}
	else
	{
		// do nothing
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].AlDL1 = val;
}

/**
  * @brief  Under the DL1 sub menu (eMENU_SUB_DL1), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuSubDL1M(void)
{
	UserMenuRightMoveCursorPos();
}

/**
  * @brief  Under the DL1 sub menu (eMENU_SUB_DL1), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuSubDL1S(void)
{
	SET_USER_MENU_ID(eMENU_DL1);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_DL1);
	
	CLR_BLINK_CURSOR_START_TIMER();
	
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].A1DL1_L = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AlDL1 & 0x00ff;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AlDL1_H = (stSysAdjValSection2[stSysSaveDataSection1.eUnit].AlDL1 & 0xff00) >> 8;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].A1DL1Decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].A1DL1Decimal;
	
	switch(stSysSaveDataSection1.eUnit)
	{
		case UNIT_BAR:
			EEPROM_WriteBytes(eBAR_A1_DL1_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].AlDL1_H, 2);
			EEPROM_WriteBytes(eBAR_A1_DL1_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].A1DL1Decimal, 1);
			break;
		
		case UNIT_PSI:
			EEPROM_WriteBytes(ePSI_A1_DL1_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].AlDL1_H, 2);
			EEPROM_WriteBytes(ePSI_A1_DL1_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].A1DL1Decimal, 1);
			break;
		
		default:
			break;
	}
}

/**
  * @brief  Under the FUC2 sub menu (eMENU_SUB_FUC2), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuSubFuc2Up(void)
{
	if(stSysAdjValSection1.eFunc2 == FUNC_HI)
	{
		stSysAdjValSection1.eFunc2 = FUNC_LO;
	}
	else if(stSysAdjValSection1.eFunc2 == FUNC_LO)
	{
		stSysAdjValSection1.eFunc2 = FUNC_WIN2;
	}
	else if(stSysAdjValSection1.eFunc2 == FUNC_WIN2)
	{
		stSysAdjValSection1.eFunc2 = FUNC_WIN1;
	}
	else if(stSysAdjValSection1.eFunc2 == FUNC_WIN1)
	{
		stSysAdjValSection1.eFunc2 = FUNC_HI;
	}
	else
	{
		// do nothing
	}
}

/**
  * @brief  Under the FUC2 sub menu (eMENU_SUB_FUC2), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuSubFuc2Down(void)
{
	if(stSysAdjValSection1.eFunc2 == FUNC_LO)
	{
		stSysAdjValSection1.eFunc2 = FUNC_HI;
	}
	else if(stSysAdjValSection1.eFunc2 == FUNC_HI)
	{
		stSysAdjValSection1.eFunc2 = FUNC_WIN1;
	}
	else if(stSysAdjValSection1.eFunc2 == FUNC_WIN1)
	{
		stSysAdjValSection1.eFunc2 = FUNC_WIN2;
	}
	else if(stSysAdjValSection1.eFunc2 == FUNC_WIN2)
	{
		stSysAdjValSection1.eFunc2 = FUNC_LO;
	}
	else
	{
		// do nothing
	}
}

/**
  * @brief  Under the FUC2 sub menu (eMENU_SUB_FUC2), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuSubFuc2M(void)
{

}

/**
  * @brief  Under the FUC2 sub menu (eMENU_SUB_FUC2), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuSubFuc2S(void)
{
	SET_USER_MENU_ID(eMENU_FUC2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_FUC2);
	
	stSysSaveDataSection1.eFunc2 = stSysAdjValSection1.eFunc2;
	EEPROM_WriteBytes(eALM2_FUNC2_ADDR, (uint8_t *)&stSysSaveDataSection1.eFunc2, 1);
}

/**
  * @brief  Under the AL2 sub menu (eMENU_SUB_AL2), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAL2Up(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL2;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand < 9)
		{
			num_thousand++;
		}
		else
		{
			num_thousand = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred < 9)
		{
			num_hundred++;
		}
		else
		{
			num_hundred = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten < 9)
		{
			num_ten++;
		}
		else
		{
			num_ten = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit < 9)
		{
			num_unit++;
		}
		else
		{
			num_unit = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL2Decimal;
		
		if(decimal < 3)
		{
			decimal++;
		}
		else
		{
			decimal = 0;
		}
		
		stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL2Decimal = decimal;
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL2 = val;
}

/**
  * @brief  Under the AL2 sub menu (eMENU_SUB_AL2), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAL2Down(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL2;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand > 0)
		{
			num_thousand--;
		}
		else
		{
			num_thousand = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred > 0)
		{
			num_hundred--;
		}
		else
		{
			num_hundred = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten > 0)
		{
			num_ten--;
		}
		else
		{
			num_ten = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit > 0)
		{
			num_unit--;
		}
		else
		{
			num_unit = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL2Decimal;
		
		if(decimal > 0)
		{
			decimal--;
		}
		else
		{
			decimal = 3;
		}

		stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL2Decimal = decimal;
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL2 = val;
}

/**
  * @brief  Under the AL2 sub menu (eMENU_SUB_AL2), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAL2M(void)
{
	UserMenuRightMoveCursorPos();
}

/**
  * @brief  Under the AL2 sub menu (eMENU_SUB_AL2), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAL2S(void)
{
	SET_USER_MENU_ID(eMENU_AL2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_AL2);
	
	SET_CALCULATE_AL2_FLAG();
	
	CLR_BLINK_CURSOR_START_TIMER();
	
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AL2_L = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL2 & 0x00ff;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AL2_H = (stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL2 & 0xff00) >> 8;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AL2Decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AL2Decimal;
	
	switch(stSysSaveDataSection1.eUnit)
	{
		case UNIT_BAR:	
			EEPROM_WriteBytes(eBAR_A2_L_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].AL2_H, 2);
			EEPROM_WriteBytes(eBAR_A2_L_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].AL2Decimal, 1);
			break;
		
		case UNIT_PSI:
			EEPROM_WriteBytes(ePSI_A2_L_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].AL2_H, 2);
			EEPROM_WriteBytes(ePSI_A2_L_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].AL2Decimal, 1);
			break;
		
		default:
			break;
	}
}

/**
  * @brief  Under the AH2 sub menu (eMENU_SUB_AH2), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAH2Up(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH2;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand < 9)
		{
			num_thousand++;
		}
		else
		{
			num_thousand = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred < 9)
		{
			num_hundred++;
		}
		else
		{
			num_hundred = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten < 9)
		{
			num_ten++;
		}
		else
		{
			num_ten = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit < 9)
		{
			num_unit++;
		}
		else
		{
			num_unit = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH2Decimal;
		
		if(decimal < 3)
		{
			decimal++;
		}
		else
		{
			decimal = 0;
		}
		
		stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH2Decimal = decimal;
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH2 = val;
}

/**
  * @brief  Under the AH2 sub menu (eMENU_SUB_AH2), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAH2Down(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH2;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand > 0)
		{
			num_thousand--;
		}
		else
		{
			num_thousand = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred > 0)
		{
			num_hundred--;
		}
		else
		{
			num_hundred = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten > 0)
		{
			num_ten--;
		}
		else
		{
			num_ten = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit > 0)
		{
			num_unit--;
		}
		else
		{
			num_unit = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH2Decimal;
		
		if(decimal > 0)
		{
			decimal--;
		}
		else
		{
			decimal = 3;
		}

		stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH2Decimal = decimal;
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH2 = val;
}

/**
  * @brief  Under the AH2 sub menu (eMENU_SUB_AH2), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAH2M(void)
{
	UserMenuRightMoveCursorPos();
}

/**
  * @brief  Under the AH2 sub menu (eMENU_SUB_AH2), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuSubAH2S(void)
{
	SET_USER_MENU_ID(eMENU_AH2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_AH2);
	
	SET_CALCULATE_AH2_FLAG();
	
	CLR_BLINK_CURSOR_START_TIMER();
	
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AH2_L = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH2 & 0x00ff;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AH2_H = (stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH2 & 0xff00) >> 8;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].AH2Decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].AH2Decimal;
	
	switch(stSysSaveDataSection1.eUnit)
	{
		case UNIT_BAR:
			EEPROM_WriteBytes(eBAR_A2_H_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].AH2_H, 2);
			EEPROM_WriteBytes(eBAR_A2_H_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].AH2Decimal, 1);
			break;
		
		case UNIT_PSI:
			EEPROM_WriteBytes(ePSI_A2_H_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].AH2_H, 2);
			EEPROM_WriteBytes(ePSI_A2_H_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].AH2Decimal, 1);
			break;
		
		default:
			break;
	}
}

/**
  * @brief  Under the DL2 sub menu (eMENU_SUB_DL2), press UP key.
  * @param  None
  * @retval None
  */
static void UserMenuSubDL2Up(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].A2DL2;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand < 9)
		{
			num_thousand++;
		}
		else
		{
			num_thousand = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred < 9)
		{
			num_hundred++;
		}
		else
		{
			num_hundred = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten < 9)
		{
			num_ten++;
		}
		else
		{
			num_ten = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit < 9)
		{
			num_unit++;
		}
		else
		{
			num_unit = 0;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].A2DL2Decimal;
		
		if(decimal < 3)
		{
			decimal++;
		}
		else
		{
			decimal = 0;
		}

		stSysAdjValSection2[stSysSaveDataSection1.eUnit].A2DL2Decimal = decimal;
	}
	else
	{
		// do nothing
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].A2DL2 = val;
}

/**
  * @brief  Under the DL2 sub menu (eMENU_SUB_DL2), press DOWN key.
  * @param  None
  * @retval None
  */
static void UserMenuSubDL2Down(void)
{
	uint16_t val = stSysAdjValSection2[stSysSaveDataSection1.eUnit].A2DL2;
	
	uint8_t num_unit = val % 10;
	uint8_t num_ten = (val % 100) / 10;
	uint8_t num_hundred = (val % 1000) / 100;
	uint8_t num_thousand = val / 1000;
	
	// bit 0
	if(GET_CURSOR_POSITION() & _BIT0)
	{	
		if(num_thousand > 0)
		{
			num_thousand--;
		}
		else
		{
			num_thousand = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT1) // bit 1
	{	
		if(num_hundred > 0)
		{
			num_hundred--;
		}
		else
		{
			num_hundred = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT2) // bit 2
	{
		if(num_ten > 0)
		{
			num_ten--;
		}
		else
		{
			num_ten = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT3) // bit 3
	{
		if(num_unit > 0)
		{
			num_unit--;
		}
		else
		{
			num_unit = 9;
		}
	}
	else if(GET_CURSOR_POSITION() & _BIT4) // bit 4
	{
		uint8_t decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].A2DL2Decimal;
		
		if(decimal > 0)
		{
			decimal--;
		}
		else
		{
			decimal = 3;
		}

		stSysAdjValSection2[stSysSaveDataSection1.eUnit].A2DL2Decimal = decimal;
	}
	else
	{
		// do nothing
	}
	
	val = num_thousand * 1000 + num_hundred * 100 + num_ten * 10 + num_unit;
	stSysAdjValSection2[stSysSaveDataSection1.eUnit].A2DL2 = val;
}

/**
  * @brief  Under the DL2 sub menu (eMENU_SUB_DL2), press M key.
  * @param  None
  * @retval None
  */
static void UserMenuSubDL2M(void)
{
	UserMenuRightMoveCursorPos();
}

/**
  * @brief  Under the DL2 sub menu (eMENU_SUB_DL2), press S key.
  * @param  None
  * @retval None
  */
static void UserMenuSubDL2S(void)
{
	SET_USER_MENU_ID(eMENU_DL2);
	SET_USER_MENU_DISPLAY_ID(eMENU_DISPLAY_DL2);
	
	CLR_BLINK_CURSOR_START_TIMER();
	
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].A2DL2_L = stSysAdjValSection2[stSysSaveDataSection1.eUnit].A2DL2 & 0x00ff;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].A2DL2_H = (stSysAdjValSection2[stSysSaveDataSection1.eUnit].A2DL2 & 0xff00) >> 8;
	stSysSaveDataSection2[stSysSaveDataSection1.eUnit].A2DL2Decimal = stSysAdjValSection2[stSysSaveDataSection1.eUnit].A2DL2Decimal;
	
	switch(stSysSaveDataSection1.eUnit)
	{
		case UNIT_BAR:
			EEPROM_WriteBytes(eBAR_A2_DL2_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].A2DL2_H, 2);
			EEPROM_WriteBytes(eBAR_A2_DL2_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_BAR].A2DL2Decimal, 1);
			break;
		
		case UNIT_PSI:
			EEPROM_WriteBytes(ePSI_A2_DL2_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].A2DL2_H, 2);
			EEPROM_WriteBytes(ePSI_A2_DL2_DECIMAL_ADDR, (uint8_t *)&stSysSaveDataSection2[UNIT_PSI].A2DL2Decimal, 1);
			break;
		
		default:
			break;
	}
}
