/*
 * uart.c
 *
 *  Created on: 23 jun. 2021
 *      Author: Braulio
 */
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_uart.h"
#include "uart.h"
#include <stdint.h>

//variables
uint8_t data = 0;
uint8_t flag1 = 0;

uint8_t hour;
uint8_t demoRingBuffer[RING_BUFFER_SIZE];
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */


void uart0_config(void)
{
	uart_config_t config;
	    BOARD_InitPins();
	    BOARD_BootClockRUN();

	    /*
	     * config.baudRate_Bps = 115200U;
	     * config.parityMode = kUART_ParityDisabled;
	     * config.stopBitCount = kUART_OneStopBit;
	     * config.txFifoWatermark = 0;
	     * config.rxFifoWatermark = 1;
	     * config.enableTx = false;
	     * config.enableRx = false;
	     */
	    UART_GetDefaultConfig(&config);
	    config.baudRate_Bps = 115200U;
	    config.enableTx     = true;
	    config.enableRx     = true;

	    UART_Init(UART0, &config, CLOCK_GetFreq(kCLOCK_CoreSysClk));


	    /* Enable RX interrupt. */
	    UART_EnableInterrupts(UART0, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
	    EnableIRQ(UART0_RX_TX_IRQn);

}


//ISR para la UART 0
void UART0_RX_TX_IRQHandler(void)
{

	 /* If new data arrived. */
	    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(UART0))
	    {
	        data = UART_ReadByte(UART0);
//	        flag1 = true;

	        /* If ring buffer is not full, add data to ring buffer. */
	        if (((rxIndex + 1) % RING_BUFFER_SIZE) != txIndex)
	        {
	            demoRingBuffer[rxIndex] = data;


	            rxIndex++;
	            rxIndex %= RING_BUFFER_SIZE;
	        }
	        flag1 = true;
	    }
	    SDK_ISR_EXIT_BARRIER;
}


void sent_data(void)
{
	/* Send data only when UART TX register is empty and ring buffer has data to send out. */
	        //while ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(UART0)) && (rxIndex != txIndex))
			if(flag1)
	        {
	            UART_WriteByte(UART0, demoRingBuffer[txIndex]);
//	            UART_WriteByte(UART0, data);

	            //data = *demoRingBuffer + txIndex;
	            flag1 = false;
	            txIndex++;
	            txIndex %= RING_BUFFER_SIZE;
	        }

}


uint16_t get_txIndex()
{
	return txIndex;
}
void set_txIndex(uint16_t dato)
{
	txIndex = dato;
}
uint8_t get_rxIndex()
{
	return rxIndex;
}
void set_rxIndex(uint16_t dato)
{
	rxIndex = dato;
}

uint8_t get_data()
{
	return data;
}
void set_data()
{
	data = false;
}

uint8_t get_hour()
{
	return hour;
}
void set_hour()
{
	hour = false;
}


uint8_t get_flag1()
{
	return flag1;
}
void set_flag1()
{
	flag1 = true;
}
void set_flag2()
{
	flag1 = false;
}

uint8_t get_ring_buffer(uint8_t index)
{
	return demoRingBuffer[index];
}

void set_ring_buffer(uint8_t index)
{
	demoRingBuffer[index] = 0;
}

void clear_buffer(void)
{
	UART_ClearStatusFlags(UART0, flag1);
}
