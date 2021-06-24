/*
 * i2c.c
 *
 *  Created on: 23 jun. 2021
 *      Author: Braulio
 */
 /* Variables
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "fsl_i2c.h"
#include "i2c.h"

uint8_t g_master_hour_txBuff[I2C_DATA_LENGTH];
uint8_t g_master_hour_rxBuff[I2C_DATA_LENGTH];

i2c_master_config_t masterConfig;
uint32_t sourceClock;
i2c_master_transfer_t masterXfer;

void i2c_init(void)
{
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();

	/*
	* masterConfig->baudRate_Bps = 100000U;
	* masterConfig->enableStopHold = false;
	* masterConfig->glitchFilterWidth = 0U;
	* masterConfig->enableMaster = true;
	*/
	I2C_MasterGetDefaultConfig(&masterConfig);
	masterConfig.baudRate_Bps = I2C_BAUDRATE;

	sourceClock = CLOCK_GetFreq(kCLOCK_BusClk);

	I2C_MasterInit(I2C0, &masterConfig, sourceClock);

	memset(&masterXfer, 0, sizeof(masterXfer));

}

void transfer_data(void)
{
	/* subAddress = 0x01, data = g_master_txBuff - write to slave.
	start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
	uint8_t deviceAddress     = 0x01U;
	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Write;
	masterXfer.subaddress     = 0x00;
	masterXfer.subaddressSize = 1;
	masterXfer.data           = g_master_hour_txBuff;
	masterXfer.dataSize       = I2C_DATA_LENGTH;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);
}
void receipt_data(void)
{
	/* subAddress = 0x01, data = g_master_rxBuff - read from slave.
	start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Read;
	masterXfer.subaddress     = 0x00;
	masterXfer.subaddressSize = 1;
	masterXfer.data           = g_master_hour_rxBuff;
	masterXfer.dataSize       = I2C_DATA_LENGTH;  //tamaño de la trama de datos
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

}

