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

uint8_t h1 = 20;

uint8_t hour_txBuff[I2C_DATA_LENGTH];
uint8_t hour_rxBuff[I2C_DATA_LENGTH];
uint8_t min_txBuff[I2C_DATA_LENGTH];
uint8_t min_rxBuff[I2C_DATA_LENGTH];
uint8_t sec_txBuff[I2C_DATA_LENGTH];
uint8_t sec_rxBuff[I2C_DATA_LENGTH];

uint8_t date_txBuff[I2C_DATA_LENGTH];
uint8_t date_rxBuff[I2C_DATA_LENGTH];
uint8_t month_txBuff[I2C_DATA_LENGTH];
uint8_t month_rxBuff[I2C_DATA_LENGTH];
uint8_t year_txBuff[I2C_DATA_LENGTH];
uint8_t year_rxBuff[I2C_DATA_LENGTH];

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

void transfer_hour(uint8_t h)
{
	/* subAddress = 0x01, data = g_master_txBuff - write to slave.
	start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
	hour_txBuff[0] = h;
	uint8_t deviceAddress     = 0x01U;
	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Write;
	masterXfer.subaddress     = 0x02;          //Registro del RTC para las horas
	masterXfer.subaddressSize = 1;
	masterXfer.data           = hour_txBuff;
	masterXfer.dataSize       = 1; //I2C_DATA_LENGTH;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);
}
uint8_t receipt_hour(void)
{
	/* subAddress = 0x01, data = g_master_rxBuff - read from slave.
	start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Read;
	masterXfer.subaddress     = 0x02;        //Registro para las horas del RTC
	masterXfer.subaddressSize = 1;
	masterXfer.data           = hour_rxBuff;
	masterXfer.dataSize       = I2C_DATA_LENGTH;  //tamaño de la trama de datos
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	uint8_t p = *(hour_rxBuff);
	return p;

}


void transfer_min(uint8_t m)
{
	/* subAddress = 0x01, data = g_master_txBuff - write to slave.
	start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
	min_txBuff[0] = m;
	uint8_t deviceAddress     = 0x01U;
	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Write;
	masterXfer.subaddress     = 0x01;          //Registro del RTC para los minutos
	masterXfer.subaddressSize = 1;
	masterXfer.data           = min_txBuff;
	masterXfer.dataSize       = 1; //I2C_DATA_LENGTH;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);
}
uint8_t receipt_min(void)
{
	/* subAddress = 0x01, data = g_master_rxBuff - read from slave.
	start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Read;
	masterXfer.subaddress     = 0x01;        //Registro para los minutos del RTC
	masterXfer.subaddressSize = 1;
	masterXfer.data           = min_rxBuff;
	masterXfer.dataSize       = I2C_DATA_LENGTH;  //tamaño de la trama de datos
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	uint8_t p = *(min_rxBuff);
	return p;
}


void transfer_sec(uint8_t s)
{
	/* subAddress = 0x01, data = g_master_txBuff - write to slave.
	start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
	min_txBuff[0] = s;
	uint8_t deviceAddress     = 0x01U;
	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Write;
	masterXfer.subaddress     = 0x00;          //Registro del RTC para los segundos
	masterXfer.subaddressSize = 1;
	masterXfer.data           = min_txBuff;
	masterXfer.dataSize       = 1; //I2C_DATA_LENGTH;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);
}
uint8_t receipt_sec(void)
{
	/* subAddress = 0x01, data = g_master_rxBuff - read from slave.
	start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Read;
	masterXfer.subaddress     = 0x00;        //Registro para los segundos
	masterXfer.subaddressSize = 1;
	masterXfer.data           = sec_rxBuff;
	masterXfer.dataSize       = I2C_DATA_LENGTH;  //tamaño de la trama de datos
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	uint8_t p = *(sec_rxBuff);
	return p;
}



void hour_register_config()
{
	/* subAddress = 0x01, data = g_master_txBuff - write to slave.
		start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
		uint8_t deviceAddress     = 0x01U;
		masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
		masterXfer.direction      = kI2C_Write;
		masterXfer.subaddress     = 0x02;                         //Registro a modificar - Horas
		masterXfer.subaddressSize = 1;                            //Tamaño en bytes del address
		masterXfer.data           = 0 << 6;                         //0000000 formato a 24 hrs
		masterXfer.dataSize       = 1;
		masterXfer.flags          = kI2C_TransferDefaultFlag;

		I2C_MasterTransferBlocking(I2C0, &masterXfer);

}


void set_hora(uint8_t hora)
{
//	uint8_t hour_txBuff[I2C_DATA_LENGTH] = {h};
	h1 = hora;
}


void transfer_date(uint8_t d)
{
	/* subAddress = 0x01, data = g_master_txBuff - write to slave.
	start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
	date_txBuff[0] = d;
	uint8_t deviceAddress     = 0x01U;
	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Write;
	masterXfer.subaddress     = 0x04;          //Registro del RTC para los días del mes (01-31)
	masterXfer.subaddressSize = 1;
	masterXfer.data           = date_txBuff;
	masterXfer.dataSize       = 1; //I2C_DATA_LENGTH;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);
}
void transfer_month(uint8_t m)
{
	/* subAddress = 0x01, data = g_master_txBuff - write to slave.
	start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
	month_txBuff[0] = m;
	uint8_t deviceAddress     = 0x01U;
	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Write;
	masterXfer.subaddress     = 0x05;          //Registro del RTC para los meses (01-12)
	masterXfer.subaddressSize = 1;
	masterXfer.data           = month_txBuff;
	masterXfer.dataSize       = 1; //I2C_DATA_LENGTH;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

}
void transfer_year(uint8_t y)
{
	/* subAddress = 0x01, data = g_master_txBuff - write to slave.
	start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
	year_txBuff[0] = y;
	uint8_t deviceAddress     = 0x01U;
	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Write;
	masterXfer.subaddress     = 0x06;          //Registro del RTC para los años (00-99)
	masterXfer.subaddressSize = 1;
	masterXfer.data           = year_txBuff;
	masterXfer.dataSize       = 1; //I2C_DATA_LENGTH;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);
}


uint8_t receipt_date(void)
{
	/* subAddress = 0x01, data = g_master_rxBuff - read from slave.
	start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Read;
	masterXfer.subaddress     = 0x04;        //Registro para los dias del mes
	masterXfer.subaddressSize = 1;
	masterXfer.data           = date_rxBuff;
	masterXfer.dataSize       = I2C_DATA_LENGTH;  //tamaño de la trama de datos
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	uint8_t p = *(date_rxBuff);
	return p;
}
uint8_t receipt_month(void)
{
	/* subAddress = 0x01, data = g_master_rxBuff - read from slave.
	start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Read;
	masterXfer.subaddress     = 0x05;        //Registro para los meses
	masterXfer.subaddressSize = 1;
	masterXfer.data           = month_rxBuff;
	masterXfer.dataSize       = I2C_DATA_LENGTH;  //tamaño de la trama de datos
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	uint8_t p = *(month_rxBuff);
	return p;
}
uint8_t receipt_year(void)
{
	/* subAddress = 0x01, data = g_master_rxBuff - read from slave.
	start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Read;
	masterXfer.subaddress     = 0x06;        //Registro para los segundos
	masterXfer.subaddressSize = 1;
	masterXfer.data           = year_rxBuff;
	masterXfer.dataSize       = I2C_DATA_LENGTH;  //tamaño de la trama de datos
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	uint8_t p = *(year_rxBuff);
	return p;
}



uint8_t get_rxBuff()
{
	return hour_rxBuff[0];
}




