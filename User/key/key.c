/**
  ******************************************************************************
  * @file    $ProjectName/User/key/key.c 
  * @author  WX
  * @version V1.0.0
  * @date    2020-7-9
  * @brief   KEY function program
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "key.h"
#include "eeprom.h"
#include "user_menu.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KEY_STSTUS_ON										1
#define KEY_STATUS_OFF									0

#define KEY_ON_TIME											(160)

#define KEY_UP_STATUS                   (uint32_t)(0x00000001)
#define KEY_DOWN_STATUS									(uint32_t)(0x00000002)
#define KEY_M_STATUS										(uint32_t)(0x00000004)
#define KEY_S_STATUS										(uint32_t)(0x00000008)
#define KEY_M_S_STATUS                  (uint32_t)(0x00000010)

/* Private macro -------------------------------------------------------------*/
#define GET_KEY_UP_STATUS()							((KEY_UP_GPIO_PORT->IDR & KEY_UP_GPIO_PIN) ? KEY_STATUS_OFF : KEY_UP_STATUS)
#define GET_KEY_DOWN_STATUS()						((KEY_DOWN_GPIO_PORT->IDR & KEY_DOWN_GPIO_PIN) ? KEY_STATUS_OFF : KEY_DOWN_STATUS)
#define GET_KEY_M_STATUS()							((KEY_M_GPIO_PORT->IDR & KEY_M_GPIO_PIN) ? KEY_STATUS_OFF : KEY_M_STATUS)
#define GET_KEY_S_STATUS()							((KEY_S_GPIO_PORT->IDR & KEY_S_GPIO_PIN) ? KEY_STATUS_OFF : KEY_S_STATUS)

#define GET_KEYS_STATUS()								(GET_KEY_UP_STATUS() | GET_KEY_DOWN_STATUS() | GET_KEY_M_STATUS() | GET_KEY_S_STATUS())

/* Private variables ---------------------------------------------------------*/
uint32_t keyUpTimeCounter = 0;
uint32_t keyDownTimeCounter = 0;
uint32_t keyMTimeCounter = 0;
uint32_t keySTimeCounter = 0;
uint32_t keyStatus = 0;
uint8_t keyMessage = eKEY_MESSAGE_NONE;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Key GPIO config.
  * @param  None
  * @retval None
  */
void KEY_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = KEY_UP_GPIO_PIN | KEY_DOWN_GPIO_PIN | KEY_M_GPIO_PIN | KEY_S_GPIO_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  Scan the key to determine the state of the key.
  * @param  None
  * @retval None
  */
void KEY_Scan(void)
{
	uint32_t Key_Status = GET_KEYS_STATUS();
	
	if(Key_Status)
	//if(GET_KEY_UP_STATUS() || GET_KEY_DOWN_STATUS() || GET_KEY_M_STATUS() || GET_KEY_S_STATUS())
	{
		// UP
		if((GET_KEY_UP_STATUS() == KEY_UP_STATUS) && 
			 (GET_KEY_DOWN_STATUS() == KEY_STATUS_OFF) &&
			 (GET_KEY_M_STATUS() == KEY_STATUS_OFF) &&
			 (GET_KEY_S_STATUS() == KEY_STATUS_OFF))
		{
			keyUpTimeCounter++;
			
			if(keyUpTimeCounter >= KEY_ON_TIME)
			{
				keyUpTimeCounter = 0;
				SET_KEY_STATUS(eKEY_UP_PRESSED);
			}
		}
		
		// DOWM
		if((GET_KEY_DOWN_STATUS() == KEY_DOWN_STATUS) && 
			 (GET_KEY_UP_STATUS() == KEY_STATUS_OFF) &&
			 (GET_KEY_M_STATUS() == KEY_STATUS_OFF) &&
			 (GET_KEY_S_STATUS() == KEY_STATUS_OFF))
		{
			keyDownTimeCounter++;
			
			if(keyDownTimeCounter >= KEY_ON_TIME)
			{
				keyDownTimeCounter = 0;
				SET_KEY_STATUS(eKEY_DOWN_PRESSED);
			}
		}
		
		// M
		if((GET_KEY_M_STATUS() == KEY_M_STATUS) && 
			 (GET_KEY_UP_STATUS() == KEY_STATUS_OFF) &&
			 (GET_KEY_DOWN_STATUS() == KEY_STATUS_OFF) &&
			 (GET_KEY_S_STATUS() == KEY_STATUS_OFF))
		{
			keyMTimeCounter++;
			
			if(keyMTimeCounter >= KEY_ON_TIME)
			{
				keyMTimeCounter = 0;
				SET_KEY_STATUS(eKEY_M_PRESSED);
			}
		}
		
		// S
		if((GET_KEY_S_STATUS() == KEY_S_STATUS) && 
			 (GET_KEY_UP_STATUS() == KEY_STATUS_OFF) &&
			 (GET_KEY_DOWN_STATUS() == KEY_STATUS_OFF) &&
			 (GET_KEY_M_STATUS() == KEY_STATUS_OFF))
		{
			keySTimeCounter++;
			
			if(keySTimeCounter >= KEY_ON_TIME)
			{
				keySTimeCounter = 0;
				SET_KEY_STATUS(eKEY_S_PRESSED);
			}
		}
	}
	else
	{
		keyUpTimeCounter = 0;
		keyDownTimeCounter = 0;
		keyMTimeCounter = 0;
		keySTimeCounter = 0;
		
		SET_KEY_STATUS(eKEY_NONE);
	}
}

/**
  * @brief  key event handling.
  * @param  None
  * @retval None
  */
void KEY_Handler(void)
{
	if(GET_KEY_STATUS() != eKEY_NONE)
	{
		// first load menu id
		if(((GET_KEY_STATUS() == eKEY_UP_PRESSED) || (GET_KEY_STATUS() == eKEY_DOWN_PRESSED) ||
				(GET_KEY_STATUS() == eKEY_M_PRESSED) || (GET_KEY_STATUS() == eKEY_S_PRESSED)) && (GET_USER_MENU_ID() == eMENU_NONE))
		{
			SET_USER_MENU_ID(eMENU_MAIN);
		}
		
		switch(GET_KEY_STATUS())
		{
			case eKEY_UP_PRESSED:
				printf("\r\n key UP \r\n");
				SET_KEY_MESSAGE(eKEY_MESSAGE_UP);
				keyUpTimeCounter = 0;
				break;
			
			case eKEY_DOWN_PRESSED:
				printf("\r\n key DOWN \r\n");
				SET_KEY_MESSAGE(eKEY_MESSAGE_DOWM);
				keyDownTimeCounter = 0;
				break;
			
			case eKEY_M_PRESSED:
				printf("\r\n key M \r\n");
				SET_KEY_MESSAGE(eKEY_MESSAGE_M);
				keyMTimeCounter = 0;
				break;
			
			case eKEY_S_PRESSED:
				printf("\r\n key S \r\n");
				SET_KEY_MESSAGE(eKEY_MESSAGE_S);
				keySTimeCounter = 0;
				break;
			
			case eKEY_M_S_PRESSED:
				break;
			
			default:
				break;
		}
		
		SET_KEY_STATUS(eKEY_NONE);
	}
}
