/**
  ******************************************************************************
  * @file    $ProjectName/usart/receiveCmd.h 
  * @author  WX
  * @version V1.0.0
  * @date    2021-2-23
  * @brief   receiveCmd.c file configuration.
  ******************************************************************************
  */

#ifndef RECEIVE_CMD_H
#define RECEIVE_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

/* Exported macro ------------------------------------------------------------*/
#define GET_A2_AUTO_UPDATE_FLG()                            (revCmdFlag & _BIT0)
#define SET_A2_AUTO_UPDATE_FLG()                            (revCmdFlag |= _BIT0)
#define CLR_A2_AUTO_UPDATE_FLG()														(revCmdFlag &= ~_BIT0)

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void CmdFromHostComputerHandler(void);

/* Exported variables --------------------------------------------------------*/
extern uint8_t revCmdFlag;

#endif // #ifndef RECEIVE_CMD_H
