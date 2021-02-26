/**
  ******************************************************************************
  * @file    $ProjectName/User/key/key.h 
  * @author  WX
  * @version V1.0.0
  * @date    2020-7-9
  * @brief   key.c file configuration.
  ******************************************************************************
  */
	
#ifndef KEY_H
#define KEY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "commonInclude.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	eKEY_NONE,
	eKEY_UP_PRESSED,
	eKEY_DOWN_PRESSED,
	eKEY_M_PRESSED,
	eKEY_S_PRESSED,
	
	eKEY_M_S_PRESSED,
}EnumKeyStatus;

enum
{
	eKEY_MESSAGE_UP = 0x00,
	eKEY_MESSAGE_DOWM,
	eKEY_MESSAGE_M,
	eKEY_MESSAGE_S,
	
	eKEY_MESSAGE_SUM,
	eKEY_MESSAGE_NONE = 0xff,
};

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define SET_KEY_STATUS(x)                    							(keyStatus = (x))
#define GET_KEY_STATUS()										 							(keyStatus)

#define SET_KEY_MESSAGE(x)                                (keyMessage = (x))
#define GET_KEY_MESSAGE()																	(keyMessage)

#define KEY_UP_GPIO_PORT 								     							GPIOB
#define KEY_DOWN_GPIO_PORT							    							KEY_UP_GPIO_PORT
#define KEY_M_GPIO_PORT									     							KEY_UP_GPIO_PORT
#define KEY_S_GPIO_PORT									     							KEY_UP_GPIO_PORT

#define KEY_UP_GPIO_PIN									     							GPIO_Pin_15
#define KEY_DOWN_GPIO_PIN							       							GPIO_Pin_12
#define KEY_M_GPIO_PIN							         							GPIO_Pin_14
#define KEY_S_GPIO_PIN									     							GPIO_Pin_13

/* Exported functions ------------------------------------------------------- */ 
void KEY_Scan(void);
void KEY_Handler(void);

/* Exported variables --------------------------------------------------------*/
extern uint32_t keyStatus;
extern uint8_t keyMessage;

#endif // #ifndef KEY_H
