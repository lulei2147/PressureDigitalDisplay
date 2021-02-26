#ifndef __SYS_H_
#define __SYS_H_ 

#include "stm32l1xx.h"
#include "general_type.h"
#include <stdio.h>

//GPIO位操作
#define PAtog(pin)       GPIOA->ODR ^= 1<<(pin)
#define PAoutL(pin)      GPIOA->BSRRH = 1<<(pin)
#define PAoutH(pin)      GPIOA->BSRRL = 1<<(pin)
#define PAout(pin)      ((GPIOA->ODR & (1<<(pin))) >> (pin))
#define PAin(pin)       ((GPIOA->IDR & (1<<(pin))) >> (pin))

#define PBtog(pin)       GPIOB->ODR ^= 1<<(pin)
#define PBoutL(pin)      GPIOB->BSRRH = 1<<(pin)
#define PBoutH(pin)      GPIOB->BSRRL = 1<<(pin)
#define PBout(pin)      ((GPIOB->ODR & (1<<(pin))) >> (pin))
#define PBin(pin)       ((GPIOB->IDR & (1<<(pin))) >> (pin))

#define PCtog(pin)       GPIOC->ODR ^= 1<<(pin)
#define PCoutL(pin)      GPIOC->BSRRH = 1<<(pin)
#define PCoutH(pin)      GPIOC->BSRRL = 1<<(pin)
#define PCout(pin)      ((GPIOC->ODR & (1<<(pin))) >> (pin))
#define PCin(pin)       ((GPIOC->IDR & (1<<(pin))) >> (pin))

#define EN_INT      	__enable_irq();		//系统开全局中断
#define DIS_INT     	__disable_irq();	//系统关全局中断

#define MAX(a,b)        (((a)>(b))?(a):(b))
#define MIN(a,b)        (((a)<(b))?(a):(b))
#define DEF(a,b)        (((a)>(b))?(a)-(b):(b)-(a)) //两个数的绝对差值
#define ABS(a)          (((a)<(0))?(-a):(a)) //绝对值

#define class struct

void NVIC_Config(void);

#endif











