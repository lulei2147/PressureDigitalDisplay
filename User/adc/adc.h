#ifndef ADC_H
#define ADC_H

#include "stm32l1xx.h"

void ADC_InitConfig(void);
void ADC_InitConfigInt(void);

extern __IO uint16_t ADC_ConvertedValue;
extern float ADC_Vol;

#endif // #ifndef ADC_H
