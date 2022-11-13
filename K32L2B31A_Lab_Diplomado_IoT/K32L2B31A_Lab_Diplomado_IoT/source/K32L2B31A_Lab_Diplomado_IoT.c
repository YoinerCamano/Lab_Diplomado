/* @File: K32L2B31A_La_Diplomado_IoT.c
 * @Autor: Yoiner Camaño Quintero
 * @Version 0.00
 * @date 05/11/2022
 * @brief Funcion principal main
 * @details
 *
 *
 *
 *
 * */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "K32L2B31A.h"

#include "fsl_debug_console.h"
#include "fsl_adc16.h"
#include "fsl_common.h"

#include "lpuart0.h"


uint8_t temp ;


int main(void) {

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	BOARD_InitLEDsPins();

	/*Variables*/
	float voltaje = 0;
	float voltaje_entrada = 0;
	float corriente = 0;
	float lux = 0;

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif
	/*Configurar ADC para lectura*/

	ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP,
			&ADC0_channelsConfig[0]);

	while (1) {

    	if(leer_bandera_nuevo_dato() != 0){

    		temp = leer_dato();

    		escribir_bandera_nuevo_dato(0);

    		PRINTF("dato es : %c\r\n",temp);


    		if(temp == 76){

				ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP,
						&ADC0_channelsConfig[0]);

				/*Espera que el ADC finalice la conversion*/

				while (0U == (kADC16_ChannelConversionDoneFlag& ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL,ADC0_CH0_CONTROL_GROUP)))
				{
				}
				/*captura de dato del ADC, conversion de voltaje, corriente y lux*/

				voltaje_entrada = (ADC16_GetChannelConversionValue(ADC0_PERIPHERAL,ADC0_CH0_CONTROL_GROUP))*(3.3/4095);

				voltaje = 3.30 - voltaje_entrada;

				if (voltaje == 0){
					corriente = 0;
					lux = 0;
				}else{
					corriente = voltaje/10000;
					lux = (125/26)*(corriente*10000) + (375/13);
				}
				PRINTF("Voltaje es : %2.2fV\r\n",voltaje_entrada);
				PRINTF("Corriente es : %f uA\r\n",(corriente*10000));
				PRINTF("Iluminancia es : %f\r\n",(lux));
    		}
    		if(temp == 86){
			GPIO_PortToggle(BOARD_LED1_GPIO, 1u << BOARD_LED1_PIN);
    		}
    		if(temp == 82){

			GPIO_PortToggle(BOARD_LED2_GPIO, 1u << BOARD_LED2_PIN);
    		}

    		if(temp == 118){
    			GPIO_PortToggle(BOARD_LED1_GPIO, 1u << BOARD_LED1_PIN);
    		}

    		if (temp == 114){
    			GPIO_PortToggle(BOARD_LED2_GPIO, 1u << BOARD_LED2_PIN);
    		}

			__asm volatile ("nop");

    	}
	}
	return 0;



}
