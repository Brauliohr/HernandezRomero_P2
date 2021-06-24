/*               PROYECTO 2
 *    DISEÑO CON MICROCONTROLADORES
 *    "Reloj Digital" - Implementando:
 *             UART, I2C, PIT
 *    Autor: Braulio Hernández Romero
 *    Fecha de entrega: 24/junio/2021
 */
#include <stdio.h>
#include <string.h>
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_uart.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "uart.h"
#include "fsl_i2c.h"
#include "pit.h"
#include "i2c.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
 /* Variables
 ******************************************************************************/

uint8_t tipString[] =
		"Proyecto 2 - Diseño con Microcontroladores\r\nOpciones:\r\n1) Configurar hora\r\n2) Configurar fecha\r\n3) Leer fecha\r\n 4) Leer hora\r\n";
uint8_t string2[] =
		"\r\nIngresa la hora con formato (HH:MM:SS):\n";
uint8_t string3[] =
		"\r\nIngresa la fecha con formato (DD:MM:AA):\n";
uint8_t string4[] =
		"\r\nLA HORA ACTUAL ES:\n";
uint8_t string5[] =
		"\r\nLA FECHA ACTUAL ES:\n";


/*******************************************************************************
 * Code
 ******************************************************************************/


int main(void)
{
    //UART config init
	uart0_config();

	//I2C config init
	i2c_init();

	/* Send g_tipString out. */
	UART_WriteBlocking(UART0, tipString, sizeof(tipString) / sizeof(tipString[0]));

    while (1)
    {
    	sent_data();

        if(get_data() == '1'){
        	//set_data(0x1b);
        	if(get_data() == 13){
        		//configurar hora
        		printf("Enter key is pressed\n");
            	UART_WriteBlocking(UART0, string2, sizeof(string2) / sizeof(string2[0]));  // muestra la opcion de cambio de frecuencia

            	//Ingresar Hora



               	set_data();
        	}
        }

        if(get_data() == '2'){
                if(get_data() == 13){
                //configurar fecha
                printf("Enter key is pressed\n");
                UART_WriteBlocking(UART0, string3, sizeof(string3) / sizeof(string3[0]));  // muestra la opcion de cambio de frecuencia

                //Ingresar Fecha


                set_data();
             }
        }
        if(get_data() == '3'){
                 if(get_data() == 13){
                 //Mostrar fecha
                 printf("Enter key is pressed\n");
                 UART_WriteBlocking(UART0, string4, sizeof(string4) / sizeof(string4[0]));  // muestra la opcion de cambio de frecuencia

                 //Mostrar fecha


                  set_data();
              }
         }

        if(get_data() == '4'){
                 if(get_data() == 13){
                 //Mostrar hora
                 printf("Enter key is pressed\n");
                 UART_WriteBlocking(UART0, string5, sizeof(string5) / sizeof(string5[0]));  // muestra la opcion de cambio de frecuencia

                 //Mostrar hora



                  set_data();
              }
         }



    }
}
