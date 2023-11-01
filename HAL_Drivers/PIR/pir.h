/*
 ================================================================================================================================
 * File Name   : pir.h
 *
 * Module Name : pir
 * 
 * Description : Header File for PIR Sensor
 *
 * Created On  : Nov 1, 2023
 *
 * Author Name : Ahmed Abd Elgleel
 ================================================================================================================================
 */
#ifndef PIR_H_
#define PIR_H_
#include "std_types.h"
#include "gpio_interface.h"

#define MOTION_DECTECTED		1
#define NO_MOTION_DECTECTED		0

#define PIR_PORT_ID		PORTC_ID
#define PIR_PIN_ID		PIN2_ID

/*Init function For PIR*/
void PIR_init();

/*Function to detect the motion*/
uint8 PIR_Motion(void);

#endif /* PIR_H_ */
