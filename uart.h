/*
 * uart.h
 *
 *  Created on: 23 jun. 2021
 *      Author: Braulio
 */

#ifndef UART_H_
#define UART_H_
/*! @brief Ring buffer size (Unit: Byte). */
#define RING_BUFFER_SIZE 2

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include <stdint.h>
#include "fsl_uart.h"




void uart0_config(void);
void UART0_RX_TX_IRQHandler(void);

uint16_t get_txIndex();
void set_txIndex(uint16_t);
uint8_t get_rxIndex();
void set_rxIndex(uint16_t);
void sent_data(void);

uint8_t get_data();
void set_data();



#endif /* UART_H_ */
