/*
 * i2c.h
 *
 *  Created on: 23 jun. 2021
 *      Author: Braulio
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdio.h>
#include <string.h>
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "fsl_i2c.h"

/* I2C source clock */
#define WAIT_TIME                  10U
#define I2C_MASTER_SLAVE_ADDR      0x68U
#define I2C_BAUDRATE               100000U
#define I2C_DATA_LENGTH            1U

void i2c_init(void);

void transfer_hour(uint8_t);
void transfer_min(uint8_t);
void transfer_sec(uint8_t);

uint8_t receipt_min(void);
uint8_t receipt_hour(void);
uint8_t receipt_sec(void);

void transfer_date(uint8_t);
void transfer_month(uint8_t);
void transfer_year(uint8_t);

uint8_t receipt_date(void);
uint8_t receipt_month(void);
uint8_t receipt_year(void);

void set_hora(uint8_t);


void hour_register_config();       //Configura el registro para las horas 12hr, 24hr

uint8_t get_rxBuff();



#endif /* I2C_H_ */
