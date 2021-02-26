/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    16-May-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "debug.h"
#include "usart.h"
#include "LG3641AH.h"
#include "dac.h"
#include "adc.h"
#include "tim.h"
#include "key.h"
#include "user_menu.h"
#include "eeprom.h"
#include "iwdg.h"
#include "spi.h"
#include "NSA2860.h"
#include "math.h"
#include "receiveCmd.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t testBuffer[10] = {0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Delay( uint32_t count )
{
	for(; count!=0; count--);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32l1xx_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32l1xx.c file
     */ 
	
  /* Add your application code here
     */
	USART2_Init(9600);
	DEBUG_MCO_GPIOInit();
	LED_SegmentDisplay_GPIOInit();
	DAC_Mode_Init();
	ADC_InitConfig();
	TIM3_Init();
	SPI_InitConfig();
	
	EEPROM_PowerOnLoad();
	
	NSA2860_Init();
	
	//NSA2860_DACWrite(0x38, 0x8E);
	
	
	//IWDG_Config();
	
//	GPIO_InitTypeDef GPIO_InitTypeStruct;
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
//	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitTypeStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOA, &GPIO_InitTypeStruct);
	
//	uint8_t i = 0;
//	
//	__disable_irq();
//	DATA_EEPROM_Unlock();
//	
//	for(i = 0; i < 4; i++)
//	{
//		DATA_EEPROM_ProgramByte(EEPROM_BASE_ADDR + i, 0x00);
//	}
//	
//	DATA_EEPROM_Lock();
//	__enable_irq();
	
  /* Infinite loop */
  while (1)
  {	
		//GPIO_SetBits(GPIOA, GPIO_Pin_4);
		
		AlarmHandler();
		KEY_Handler();
		LED_SegmentDisplay_Handler();
		//LED_SegmentDisplay_SendData(0x00, 0x88);
		UserMenuCall();
		
		//IWDG_Feed();
		
		//GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		
//		extern uint16_t timeTest;
//		while(timeTest)
//		{
//			
//		}
//		timeTest=50;
		
//		while(1)
//		{
//			printf("123\n");
//			Delay_ms(500);
//			printf("456\n");
//			Delay_ms(500);
//		}
		
		CmdFromHostComputerHandler();
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
