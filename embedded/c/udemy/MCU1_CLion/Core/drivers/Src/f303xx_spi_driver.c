/*
 * f303xx_spi_driver.c
 *
 *  Created on: May 9, 2023
 *      Author: Kana
 */

// peripheral clock setup
#include "stm32f303xx_spi_driver.h"

static void spiTxeInterruptHandle(SPI_Handle_t *pSPIHandle);
static void spiRxneInterruptHandle(SPI_Handle_t *pSPIHandle);
static void spiOvrErrInterruptHandle(SPI_Handle_t *pSPIHandle);

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
		// 1. wait until data has come to the Tx register (Txe is reset)
		while (SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET)
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
		while (SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_SET)
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

void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {
	// 1. Find out the ipr register
	uint8_t iprxOffset = IRQNumber / 4; // offset of the priority register. 4 - cuz each register is 32 bits
	uint8_t iprxSection = IRQNumber % 4 * 8; // each register has 4 blocks, each block has 8 bits

	uint8_t shiftAmount = iprxSection + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprxOffset) |= (IRQPriority << shiftAmount);
}

// ISR - interrupt service routine
// IRQ - interrupt request
void SPI_IRQHandling(SPI_Handle_t *pHandle) {
	uint8_t tmp1, tmp2;
	// 1. check the interrupt cause (tx/rx/error)

	// check TXE flag
	tmp1 = pHandle->pSPIx->SR & (1 << SPI_SR_TXE_OFFSET);
	tmp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if (tmp1 && tmp2) {
		// handle TXE
		spiTxeInterruptHandle(pHandle);
		return;
	}

	// check RXNE flag
	tmp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE_OFFSET);
	tmp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if (tmp1 && tmp2) {
		// handle RXNE
		spiRxneInterruptHandle(pHandle);
		return;
	}

	// this time we care only about the overrun error
	tmp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR_OFFSET);
	tmp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

	if (tmp1 && tmp2) {
		// handle TXE
		spiOvrErrInterruptHandle(pHandle);
	}
}

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *dataPtr, uint32_t length) {
	uint8_t state = pSPIHandle->TxState;
	if (state == SPI_BUSY_IN_TX) {
		return state;
	}

	// 1. Save the Tx buffer address and length information in some global variables
	pSPIHandle->pTxBuffer = dataPtr;
	pSPIHandle->TxLength = length;

	// 2. Mark the SPI state as busy in transmission
	// so that no other code can take over same SPI peripheral until transmission is over
	pSPIHandle->TxState = SPI_BUSY_IN_TX;

	// 3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
	pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);

	// 4. Data transmission will be handled by the ISR code (will be implemented later)
	return state;
}
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *rxBufPtr, uint32_t length) {
	uint8_t state = pSPIHandle->RxState;
	if (state == SPI_BUSY_IN_RX) {
		return state;
	}

	// 1. Save the Rx buffer address and length information in some global variables
	pSPIHandle->pRxBuffer = rxBufPtr;
	pSPIHandle->RxLength = length;

	// 2. Mark the SPI state as busy in transmission
	// so that no other code can take over same SPI peripheral until transmission is over
	pSPIHandle->RxState = SPI_BUSY_IN_RX;

	// 3. Enable the TXEIE control bit to get interrupt whenever RXNEIE flag is set in SR
	pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);

	// 4. Data transmission will be handled by the ISR code (will be implemented later)
	return state;
}

static void spiTxeInterruptHandle(SPI_Handle_t *pSPIHandle) {
	// check the DFF bit in CR1
	if ((pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))) {
		//16 bit DFF
		//1. load the data in to the DR
		pSPIHandle->pSPIx->DR = *((uint16_t*) pSPIHandle->pTxBuffer);
		pSPIHandle->TxLength -= 2;
		(uint16_t*) pSPIHandle->pTxBuffer++;
	} else {
		//8 bit DFF
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLength--;
		pSPIHandle->pTxBuffer++;
	}

	if (pSPIHandle->TxLength == 0) {
		// 1. close the SPI communication
		SPI_CloseTransmission(pSPIHandle);
		// 2. inform the application that tx is over
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_COMPLETE);
	}
}
static void spiRxneInterruptHandle(SPI_Handle_t *pSPIHandle) {
	//do rxing as per the dff
	if (pSPIHandle->pSPIx->CR1 & (1 << 11)) {
		//16 bit
		*((uint16_t*) pSPIHandle->pRxBuffer) = (uint16_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLength -= 2;
		pSPIHandle->pRxBuffer++;
		pSPIHandle->pRxBuffer++;

	} else {
		//8 bit
		*(pSPIHandle->pRxBuffer) = (uint8_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLength--;
		pSPIHandle->pRxBuffer++;
	}

	if (pSPIHandle->RxLength == 0) {
		// 1. close the SPI communication
		SPI_CloseReception(pSPIHandle);

		// 2. inform the application that tx is over
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_COMPLETE);
	}
}
static void spiOvrErrInterruptHandle(SPI_Handle_t *pSPIHandle) {
	uint8_t tmp;

	// 1. clear the ovr flag
	if (pSPIHandle->TxState != SPI_BUSY_IN_TX) {
		tmp = pSPIHandle->pSPIx->DR;
		tmp = pSPIHandle->pSPIx->SR;
	}

	(void) tmp;

	// 2. inform the application about the error
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);

}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx) {
	uint8_t tmp;
	tmp = pSPIx->DR;
	tmp = pSPIx->SR;

	(void) tmp;
}
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle) {
	// this prevents interrupt from setting up of TXE flag
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLength = 0;
	pSPIHandle->TxState = SPI_READY;
}
void SPI_CloseReception(SPI_Handle_t *pSPIHandle) {
	// this prevents interrupt from setting up of RXNEIE flag
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLength = 0;
	pSPIHandle->RxState = SPI_READY;
}

__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t event) {
	// weak implementation. The application may override this function
}
