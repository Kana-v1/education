//
// Created by Kana on 6/5/2023.
//

#ifndef MCU1_CLION_STM32F303XX_RCC_DRIVER_H
#define MCU1_CLION_STM32F303XX_RCC_DRIVER_H

#include "stm32f303xx.h"

//This returns the APB1 clock value
uint32_t RCC_GetPCLK1Value(void);

//This returns the APB2 clock value
uint32_t RCC_GetPCLK2Value(void);

uint32_t  RCC_GetPLLOutputClock(void);

#endif //MCU1_CLION_STM32F303XX_RCC_DRIVER_H
