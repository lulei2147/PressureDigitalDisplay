#include "dac.h"

#define test      0

extern __IO uint16_t ADC_ConvertedValue;

#define A              0
#define B              (510) // 400mv
const uint16_t Sine12bit[2] = {
	B, B,
};

__IO uint16_t testVal = 4095/2;

void DAC_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	DAC_InitTypeDef DAC_InitTypeStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitTypeStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitTypeStruct);
	
	DAC_DeInit();
	DAC_InitTypeStruct.DAC_Trigger = DAC_Trigger_T2_TRGO;
	DAC_InitTypeStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitTypeStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitTypeStruct);
	
	DAC_Cmd(DAC_Channel_1, ENABLE);
	
	DAC_DMACmd(DAC_Channel_1, ENABLE);
}

static void DAC_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0x00;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = (SystemCoreClock / 1000) * 1 / 20 - 1;  // 50us
	TIM_TimeBaseInitStruct.TIM_Prescaler = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
	
	TIM_Cmd(TIM2, ENABLE);
}

static void DAC_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Channel2);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(DAC->DHR12R1));
#if(test == 1)
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&testVal;//(uint32_t)&ADC_ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
#else
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Sine12bit; //(uint32_t)&DualSine12bit;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
#endif
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel2, ENABLE);
}

void DAC_Mode_Init(void)
{
	DAC_InitConfig();
	DAC_TIM_Config();
	DAC_DMA_Config();
}
