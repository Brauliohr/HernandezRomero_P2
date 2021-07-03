/*    - PROYECTO 2
 *    - DISEÑO CON MICROCONTROLADORES
 *    - Profesor: Dr. José Luis Pizano Escalante
 *    - "Reloj Digital" - Implementando:
 *    - UART, I2C -> RTC (DS1307)
 *    - Autor: Braulio Hernández Romero
 *    - Fecha de entrega: 02/julio/2021
 *
 ***********************************************/
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
#include "i2c.h"

/**** Opciones del menú ****/
typedef enum{
	menu,
	input_hour,
	input_date,
	show_hour,
	show_date
}State_name;


 /* Cadenas para los mensajes en la terminal
 ******************************************************************************/

uint8_t tipString[] =
		"\nPROYECTO 2 - RELOJ DIGITAL\r\nOPCIONES:\r\n1) CONFIGURAR HORA\r\n2) CONFIGURAR FECHA\r\n3) LEER HORA\r\n4) LEER FECHA\r\n";
uint8_t string2[] =
		"\r\nINGRESA LA HORA CON EL FORMATO: (HH:MM:SS):\n";
uint8_t string3[] =
		"\r\nINGRESA LA FECHA CON EL FORMATO (DD:MM:AA):\n";
uint8_t string4[] =
		"\r\nLA HORA ACTUAL ES:\n";
uint8_t string5[] =
		"\r\nLA FECHA ACTUAL ES:\n";
uint8_t string6[] =
		"\r\nPlease select an option between 1-4\n";


/****** Banderas para el flujo del programa *****/
bool flag2 = true;
bool flag3 = true;
bool flag4 = true;
bool flag5 = true;
bool flag6 = true;
bool flag7 = true;

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
	uart0_config();                         //Inicializa la UART0

	i2c_init();								//Inicializa el módulo I2C

	hour_register_config();                 //Inicializa el RTC (DS1307) en modo 24 hrs

	State_name state = menu;

    while (1)
    {

    	if(flag2)
    	{
    		/* Send g_tipString out. */
    		UART_WriteBlocking(UART0, tipString, sizeof(tipString) / sizeof(tipString[0]));
    		flag2 = false;
    	}

    	switch(state){
    	case menu:

    		sent_data();

    		uint8_t option = get_ring_buffer(0) - 0x30;    //para seleccionar la opción del menú
    		uint8_t enter = get_ring_buffer(1);            //segundo dato enviado, para presionar "enter"

    		if(option >= input_hour && option <= show_date && enter == 13){  //Evalúa a que opción del menu ir, al presionar la opción y luego enter
    			state = option;

    			set_rxIndex(0);
    			set_txIndex(0);

    			flag3 = true;
    		}
    		else if(option > show_date && enter == 13)
    		{
    			if(flag7){
    				UART_WriteBlocking(UART0, string6, sizeof(string6) / sizeof(string6[0]));
    				set_rxIndex(0);
    				set_txIndex(0);
    				flag7 = false;
    			}

    		}

    		break;

    	case input_hour: 										//Opción del menú para igresar la hora (hh:mm:ss)

    		sent_data();

    		if(flag3)
    		{
    			/* Send g_tipString out. */
    		    UART_WriteBlocking(UART0, string2, sizeof(string2) / sizeof(string2[0]));

    		    flag3 = false;

    		    for(uint8_t i = 0; i <= 8; i++){
    		    	set_ring_buffer(i);
    		    }
    		}

    		uint8_t dec_hour = (get_ring_buffer(0) - 0x30)*10;    //Variable para almacenar las horas - decenas
    		uint8_t unit_hour = (get_ring_buffer(1) - 0x30)*1;    //Variable para almacenar las horas - unidades
    		uint8_t	hour_in = dec_hour + unit_hour;               //Variable para las horas (total)
    		uint8_t dec_min = (get_ring_buffer(3) - 0x30)*10;
    		uint8_t unit_min = (get_ring_buffer(4) - 0x30)*1;
    		uint8_t	min_in = dec_min + unit_min;               //Variable para los min (total)
    		uint8_t dec_sec = (get_ring_buffer(6) - 0x30)*10;
    		uint8_t unit_sec = (get_ring_buffer(7) - 0x30)*1;
    		uint8_t	sec_in = dec_sec + unit_sec;               //Variable para los seg (total)
    		uint8_t enter2 = get_ring_buffer(8);                  //Variable para el "Enter"


    		if(hour_in <= 23 && enter2 == 13 && flag4)
    		{
    			printf("La hora ingresada fue: %d:%d:%d\n", hour_in, min_in, sec_in);
    			//set_hora(hour_in);
    			transfer_hour(hour_in);       //Envía la hora al RTC
    			transfer_min(min_in);
    			transfer_sec(sec_in);

    			state = menu;
    			flag2 = true;
    			flag4 = false;

    			//Reinicia el buffer
    			set_rxIndex(0);
    			set_txIndex(0);

    			//Setea a 0 los valores internos del buffer
    			for(uint8_t i = 0; i <= 8; i++){
    				set_ring_buffer(i);
    			}

    		}

    		//*Por agregar: Bloques para validar la hora ingresada

    		/*Secuencia de escape*/
    		if(get_data() == 27)
    		{
    			//presiona esc para ir al menu principal
    			state = menu;
    			set_data();

    			flag2 = true;

    			//Reinicia el buffer
    			set_rxIndex(0);
    			set_txIndex(0);

    			//Setea a 0 los valores internos del buffer
    			for(uint8_t i = 0; i <= 8; i++){
    				set_ring_buffer(i);
    		    }
    		}

    		break;

    	case input_date:                                       //Opción del menú para ingresar la fecha (dd:mm:aa)

    		sent_data();
    		flag4 = true;

    		if(flag5)
    		{
    			/* Send g_tipString out. */
    		    UART_WriteBlocking(UART0, string3, sizeof(string3) / sizeof(string3[0]));
    		    flag5 = false;

    		    for(uint8_t i = 0; i <= 8; i++){
    		    	set_ring_buffer(i);
    		    }
    		}

    		uint8_t dec_date = (get_ring_buffer(0) - 0x30)*10;    //Variable para almacenar las horas - decenas
    		uint8_t unit_date = (get_ring_buffer(1) - 0x30)*1;    //Variable para almacenar las horas - unidades
    		int8_t	date_in = dec_date + unit_date;               //Variable para las horas (total)
    		uint8_t dec_month = (get_ring_buffer(3) - 0x30)*10;
    		uint8_t unit_month = (get_ring_buffer(4) - 0x30)*1;
    		uint8_t	month_in = dec_month + unit_month;               //Variable para los min (total)
    		uint8_t dec_year = (get_ring_buffer(6) - 0x30)*10;
    		uint8_t unit_year = (get_ring_buffer(7) - 0x30)*1;
    		uint8_t	year_in = dec_year + unit_year;               //Variable para los seg (total)
    		enter2 = get_ring_buffer(8);                  		  //Variable para el "Enter"



    		if(date_in <= 31 && enter2 == 13 && flag4)
    		{
    			printf("La fecha ingresada fue: %d:%d:%d\n", date_in, month_in, year_in);
    		    //set_hora(hour_in);
    		    transfer_date(date_in);       //Envía el día al RTC
    		    transfer_month(month_in);
    		    transfer_year(year_in);

    		    state = menu;
    		    flag2 = true;
    		    flag4 = false;

    		    set_data();

    		    //Setea a 0 los valores internos del buffer
    		    for(uint8_t i = 0; i <= 8; i++){
    		    	set_ring_buffer(i);
    		    }
    		    //Reinicia el buffer
    		    set_rxIndex(0);
    		    set_txIndex(0);

    		}

    		//*Por agregar: Bloques para validar la fecha ingresada

    		/*Secuencia de escape*/
    		 if(get_data() == 27)
    		 {
    		 	//presiona esc para ir al menu principal
    		    state = menu;
    		    set_data();

    		    flag2 = true;

    		    //Reinicia el buffer
    		    set_rxIndex(0);
    		    set_txIndex(0);

    		    //Setea a 0 los valores internos del buffer
    		    for(uint8_t i = 0; i <= 8; i++){
    		    	set_ring_buffer(i);
    		  	}
    		  }

    		break;


    	case show_hour:                                               //Opción del menú para mostrar la hora

    		if(flag6)
    		{
    			/* Send g_tipString out. */
    		    UART_WriteBlocking(UART0, string4, sizeof(string4) / sizeof(string4[0]));

    		    uint8_t array_hour[3];
    		    array_hour[0] = receipt_hour();
    		    array_hour[1] = receipt_min();
    		    array_hour[2] = receipt_sec();

    		    //*Por resolver, el envío de los datos de array_hour hacía la UART
    		    //Están siendo envíados por el esclavo hacía el maestro, pero falta
    		    //hacer que esos datos sean enviados a una función para escribirlos en
    		    //el buffer de transmisión a la UART

    		    printf("La hora actual es: %d:%d:%d\n", array_hour[0], array_hour[1], array_hour[2]);

    		    state = menu;
    		    flag2 = true;

    		    //Reinicia el buffer
    		    set_rxIndex(0);
    		    set_txIndex(0);

    		    //Setea a 0 los valores internos del buffer
    		    for(uint8_t i = 0; i <= 8; i++){
    		    	set_ring_buffer(i);
			    }
    		    flag6 = false;
    		}

    		break;

    	case show_date:                                               //Opción del menú para mostrar la fecha

    		if(flag7)
    		{
    			/* Send g_tipString out. */
    		    UART_WriteBlocking(UART0, string5, sizeof(string5) / sizeof(string5[0]));

    		    uint8_t array_date[3];
    		    array_date[0] = receipt_date();
    		    array_date[1] = receipt_month();
    		    array_date[2] = receipt_year();

    		    //*Por resolver, el envío de los datos de array_hour hacía la UART
    		    //Están siendo envíados por el esclavo hacía el maestro, pero falta
    		    //hacer que esos datos sean enviados a una función para escribirlos en
    		    //el buffer de transmisión a la UART
    		    printf("La fecha actual es: %d:%d:%d\n", array_date[0], array_date[1], array_date[2]);

    		    state = menu;
    		    flag2 = true;

    		    //Reinicia el buffer
    		    set_rxIndex(0);
    		    set_txIndex(0);

    		    //Setea a 0 los valores internos del buffer
    		    for(uint8_t i = 0; i <= 8; i++){
    		    set_ring_buffer(i);
    			}

    		    flag7 = false;
    		    }

    		break;

    	} //Fin del switch case

    } //fin del while(1)

    return 0;
} //Fin del main
