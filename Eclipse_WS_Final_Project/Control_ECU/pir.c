/*
 ================================================================================================================================
 * File Name   : pir.c
 *
 * Module Name : pir
 * 
 * Description : Source File for PIR Sensor
 *
 * Created On  : Nov 1, 2023
 *
 * Author Name : Ahmed Abd Elgleel
 ================================================================================================================================
 */
#include "pir.h"

void PIR_init()
{
	GPIO_setupPinDirection(PIR_PORT_ID, PIR_PIN_ID, PIN_INPUT);
}

uint8 PIR_Motion(void)
{
	uint8 motion ;
	motion=GPIO_readPin(PIR_PORT_ID, PIR_PIN_ID);

	/*if their is a motion we will return MOTION_DETECTED else we will return NO_MOTION_DETECTED */
	return motion;
}

