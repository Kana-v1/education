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
		} else if (pSPIx == SPI2) {
			SPI2_PCLK_EN();
		}
	} else {
		if (pSPIx == SPI1) {
			SPI1_PCLK_DI();
		} else if (pSPIx == SPI2) {
			SPI2_PCLK_DI();
		}
	}
}

void SPI_SSIConfig(SPI_RegDef_t *pSPI, uint8_t enOrDi) {
	if (enOrDi == ENABLE) {
		pSPI->CR1 |= (1 << SPI_CR1_SSI);
	} else {
		pSPI->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

void SPI_Init(SPI_Handle_t *pSPIHandle) {
	// peripheral clock enable
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);
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
	tempReg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	// 4. configure the DFF
	tempReg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	// 5. configure the CPOL
	tempReg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	// 6. configure the CPHA
	tempReg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	// 7. configure the SSM
	tempReg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	pSPIHandle->pSPIx->CR1 = tempReg;
}

void SPI_DeInit(SPI_RegDef_t *pSPIx) {

}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t flagName) {
	if (pSPIx->SR & flagName) {
		return FLAG_SET;
	}

	return FLAG_RESET;
}

void SPI_SSOEConfig(SPI_RegDef_t *pSPI, uint8_t enOrDi) {
	if (enOrDi == ENABLE) {
		pSPI->CR2 |= (1 << SPI_CR2_SSOI);
	} else {
		pSPI->CR2 &= ~(1 << SPI_CR2_SSOI);
	}
}

// data send and receive
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *dataPtr, uint32_t length) {
	while (length > 0) {
		// 1. wait until data has come to the Tx register (Txe is set)
		while (SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_SET) // was FLAG_RESET
			;

		// 2. check the DFF bit in CR1
		if (pSPIx->CR1 & (1 << SPI_CR1_DFF)) {
			// 16 bit data format
			// 1. load the data into the DR
			pSPIx->DR = *((uint16_t*) dataPtr);
			length -= 2;
			(uint16_t*) dataPtr++;
		} else {
			// 8 bit data format
			pSPIx->DR = *dataPtr;
			length--;
			dataPtr++;
		}
	}
}

void SPI_PeripheralControl(SPI_RegDef_t *pSPI, uint8_t enOrDi) {
	if (enOrDi == ENABLE) {
		pSPI->CR1 |= (1 << SPI_CR1_SPE);
	} else {
		pSPI->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *rxBufPtr, uint32_t length) {
	while (length > 0) {
		// 1. wait until data has come to the RXNE register (RXNE is set)
		while (SPI_GetFlagStatus(pSPIx, SPI_TXE_RXNE) == FLAG_SET)
			;

		// 2. check the DFF bit in CR1
		if (pSPIx->CR1 & (1 << SPI_CR1_DFF)) {
			// 16 bit data format
			// 1. read the data from the DR
			*((uint16_t*) rxBufPtr) = pSPIx->DR;
			length -= 2;
			(uint16_t*) rxBufPtr++;
		} else {
			// 8 bit data format
			*rxBufPtr = pSPIx->DR;
			length--;
			rxBufPtr++;
		}
	}
}

// IRQ Configuration and ISR handling
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi) {

}

void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {

}

void SPI_IRQHandling(SPI_Handle_t *pHandle) {

}
