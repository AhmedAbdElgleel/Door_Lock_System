/*
 ================================================================================================================================
 * File Name   : buzzer.c
 *
 * Module Name : BUZZER
 *
 * Description : Source file for BUZZER driver
 *
 * Created On  : Oct 18, 2023
 *
 * Author Name : Ahmed Abd Elgleel
 ================================================================================================================================
 */
#include "gpio_interface.h"
#include "buzzer.h"

/*
 * Description :
 * Function to setup the direction of buzzer as output pin and turn off buzzer initially using gpio.
 */
void BUZZER_init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, TURN_OFF_BUZZER);
}

/*
 * Description :
 * Function to turn on the buzzer using gpio.
 */
void BUZZER_on(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, TURN_ON_BUZZER);
}

/*
 * Description :
 * Function to turn off the buzzer using gpio .
 */
void BUZZER_off(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, TURN_OFF_BUZZER);
}

/*
 * Description :
 * Function to Toggle the buzzer sound using gpio .
 */
void BUZZER_toggle(void)
{
	GPIO_togglePin(BUZZER_PORT_ID, BUZZER_PIN_ID);
}
