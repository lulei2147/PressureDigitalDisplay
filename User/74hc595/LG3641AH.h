/**
  ******************************************************************************
  * @file    $ProjectName/User/74hc595/LG3641AH.h 
  * @author  WX
  * @version V1.0.0
  * @date    2020-7-9
  * @brief   LG3641AH.c file configuration.
  ******************************************************************************
  */

#ifndef LG3641AH_H
#define LG3641AH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "commonInclude.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define NXETBE_SDI_GPIO_PORT																			GPIOB
#define NXETBE_SCK_GPIO_PORT																			GPIOB
#define NXETBE_RCK_GPIO_PORT																			GPIOB

#define NXETBE_SDI_GPIO_PIN										  									GPIO_Pin_3
#define NXETBE_SCK_GPIO_PIN										  									GPIO_Pin_4
#define NXETBE_RCK_GPIO_PIN										  									GPIO_Pin_5

#define SET_NXETBE_SDI()							      											GPIO_SetBits(NXETBE_SDI_GPIO_PORT, NXETBE_SDI_GPIO_PIN)
#define CLR_NXETBE_SDI()							      											GPIO_ResetBits(NXETBE_SDI_GPIO_PORT, NXETBE_SDI_GPIO_PIN)

#define SET_NXETBE_SCK()							      											GPIO_SetBits(NXETBE_SCK_GPIO_PORT, NXETBE_SCK_GPIO_PIN)
#define CLR_NXETBE_SCK()							      											GPIO_ResetBits(NXETBE_SCK_GPIO_PORT, NXETBE_SCK_GPIO_PIN)

#define SET_NXETBE_RCK()							      											GPIO_SetBits(NXETBE_RCK_GPIO_PORT, NXETBE_RCK_GPIO_PIN)
#define CLR_NXETBE_RCK()							      											GPIO_ResetBits(NXETBE_RCK_GPIO_PORT, NXETBE_RCK_GPIO_PIN)

// calculate flag
#define GET_CALCULATE_AL1_FLAG()											        		(extLedSegmentDisplayFlag & _BIT0)
#define SET_CALCULATE_AL1_FLAG()                                  (extLedSegmentDisplayFlag |= _BIT0)
#define CLR_CALCULATE_AL1_FLAG()                                  (extLedSegmentDisplayFlag &= ~_BIT0)

#define GET_CALCULATE_AH1_FLAG()											        		(extLedSegmentDisplayFlag & _BIT1)
#define SET_CALCULATE_AH1_FLAG()                                  (extLedSegmentDisplayFlag |= _BIT1)
#define CLR_CALCULATE_AH1_FLAG()                                  (extLedSegmentDisplayFlag &= ~_BIT1)

#define GET_CALCULATE_AL2_FLAG()											        		(extLedSegmentDisplayFlag & _BIT2)
#define SET_CALCULATE_AL2_FLAG()                                  (extLedSegmentDisplayFlag |= _BIT2)
#define CLR_CALCULATE_AL2_FLAG()                                  (extLedSegmentDisplayFlag &= ~_BIT2)

#define GET_CALCULATE_AH2_FLAG()											        		(extLedSegmentDisplayFlag & _BIT3)
#define SET_CALCULATE_AH2_FLAG()                                  (extLedSegmentDisplayFlag |= _BIT3)
#define CLR_CALCULATE_AH2_FLAG()                                  (extLedSegmentDisplayFlag &= ~_BIT3)

/* Exported functions ------------------------------------------------------- */
void LED_SegmentDisplay_GPIOInit(void);
void LED_SegmentDisplay_SendData(uint8_t data, uint8_t data0);
void LED_SegmentDisplay_Handler(void);
void LED_SegmentDealDisplay(char *pStr, uint8_t posIdx);
void AlarmHandler(void);

/* Exported variables --------------------------------------------------------*/
extern const uint8_t low8BitData[];
extern const uint8_t high8BitData[];

extern uint32_t SegDispTimeCounter;
extern uint8_t extLedSegmentDisplayFlag;

extern float calculatedAL1Val;
extern float calculatedAH1Val;
extern float calculatedAL2Val;
extern float calculatedAH2Val;

#endif // #ifndef LG3641AH_H
