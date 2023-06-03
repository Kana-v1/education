/*
 * stm32f303xx_i2c_driver.c
 *
 *  Created on: May 30, 2023
 *      Author: Kana
 */

#include "stm32f303xx.h"

static void I2C_MasterAddressPhaseInit(I2C_Handle_t *pI2CHandle, uint32_t dataLength, uint8_t slaveAddr, uint8_t readWrite);
static void I2C_GenerateStart(I2C_RegDef_t *pI2Cx);
static void I2C_GenerateStop(I2C_RegDef_t *pI2Cx);

static void I2C_MasterAddressPhaseInit(I2C_Handle_t *pI2CHandle, uint32_t dataLength, uint8_t slaveAddr, uint8_t readWrite) {
	uint32_t tempreg = 0;
	// 1. set addressing mode
	tempreg |= (0 << I2C_CR2_ADD10);

	// 2. set slave address
	tempreg |= (slaveAddr << I2C_CR2_SADD);

	// 3. set number of bytes to be addressed
	tempreg |= (dataLength << I2C_CR2_NBYTES);

	// 4. set read or write master mode
	tempreg |= (readWrite << I2C_CR2_RD_WRN);

	pI2CHandle->pI2Cx->CR2 |= tempreg;
}

static void I2C_GenerateStart(I2C_RegDef_t *pI2Cx) {
   pI2Cx->CR2 |= (1 << I2C_CR2_START);
}

static void I2C_GenerateStop(I2C_RegDef_t *pI2Cx) {
   pI2Cx->CR1 |= (1 << I2C_CR2_STOP);
}

uint16_t AHB_PreScaler[8] = { 2, 4, 8, 16, 64, 128, 256, 512 };
uint16_t APB1_PreScaler[8] = { 2, 4, 8, 16 };

uint32_t RCC_GetPLLOutputClock(void) {
	return 16000000;
}

// stm32f303 I2C is clocked by an independent clock source which allows to the I2C to operate
// independently from the PCLK frequency
// for clock source can be used either HSI(default value) or SYSCLK
uint32_t I2C_CLK(void) {
	uint32_t pclk1, systemClk, ahbp, apb1p;

	uint8_t clkSrc = (RCC->CFGR >> 2) & 0b0011;

	if (clkSrc == 0) {
		// HSI (default value. should be chosen since we didn't change clock source)
		systemClk = 16000000; // 16 MHz
	} else if (clkSrc == 1) {
		// HSE
		systemClk = 8000000; // 8 MHz
	} else if (clkSrc == 2) {
		// PLL
		systemClk = RCC_GetPLLOutputClock();
	}

	// ahb
	uint8_t temp = (RCC->CFGR >> 4) & 0b1111; // mask all bits except first 4

	if (temp < 0b1000) {
		ahbp = 1;
	} else {
		ahbp = AHB_PreScaler[temp - 8]; // 8 == 0n1000
	}

	// apb1
	temp = (RCC->CFGR >> 8) & 0b111; // mask all bits except first 3

	if (temp < 0b100) {
		apb1p = 1;
	} else {
		apb1p = APB1_PreScaler[temp - 4]; // 4 == 0n100
	}

	pclk1 = systemClk / ahbp / apb1p;

	return pclk1;
}

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi) {
	if (EnOrDi == ENABLE) {
		if (pI2Cx == I2C1) {
			I2C1_PCLK_EN();
		} else if (pI2Cx == I2C2) {
			I2C2_PCLK_EN();
		}
	} else {
		if (pI2Cx == I2C1) {
			I2C1_PCLK_EN();
		} else if (pI2Cx == I2C2) {
			I2C2_PCLK_DI();
		}
	}
}

void I2C_Init(I2C_Handle_t *pI2CHandle) {
	uint32_t tempreg = 0;

	// config has to be set with 0 in the PE bit of the CR1 register

	// enable acknowledge
	tempreg |= pI2CHandle->I2C_Config.I2C_AckControl << I2C_CR1_SBC;
	pI2CHandle->pI2Cx->CR1 = tempreg;

	tempreg = 0;
	tempreg |= ~pI2CHandle->I2C_Config.I2C_AckControl << I2C_CR2_NACK;
	pI2CHandle->pI2Cx->CR2 = tempreg;

	// program the device own address
	tempreg = 0;
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << I2C_OAR1_OA1;
	tempreg |= pI2CHandle->I2C_Config.I2C_AckControl << I2C_OAR1_OA1EN;
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	// config timings
	// all config below are for the 100kHz timing, and fI2CCLK = 8MHz. Configs were taken from the user manual
	tempreg = 0;
	tempreg |= 1 << I2C_TIMINGR_PRESC;
	tempreg |= 0x13 << I2C_TIMINGR_SCLL;
	tempreg |= 0xF << I2C_TIMINGR_SCLH;
	tempreg |= 0x2 << I2C_TIMINGR_SDADEL;

	pI2CHandle->pI2Cx->TIMINGR = tempreg;
}

void I2C_DeInit(I2C_RegDef_t *pI2Cx) {
	if (pI2Cx == I2C1) {
		RCC->APB1RSTR |= (1 << I2C1_RESET_OFFSET);
	} else {
		RCC->APB1RSTR |= (1 << I2C2_RESET_OFFSET);
	}
}

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t enOrDi) {
	if (enOrDi == ENABLE) {
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
	} else {
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
	}
}

// this method is implemented only for the data length <= 255 bytes
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t length, uint8_t slaveAddr) {
	// 1. address phase
	I2C_MasterAddressPhaseInit(pI2CHandle, length, slaveAddr, I2C_MASTER_WRITE_MODE);

	// 2. generate start condition
	I2C_GenerateStart(pI2CHandle->pI2Cx);

	// 3. wait until bus is free to use
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_ISR_BUSY));
	I2C_PeripheralControl(pI2CHandle->pI2Cx, ENABLE);

	// 4. wait until data register is empty
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_ISR_TXIS));

	// 5. send data
	pI2CHandle->pI2Cx->TXDR = *pTxBuffer;

	// 6. wait until transfer is complete
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_ISR_TC));

	// 7. generate stop
	I2C_GenerateStop(pI2CHandle->pI2Cx);

	// 8. release peripheral
	I2C_PeripheralControl(pI2CHandle->pI2Cx, DISABLE);
}


uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t flagName) {
	if (pI2Cx->ISR & flagName) {
		return FLAG_SET;
	}

	return FLAG_RESET;
}


// IRQ Configuration and ISR handling
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi) {
	// it is in the processor part, so we should check processor user reference
	if (EnOrDi == ENABLE) {
		if (IRQNumber <= 31) {
			// program ISER0 register
			*NVIC_ISER0 |= (1 << IRQNumber);
		} else if (IRQNumber > 31 && IRQNumber < 64) {
			// program ISER1 register
			*NVIC_ISER1 |= (1 << (IRQNumber % 32));
		} else if (IRQNumber >= 64 && IRQNumber < 96) {
			// program ISER2 register
			*NVIC_ISER2 |= (1 << (IRQNumber % 64));
		}
	} else {
		if (IRQNumber <= 31) {
			// program ICER0 register
			*NVIC_ICER0 |= (1 << IRQNumber);
		} else if (IRQNumber > 31 && IRQNumber < 64) {
			// program ICER1 register
			*NVIC_ICER1 |= (1 << (IRQNumber % 32));
		} else if (IRQNumber >= 64 && IRQNumber < 96) {
			// program ICER2 register
			*NVIC_ICER2 |= (1 << (IRQNumber % 64));
		}
	}
}

void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {
	// 1. Find out the ipr register
	uint8_t iprxOffset = IRQNumber / 4; // offset of the priority register. 4 - cuz each register is 32 bits
	uint8_t iprxSection = IRQNumber % 4 * 8; // each register has 4 blocks, each block has 8 bits

	uint8_t shiftAmount = iprxSection + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprxOffset) |= (IRQPriority << shiftAmount);
}





