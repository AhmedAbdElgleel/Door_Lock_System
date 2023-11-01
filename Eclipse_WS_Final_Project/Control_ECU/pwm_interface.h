/*
 ================================================================================================================================
 * File Name   : pwm_timer0.h
 *
 * Module Name : PWM
 *
 * Description: Header file for the PWM
 *
 * Created On  : Oct 26, 2023
 *
 * Author Name : Ahmed Abd Elgleel
 ================================================================================================================================
 */


#ifndef PWM_INTERFACE_H_
#define PWM_INTERFACE_H_

#include"std_types.h"

/*******************************************************************************
 *                               	Typedef	                                   *
 *******************************************************************************/

typedef enum{
	PWM0,PWM2
}Pwm_Number;

typedef enum{
	NON_INVERTING=32,INVERTING=16+32
}Mode;

typedef enum
{
	/*Important remark:
	 * 		Timer0 and Timer1 have the same Prescalers but stay alert timer2 has no Counter mode
	 * 		so it has additional prescales so when deal with timer0 or timer1 just use the
	 * 		prescales as in First Row without prefix _T2 but on deal with Timer2 use prescales ended with _T2*/
	NO_PWM_CLOCK,PWM_F_CPU_CLOCK,PWM_F_CPU_8,PWM_F_CPU_64,PWM_F_CPU_256,PWM_F_CPU_1024,
	NO_PWM_CLOCK_T2=0,PWM_F_CPU_CLOCK_T2=1,PWM_F_CPU_8_T2=2,PWM_F_CPU_32_T2=3,PWM_F_CPU_64_T2=4,PWM_F_CPU_128_T2=5,PWM_F_CPU_256_T2=6,PWM_F_CPU_1024_T2=7
}Pwm_Clock;


typedef struct{
	Pwm_Number num;
	Mode mode;
	Pwm_Clock prescaleTimer;
}PWM_config;


/*******************************************************************************
 *                         FUNCTIONS Prototype	                               *
 *******************************************************************************/

void PWM_init(const PWM_config *conf);

void PWM_setDutyCycle(Pwm_Number pwm_no,uint8 duty);

#endif /* PWM_INTERFACE_H_ */
