/**
  ******************************************************************************
  * @file    $ProjectName/spi/NSA2860.c 
  * @author  WX
  * @version V1.0.0
  * @date    2021-1-8
  * @brief   NSA2860 driver.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "NSA2860.h"
#include "spi.h"
#include "tim.h"
#include "stdio.h"
#include "math.h"

/* Private typedef -----------------------------------------------------------*/
// COMMAND
enum
{
	REG_COMMAND_CMD_MODE = 0x00,
	REG_COMMAND_RESERVED_1 = 0x01,
	REG_COMMAND_RESERVED_2 = 0x02,
	REG_COMMAND_CONTINUE_MODE = 0x03,
	REG_COMMAND_EEP_MODE = 0x33,
};

/* Private define ------------------------------------------------------------*/
#define REG_WRITE                                               (0x00)
#define REG_READ                                                (0x80)

// register
// general register
#define REG_IF_CTRL                                             (0x00)
#define REG_STATUS                                              (0x02)
// ADC
#define REG_PDATA_H                                             (0x06)
#define REG_PDATA_M                                             (0x07)
#define REG_PDATA_L                                             (0x08)
// temperature
#define REG_TDATA_H                                             (0x09)
#define REG_TDATA_M                                             (0x0a)
#define REG_TDATA_L                                             (0x0b)
// DAC
#define REG_DAC_DATA_H                                          (0x12)
#define REG_DAC_DATA_L                                          (0x13)
#define REG_DAC_BLANK                                           (0x14)
// COMMAND
#define REG_COMMAND                                             (0x30)

// eeprom register
// SYS_CONFIG1
#define REG_SYS_CONFIG1                                         (0xA1)
// SYS_CONFIG2
#define REG_SYS_CONFIG2                                         (0xA2)
// PCH_CONFIG1
#define REG_PCH_CONFIG1                                         (0xA4)
// PCH_CONFIG2
#define REG_PCH_CONFIG2                                         (0xA5)
// TCH_CONFIG
#define REG_TCH_CONFIG                                          (0xA6)

// GAIN
#define PCH_CONFIG1_GAIN_P_1X                                   (0x00)
#define PCH_CONFIG1_GAIN_P_2X                                   (0x10)
#define PCH_CONFIG1_GAIN_P_4X                                   (0x20)
#define PCH_CONFIG1_GAIN_P_6X                                   (0x30)
#define PCH_CONFIG1_GAIN_P_8X                                   (0x40)
#define PCH_CONFIG1_GAIN_P_12X                                  (0x50)
#define PCH_CONFIG1_GAIN_P_16X                                   (0x60)
#define PCH_CONFIG1_GAIN_P_24X                                   (0x70)
#define PCH_CONFIG1_GAIN_P_32X                                   (0x00)
#define PCH_CONFIG1_GAIN_P_48X                                   (0x00)
#define PCH_CONFIG1_GAIN_P_64X                                   (0x00)
#define PCH_CONFIG1_GAIN_P_96X                                   (0x00)
#define PCH_CONFIG1_GAIN_P_128X                                   (0x00)
#define PCH_CONFIG1_GAIN_P_192X                                   (0x00)
#define PCH_CONFIG1_GAIN_P_128X                                   (0x00)

/* Private function prototypes -----------------------------------------------*/
void NSA2860_WriteDataToReg(uint8_t regAddr, uint8_t data);
void NSA2860_InterTemperatureSensorInit(void);
void NSA2860_ReadDataFromGeneralReg(uint8_t regAddr, uint8_t *pBuffer, uint8_t length);
void NSA2860_DAC_ADCInit(void);

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  NSA2860 Configuration.
  * @param  None
  * @retval None
  */
void NSA2860_Init(void)
{
	Delay_ms(300);
	
	NSA2860_InterTemperatureSensorInit();
	NSA2860_DAC_ADCInit();
}

/**
  * @brief  NSA2860 internal temperature sensor initialization.
  * @param  None
  * @retval None
  */
void NSA2860_InterTemperatureSensorInit(void)
{
	NSA2860_WriteDataToReg(REG_COMMAND, REG_COMMAND_CMD_MODE);
	NSA2860_WriteDataToReg(REG_TCH_CONFIG, 0x40);
	NSA2860_WriteDataToReg(REG_COMMAND, REG_COMMAND_CONTINUE_MODE);
}

/**
  * @brief  NSA2860 DAC and ADC initialization.
  * @param  None
  * @retval None
  */
void NSA2860_DAC_ADCInit(void)
{
	NSA2860_WriteDataToReg(REG_COMMAND, REG_COMMAND_CMD_MODE);
	
	// DAC
	// VREFP : 2.5V, OUT_MODE : 0000 Voltage
	NSA2860_WriteDataToReg(REG_SYS_CONFIG2, 0x90);
	// DAC_REF : 1.2V, SYS_CHOP_EN : 1, RAW_P : 1
	NSA2860_WriteDataToReg(REG_PCH_CONFIG2, 0x85);
	
	// ADC
	// GAIN_P : 0111:24X, ODR_P : 0000:2.4KHz
	NSA2860_WriteDataToReg(REG_PCH_CONFIG1, 0x85);
	
	NSA2860_WriteDataToReg(REG_COMMAND, REG_COMMAND_CONTINUE_MODE);
}

/**
  * @brief  Configure the register, write data to the register.
	* @param  regAddr: register address.
	* @param  data: data to be written.
  * @retval None
  */
void NSA2860_WriteDataToReg(uint8_t regAddr, uint8_t data)
{
	SPI_WriteByte(REG_WRITE);
	SPI_WriteByte(regAddr);
	SPI_WriteByte(data);
}

/**
  * @brief  Read data from the general register.
	* @param  reAddr: register address
	* @param  pBuffer: space to save data.
	* @param  length: the length of the read data.
  * @retval None
  */
void NSA2860_ReadDataFromGeneralReg(uint8_t regAddr, uint8_t *pBuffer, uint8_t length)
{
	uint8_t i = 0;
	uint8_t addr = regAddr;
	
	if(addr <= REG_COMMAND)
	{
	
	}
	else
	{
		for(i = 0; i < length; i++)
		{
			SPI_WriteByte(REG_READ);
			SPI_WriteByte(regAddr + i);
			*(pBuffer + i) = SPI_WriteByte(0xFF);
		}
	}
}

/**
  * @brief  Load temperature sensor data.
	* @param  None
  * @retval None
  */
void NSA2860_LoadTempFromIntSensor(uint8_t *pData)
{
	uint8_t tempH = 0;
	uint8_t tempM = 0;
	uint8_t tempL = 0;
	uint32_t tdata = 0;
	float temperature = 0.0f;
	
	SPI_WriteByte(0x80);
	SPI_WriteByte(0x09);
	tempH = SPI_WriteByte(0xFF);
	printf("\ntempH [23:16]: 0x%X\n", tempH);
	
	SPI_WriteByte(0x80);
	SPI_WriteByte(0x0a);
	tempM = SPI_WriteByte(0xFF);
	printf("tempM [15:8]: 0x%X\n", tempM);
	
	SPI_WriteByte(0x80);
	SPI_WriteByte(0x0b);
	tempL = SPI_WriteByte(0xFF);
	printf("tempL [7:0]: 0x%X\n", tempL);
	
	if(tempH & 0x80)
	{
		tdata = ~(0xFF000000 | (((uint32_t)tempH) << 16) | (((uint32_t)tempM) << 8) | tempL) + 1;
		temperature = 25.0f - (float)tdata / pow(2, 16);
	}
	else
	{
		tdata = (((uint32_t)tempH) << 16) | (((uint32_t)tempM) << 8) | tempL;
		temperature = (float)tdata / pow(2, 16) + 25.0f;
	}
	
	printf("temperature : %f¡æ\n", temperature);	
}

/**
  * @brief  Write data to DAC register.
	* @param  None
  * @retval None
  */
void NSA2860_DACWrite(uint8_t data_L, uint8_t data_H)
{
	// DAC_BLANK = 1
	SPI_WriteByte(0x00);
	SPI_WriteByte(0x14);
	SPI_WriteByte(0xFF);
	
	// DAC_DATA[7:0]
	SPI_WriteByte(0x00);
	SPI_WriteByte(0x13);
	SPI_WriteByte(data_L);
	
	// DAC_DATA[15:8]
	SPI_WriteByte(0x00);
	SPI_WriteByte(0x12);
	SPI_WriteByte(data_H);
	
	// DAC_BLANK = 0
	SPI_WriteByte(0x00);
	SPI_WriteByte(0x14);
	SPI_WriteByte(0x00);
}
