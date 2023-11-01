/*
 ================================================================================================================================
 * File Name   : buzzer.h
 *
 * Module Name : BUZZER
 *
 * Description : Header file for BUZZER driver
 *
 * Created On  : Oct 18, 2023
 *
 * Author Name : Ahmed Abd Elgleel
 ================================================================================================================================
 */
#ifndef BUZZER_H_
#define BUZZER_H_
#include "std_types.h"

/*choose the port and pin that the buzzer connected to it*/
#define BUZZER_PORT_ID		PORTC_ID
#define BUZZER_PIN_ID		PIN7_ID
#define BUZZER_ENBALE_WITH_NPN

#ifdef	BUZZER_ENBALE_WITH_NPN
#define TURN_ON_BUZZER	LOGIC_HIGH
#define TURN_OFF_BUZZER	LOGIC_LOW
#else
#define TURN_ON_BUZZER	LOGIC_LOW
#define TURN_OFF_BUZZER	LOGIC_HIGH
#endif

/*
 * Description :
 * Function to setup the direction of buzzer as output pin and turn off buzzer initially using gpio.
 */
void BUZZER_init(void);

/*
 * Description :
 * Function to turn on the buzzer using gpio.
 */
void BUZZER_on(void);

/*
 * Description :
 * Function to turn off the buzzer using gpio .
 */
void BUZZER_off(void);

/*
 * Description :
 * Function to Toggle the buzzer sound using gpio .
 */
void BUZZER_toggle(void);

#endif /* BUZZER_H_ */
