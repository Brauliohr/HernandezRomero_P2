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
#define WAIT_TIME                   10U
#define I2C_MASTER_SLAVE_ADDR      0x68U
#define I2C_BAUDRATE               100000U
#define I2C_DATA_LENGTH            3U

void i2c_init(void);
void transfer_data(void);
void receipt_data(void);





#endif /* I2C_H_ */
