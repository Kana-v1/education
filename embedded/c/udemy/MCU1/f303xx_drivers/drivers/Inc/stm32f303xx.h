/*
 * stm32f303xx.h
 *
 *  Created on: Apr 11, 2023
 *      Author: kana
 */
#include <stdint.h>
#include "stm32f303xx_gpio_driver.h"

#ifndef INC_STM32F303XX_H_

#define __vo volatile

#define NVIC_ISER0		 (( __vo uint32_t*)0xE000E100)
#define NVIC_ISER1		 (( __vo uint32_t*)0xE000E104)
#define NVIC_ISER2		 (( __vo uint32_t*)0xE000E108)
#define NVIC_ISER3		 (( __vo uint32_t*)0xE000E10C)

#define NVIC_ICER0		 (( __vo uint32_t*)0xE000E180)
#define NVIC_ICER1		 (( __vo uint32_t*)0xE000E184)
#define NVIC_ICER2		 (( __vo uint32_t*)0xE000E188)
#define NVIC_ICER3		 (( __vo uint32_t*)0xE000E18C)

// ARM Cortex Mx Processor Priority Register Address Calculation
#define NVIC_PR_BASE_ADDR 		((__vo uint32_t *)0xE000E400)
#define NO_PR_BITS_IMPLEMENTED 	4


#define INC_STM32F303XX_H_

#define FLASH_BASEADDR 			0x08000000U // Main memory
#define SRAM					0x20000000U
#define ROM 					0x1FFFD800U // System memory

#define PERIPH_BASE 			0x40000000U // APB1 base memory
#define APB1PERIPH_BASE			PERIPH_BASE
#define APB2PERIPH_BASE 		0x40010000U
#define AHB1PERIPH_BASE			0x40020000U
#define AHB2PERIPH_BASE 		0x48000000U
#define AHB3PERIPH_BASE			0x50000000U

#define GPIOA_BASEADDR			(AHB2PERIPH_BASE + 0x0000)
#define GPIOB_BASEADDR			(AHB2PERIPH_BASE + 0x0400)
#define GPIOC_BASEADDR			(AHB2PERIPH_BASE + 0x0800)
#define GPIOD_BASEADDR			(AHB2PERIPH_BASE + 0x0C00)
#define GPIOE_BASEADDR			(AHB2PERIPH_BASE + 0x1000)
#define GPIOF_BASEADDR			(AHB2PERIPH_BASE + 0x1400)
#define RCC_BASEADDR			0x40021000

#define I2C1_BASEADDR			(APB1PERIPH_BASE + 0x5400)
#define I2C2_BASEADDR			(APB1PERIPH_BASE + 0x5800)
#define SPI2_BASEADDR 			(APB1PERIPH_BASE + 0x3800)
#define SPI3_BASEADDR 			(APB1PERIPH_BASE + 0x3C00)
#define USART2_BASEADDR			(APB1PERIPH_BASE + 0x4400)
#define USART3_BASEADDR			(APB1PERIPH_BASE + 0x4800)
#define UART4_BASEADDR			(APB1PERIPH_BASE + 0x4C00)
#define UART5_BASEADDR			(APB1PERIPH_BASE + 0x5000)

#define EXTI_BASEADDR 			(APB2PERIPH_BASE + 0x0400)
#define SPI1_BASEADDR 			(APB2PERIPH_BASE + 0x3000)
#define USART1_BASEADDR			(APB2PERIPH_BASE + 0x3800)
#define SYSCFG_BASEADDR			(APB2PERIPH_BASE + 0x0000)

// periferal register definition structure
typedef struct {
	__vo uint32_t MODER;				// GPIO port mode register
	__vo uint32_t OTYPER;				// GPIO port output type register
	__vo uint32_t OSPEEDR; 				// GPIO port output speed register
	__vo uint32_t PUPDR;				// GPIO port pull-up/pull-down register
	__vo uint32_t IDR;					// GPIO port input data register
	__vo uint32_t ODR;					// GPIO port output data register
	__vo uint32_t BSRR;					// GPIO port bit set/reset register
	__vo uint32_t LCKR;					// GPIO port configuration lock register
	__vo uint32_t AFR[2];				// GPIO alternate function [0] - low, [1] - high register
	__vo uint32_t BRR;					// GPIO port bit reset register
} GPIO_RegDef_t;

typedef struct {
	__vo uint32_t CR;
	__vo uint32_t CFGR;
	__vo uint32_t CIR;
	__vo uint32_t APB2RSTR;
	__vo uint32_t APB1RSTR;
	__vo uint32_t AHBENR;
	__vo uint32_t APB2ENR;
	__vo uint32_t APB1ENR;
	__vo uint32_t BDCR;
	__vo uint32_t CSR;
	__vo uint32_t AHBRSTR;
	__vo uint32_t CFGR2;
	__vo uint32_t CFGR3;
} RCC_RegDef_t;

typedef struct {
	__vo uint32_t IMR1;
	__vo uint32_t EMR1;
	__vo uint32_t RTSR1;
	__vo uint32_t FTSR1;
	__vo uint32_t SWIER1;
	__vo uint32_t PR1;
	__vo uint32_t IMR2;
	__vo uint32_t EMR2;
	__vo uint32_t RTSR2;
	__vo uint32_t FTSR2;
	__vo uint32_t SWIER2;
	__vo uint32_t PR2;
} EXTI_RegDef_t;

typedef struct {
	__vo uint32_t RCR;
	__vo uint32_t EXTICR[4];
	__vo uint32_t CFGR[3];
} SYSCFG_RegDef_t;

#define GPIOA	((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB	((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC	((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD	((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE	((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF	((GPIO_RegDef_t*)GPIOF_BASEADDR)

#define RCC			((RCC_RegDef_t*)RCC_BASEADDR)
#define EXTI 			((EXTI_RegDef_t*)EXTI_BASEADDR)
#define SYSCFG 		((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

#define GPIOA_PCLK_EN()		( RCC->AHBENR |= (1 << 17) ) // GPIOA peripheral clock enable
#define GPIOB_PCLK_EN()		( RCC->AHBENR |= (1 << 18) ) // GPIOB peripheral clock enable
#define GPIOC_PCLK_EN()		( RCC->AHBENR |= (1 << 19) ) // GPIOC peripheral clock enable
#define GPIOD_PCLK_EN()		( RCC->AHBENR |= (1 << 20) ) // GPIOD peripheral clock enable
#define GPIOE_PCLK_EN()		( RCC->AHBENR |= (1 << 21) ) // GPIOE peripheral clock enable
#define GPIOF_PCLK_EN()		( RCC->AHBENR |= (1 << 22) ) // GPIOF peripheral clock enable

#define I2C1_PCLK_EN()		( RCC->APB1ENR |= (1 << 21) ) // I2C1 peripheral clock enable
#define I2C2_PCLK_EN()		( RCC->APB1ENR |= (1 << 22) ) // I2C2 peripheral clock enable

#define SPI2_PCLK_EN()		( RCC->APB1ENR |= (1 << 14) ) // I2SPI2 peripheral clock enable
#define SPI3_PCLK_EN()		( RCC->APB1ENR |= (1 << 15) ) // I2SPI3 peripheral clock enable

#define USART2_PCLK_EN()	( RCC->APB1ENR |= (1 << 17) ) // USART2 peripheral clock enable
#define USART3_PCLK_EN()	( RCC->APB1ENR |= (1 << 18) ) // USART3 peripheral clock enable

#define UART4_PCLK_EN() 	( RCC->APB1ENR |= (1 << 19) ) // UART4 peripheral clock enable
#define UART5_PCLK_EN()	    ( RCC->APB1ENR |= (1 << 20) ) // UART5 peripheral clock enable

#define SPI1_PCLK_EN()		( RCC->APB2ENR |= (1 << 12) ) // SPI1 peripheral clock enable
#define USART1_PCLK_EN()	( RCC->APB2ENR |= (1 << 14) ) // USARTI1 peripheral clock enable
#define SYSCFG_PCLK_EN()	( RCC->APB2ENR |= (1 << 0) )  // SYSCFG peripheral clock enable

#define GPIOA_PCLK_DI()		( RCC->AHBENR &= ~(1 << 17) ) // GPIOA peripheral clock enable
#define GPIOB_PCLK_DI()		( RCC->AHBENR &= ~(1 << 18) ) // GPIOB peripheral clock enable
#define GPIOC_PCLK_DI()		( RCC->AHBENR &= ~(1 << 19) ) // GPIOC peripheral clock enable
#define GPIOD_PCLK_DI()		( RCC->AHBENR &= ~(1 << 20) ) // GPIOD peripheral clock enable
#define GPIOE_PCLK_DI()		( RCC->AHBENR &= ~(1 << 21) ) // GPIOE peripheral clock enable
#define GPIOF_PCLK_DI()		( RCC->AHBENR &= ~(1 << 22) ) // GPIOF peripheral clock enable

#define I2C1_PCLK_DI()		( RCC->APB1ENR &= ~(1 << 21) ) // I2C1 peripheral clock enable
#define I2C2_PCLK_DI()		( RCC->APB1ENR &= ~(1 << 22) ) // I2C2 peripheral clock enable

#define SPI2_PCLK_DI()		( RCC->APB1ENR &= ~(1 << 14) ) // I2SPI2 peripheral clock enable
#define SPI3_PCLK_DI()		( RCC->APB1ENR &= ~(1 << 15) ) // I2SPI3 peripheral clock enable

#define USART2_PCLK_DI()	( RCC->APB1ENR &= ~(1 << 17) ) // USART2 peripheral clock enable
#define USART3_PCLK_DI()	( RCC->APB1ENR &= ~(1 << 18) ) // USART3 peripheral clock enable

#define UART4_PCLK_DI() 	( RCC->APB1ENR &= ~(1 << 19) ) // UART4 peripheral clock enable
#define UART5_PCLK_DI()	    ( RCC->APB1ENR &= ~(1 << 20) ) // UART5 peripheral clock enable

#define SPI1_PCLK_DI()		( RCC->APB2ENR &= ~(1 << 12) ) // SPI1 peripheral clock enable
#define USART1_PCLK_DI()	( RCC->APB2ENR &= ~(1 << 14) ) // USARTI1 peripheral clock enable
#define SYSCFG_PCLK_DI()	( RCC->APB2ENR &= ~(1 << 0) )  // SYSCFGperipheral clock enable

#define GPIOA_REG_RESET()	do { (RCC->AHBRSTR |= (1 << 17)); (RCC->AHBRSTR &= ~(1 << 17)); } while (0)
#define GPIOB_REG_RESET()	do { (RCC->AHBRSTR |= (1 << 18)); (RCC->AHBRSTR &= ~(1 << 18)); } while (0)
#define GPIOC_REG_RESET()	do { (RCC->AHBRSTR |= (1 << 19)); (RCC->AHBRSTR &= ~(1 << 19)); } while (0)
#define GPIOD_REG_RESET()	do { (RCC->AHBRSTR |= (1 << 20)); (RCC->AHBRSTR &= ~(1 << 20)); } while (0)
#define GPIOE_REG_RESET()	do { (RCC->AHBRSTR |= (1 << 21)); (RCC->AHBRSTR &= ~(1 << 21)); } while (0)
#define GPIOF_REG_RESET()	do { (RCC->AHBRSTR |= (1 << 22)); (RCC->AHBRSTR &= ~(1 << 22)); } while (0)

#define ENABLE 			1
#define DISABLE 		0
#define SET 			ENABLE
#define RESET 			DISABLE
#define GPIO_PIN_SET 	SET
#define GPIO_PIN_RESET 	RESET

#define GPIO_BASEADDR_TO_CODE(x)		(x == GPIOA) ? 0 :\
										(x == GPIOB) ? 1 :\
										(x == GPIOC) ? 2 :\
										(x == GPIOD) ? 3 :\
										(x == GPIOE) ? 4 :\
										(x == GPIOF) ? 5 : 0

#define IRQ_NO_EXTI0			6
#define IRQ_NO_EXTI1			7
#define IRQ_NO_EXTI2			8
#define IRQ_NO_EXTI3			9
#define IRQ_NO_EXTI4			10
#define IRQ_NO_EXTI9_5		23
#define IRQ_NO_EXTI15_10		40

#define NVIC_IRQ_PRIO0		0
#define NVIC_IRQ_PRIO12		12
#define NVIC_IRQ_PRIO15		15

#endif /* INC_STM32F303XX_H_ */
