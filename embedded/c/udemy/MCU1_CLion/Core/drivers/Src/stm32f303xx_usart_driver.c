//
// Created by Kana on 6/4/2023.
//

#include "stm32f303xx_usart_driver.h"

void USART_Init(USART_Handle_t *pUSARTHandle) {

    //Temporary variable
    uint32_t tempreg = 0;

/******************************** Configuration of CR1******************************************/

    //Implement the code to enable the Clock for given USART peripheral
    USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

    //Enable USART Tx and Rx engines according to the USART_Mode configuration item
    if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX) {
        //Implement the code to enable the Receiver bit field
        tempreg |= (1 << USART_CR1_RE);
    } else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX) {
        //Implement the code to enable the Transmitter bit field
        tempreg |= (1 << USART_CR1_TE);

    } else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX) {
        //Implement the code to enable the both Transmitter and Receiver bit fields
        tempreg |= ((1 << USART_CR1_TE) | (1 << USART_CR1_RE));
    }

    //Implement the code to configure the Word length configuration item
    tempreg |= pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M1;


    //Configuration of parity control bit fields
    if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN) {
        //Implement the code to enale the parity control
        tempreg |= (1 << USART_CR1_PCE);

        //Implement the code to enable EVEN parity
        //Not required because by default EVEN parity will be selected once you enable the parity control

    } else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD) {
        //Implement the code to enable the parity control
        tempreg |= (1 << USART_CR1_PCE);

        //Implement the code to enable ODD parity
        tempreg |= (1 << USART_CR1_PS);

    }

    //Program the CR1 register
    pUSARTHandle->pUSARTx->CR1 = tempreg;

/******************************** Configuration of CR2******************************************/

    tempreg = 0;

    //number of stop bits inserted during USART frame transmission
    tempreg |= pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP;

    //Program the CR2 register
    pUSARTHandle->pUSARTx->CR2 = tempreg;

/******************************** Configuration of CR3******************************************/

    tempreg = 0;

    //Configuration of USART hardware flow control
    if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS) {
        //Implement the code to enable CTS flow control
        tempreg |= (1 << USART_CR3_CTSE);


    } else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS) {
        //Implement the code to enable RTS flow control
        tempreg |= (1 << USART_CR3_RTSE);

    } else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS) {
        //Implement the code to enable both CTS and RTS Flow control
        tempreg |= (1 << USART_CR3_RTSE) | (1 << USART_CR3_CTSE);
    }

    pUSARTHandle->pUSARTx->CR3 = tempreg;
}


void USART_DeInit(USART_RegDef_t *pUSARTpx) {
    if (pUSARTpx == USART1) {
        RCC->APB2RSTR |= 1 << RCC_APB2_USART1RST;
    }
}

void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi) {
    if (EnOrDi == ENABLE) {
        if (pUSARTx == USART1) {
            USART1_PCLK_EN();
        }
    } else {
        if (pUSARTx == USART1) {
            USART1_PCLK_DI();
        }
    }
}

void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi) {
    pUSARTx->CR1 |= EnOrDi << USART_CR1_UE;
}

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint8_t statusFlagName) {
    if (pUSARTx->ISR & statusFlagName) {
        return FLAG_SET;
    }

    return FLAG_RESET;
}

void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint8_t statusFlagName) {
    pUSARTx->ISR &= ~statusFlagName;
}

void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi) {
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

void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {
    // 1. Find out the ipr register
    uint8_t iprxOffset = IRQNumber / 4; // offset of the priority register. 4 - cuz each register is 32 bits
    uint8_t iprxSection = IRQNumber % 4 * 8; // each register has 4 blocks, each block has 8 bits

    uint8_t shiftAmount = iprxSection + (8 - NO_PR_BITS_IMPLEMENTED);
    *(NVIC_PR_BASE_ADDR + iprxOffset) |= (IRQPriority << shiftAmount);
}

void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len) {
    uint16_t *pdata;
    //Loop over until "Len" number of bytes are transferred
    for (uint32_t i = 0; i < Len; i++) {
        // wait until TXE flag is set in the SR
        while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_ISR_TXE));

        //Check the USART_WordLength item for 9BIT or 8BIT in a frame
        if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS) {
            //if 9BIT, load the DR with 2bytes masking the bits other than first 9 bits
            pdata = (uint16_t *) pTxBuffer;
            pUSARTHandle->pUSARTx->TDR = (*pdata & (uint16_t) 0b111111111);

            //check for USART_ParityControl
            if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) {
                //No parity is used in this transfer. so, 9bits of user data will be sent
                pTxBuffer += 2;
            } else {
                //Parity bit is used in this transfer . so , 8bits of user data will be sent
                //The 9th bit will be replaced by parity bit by the hardware
                pTxBuffer++;
            }
        } else {
            //This is 8bit data transfer
            pUSARTHandle->pUSARTx->TDR = (*pTxBuffer & (uint8_t) 0b11111111);

            //Implement the code to increment the buffer address
            pTxBuffer++;
        }
    }

    //Implement the code to wait till TC flag is set in the SR
    while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_ISR_TC));
}


void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len) {
    //Loop over until "Len" number of bytes are transferred
    for (uint32_t i = 0; i < Len; i++) {
        //Implement the code to wait until RXNE flag is set in the SR
        while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_ISR_RXNE));

        //Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
        if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS) {
            //We are going to receive 9bit data in a frame

            //check are we using USART_ParityControl control or not
            if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) {
                //No parity is used. so, all 9bits will be of user data

                //read only first 9 bits. so, mask the DR with 0x01FF
                *((uint16_t *) pRxBuffer) = (pUSARTHandle->pUSARTx->RDR & (uint16_t) 0b111111111);

                //Now increment the pRxBuffer two times
                pRxBuffer += 2;
            } else {
                //Parity is used, so, 8bits will be of user data and 1 bit is parity
                *pRxBuffer = (pUSARTHandle->pUSARTx->RDR & (uint8_t) 0xFF);

                //Increment the pRxBuffer
                pRxBuffer++;
            }
        } else {
            //We are going to receive 8bit data in a frame

            //check are we using USART_ParityControl control or not
            if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) {
                //No parity is used , so all 8bits will be of user data

                //read 8 bits from DR
                *pRxBuffer = (pUSARTHandle->pUSARTx->RDR & (uint8_t) 0b11111111);
            } else {
                //Parity is used, so , 7 bits will be of user data and 1 bit is parity

                //read only 7 bits , hence mask the DR with 0X7F
                *pRxBuffer = (uint8_t) 0b1111111;

            }

            //increment the pRxBuffer
            pRxBuffer++;
        }
    }

}

uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len) {
    uint8_t txstate = pUSARTHandle->TxBusyState;

    if (txstate != USART_BUSY_IN_TX) {
        pUSARTHandle->TxLen = Len;
        pUSARTHandle->pTxBuffer = pTxBuffer;
        pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

        //enable interrupt for TXE
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TXEIE);

        //Implement the code to enable interrupt for TC
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);
    }

    return txstate;
}


uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len) {
    uint8_t rxstate = pUSARTHandle->RxBusyState;

    if (rxstate != USART_BUSY_IN_RX) {
        pUSARTHandle->RxLen = Len;
        pUSARTHandle->pRxBuffer = pRxBuffer;
        pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

        //enable interrupt for RXNE
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_EXNEIE);
    }

    return rxstate;
}


void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate) {
    //Variable to hold the APB clock
    uint32_t PCLKx;

    uint32_t usartdiv;

    //variables to hold Mantissa and Fraction values
    uint32_t M_part, F_part;

    uint32_t tempreg = 0;

    //Get the value of APB bus clock in to the variable PCLKx
    if (pUSARTx == USART1) {
        //USART1 and USART6 are hanging on APB2 bus
        PCLKx = RCC_GetPCLK2Value();
    } else {
        PCLKx = RCC_GetPCLK1Value();
    }

    //Check for OVER8 configuration bit
    if (pUSARTx->CR1 & (1 << USART_CR1_OVER8)) {
        //OVER8 = 1 , over sampling by 8
        usartdiv = PCLKx / (2 * BaudRate);
    } else {
        //over sampling by 16
        usartdiv = PCLKx / BaudRate;
    }

    //Calculate the final fractional
    if (pUSARTx->CR1 & (1 << USART_CR1_OVER8)) {
        //OVER8 = 1 , over sampling by 8
        tempreg = usartdiv;
    } else {
        //over sampling by 16
        tempreg |= (usartdiv & 0b111) >> 1; // usartdiv[3:0] shifted 1 bit to the right
        tempreg |= usartdiv << 4;
    }

    //copy the value of tempreg in to BRR register
    pUSARTx->BRR = tempreg;
}
