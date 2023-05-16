/*
 * f303xx_spi_driver.c
 *
 *  Created on: May 9, 2023
 *      Author: Kana
 */

// peripheral clock setup
#include "stm32f303xx_spi_driver.h"

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi) {
	if (EnOrDi == ENABLE) {
		if (pSPIx == SPI1) {
			SPI1_PCLK_EN();
		}
	} else {
		if (pSPIx == SPI1) {
			SPI1_PCLK_DI();
		}
	}
}

void SPI_Init(SPI_Handle_t *pSPIHandle) {
	// configure the SOU_CR1 register
	uint32_t tempReg = 0;

	// 1. configure the device mode
	tempReg |= pSPIHandle->SPIConfig.SPI_DeviceMode << 2;

	// 2. configure the bus configuration
	if (pSPIHandle->SPIConfig.SPI_DeviceBusConfig == SPI_BUS_CONFIG_FD) {
		// bidirectional mode should be cleared
		tempReg &= ~(1 << SPI_CR1_BIDIMODE);
	} else if (pSPIHandle->SPIConfig.SPI_DeviceBusConfig == SPI_BUS_CONFIG_HD) {
		// bidirectional mode should be set
		tempReg |= ~(1 << SPI_CR1_BIDIMODE);
	} else if (pSPIHandle->SPIConfig.SPI_DeviceBusConfig == SPI_BUS_CONFIG_S_RXONLY) {
		// bidirectional mode should be cleared
		tempReg &= ~(1 << SPI_CR1_BIDIMODE);
		// RXONLY bit must be set
		tempReg |= ~(1 << SPI_CR1_RXONLY);
	}

	// 3. configure the serial clock speed
	tempReg |= pSPIHandle->SPIConfig->SPI_SclkSpeed << SPI_CR1_BR;

	// 4. configure the DFF
	tempReg |= pSPIHandle->SPIConfig->SPI_DFF << SPI_CR1_DFF;

	// 5. configure the CPOL
	tempReg |= pSPIHandle->SPIConfig->SPI_CPOL << SPI_CR1_CPOL;

	// 6. configure the CPHA
	tempReg |= pSPIHandle->SPIConfig->SPI_CPHA << SPI_CR1_CPHA;

	// 7. configure the SSM
	tempReg |= pSPIHandle->SPIConfig->SPI_SSM << SPI_CR1_SSM;

	pSPIHandle->pSPIx->CR1 = tempreg;
}

void SPI_DeInit(SPI_RegDef_t *pSPIx) {

}

// data send and receive
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *dataPtr, uint32_t length) {

}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *rcBufPtr, uint32_t length) {

}

// IRQ Configuration and ISR handling
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi) {

}

void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {

}

void SPI_IRQHandling(SPI_Handle_t *pHandle) {

}
