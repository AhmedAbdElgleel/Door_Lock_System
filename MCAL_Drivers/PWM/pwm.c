/*
 ================================================================================================================================
 * File Name   : pwm_timer0.c
 *
 * Module Name : PWM
 *
 * Description: Source file for the PWM
 *
 * Created On  : Oct 26, 2023
 *
 * Author Name : Ahmed Abd Elgleel
 ================================================================================================================================
 */

#include "gpio_interface.h"
#include "pwm_interface.h"
#include "timer_private.h"


/*******************************************************************************
 *                         FUNCTIONS definition	                               *
 *******************************************************************************/

void PWM_init(const PWM_config *Config_Ptr)
{
	if(Config_Ptr->num == PWM0)
	{
		/*Clear FOC0 bit in case using pwm mode FOC0=0 IN TCCR0*/
		TIMER0_TCCR0_REG.Bits.FOC0_Bit=0;

		/*Choose Fast Pwm Mode via set WGM00=1 AND WGM01=1 IN TCCR0*/
		TIMER0_TCCR0_REG.Bits.WGM00_Bit=1;
		TIMER0_TCCR0_REG.Bits.WGM01_Bit=1;

		/*choose the mode inverting or non inverting*/
		TIMER0_TCCR0_REG.Byte|=(Config_Ptr->mode);

		/*choose prescale*/
		TIMER0_TCCR0_REG.Byte|=(Config_Ptr->prescaleTimer);

		GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);

		/*Just Initially*/
		TIMER0_OCR0_REG.Byte=0;

	}

	else if(Config_Ptr->num == PWM2)
	{
		/*Clear FOC2 bit in case using pwm mode FOC2=0 IN TCCR2*/
		TIMER2_TCCR2_REG.Bits.FOC2_Bit=0;

		/*Choose Fast Pwm Mode via set WGM20=1 AND WGM21=1 IN TCCR2*/
		TIMER2_TCCR2_REG.Bits.WGM20_Bit=1;
		TIMER2_TCCR2_REG.Bits.WGM21_Bit=1;

		/*choose the mode inverting or non inverting*/
		TIMER2_TCCR2_REG.Byte|=(Config_Ptr->mode);

		/*choose prescale*/
		TIMER2_TCCR2_REG.Byte|=(Config_Ptr->prescaleTimer);

		GPIO_setupPinDirection(PORTD_ID, PIN7_ID, PIN_OUTPUT);

		/*Just Initially*/
		TIMER2_OCR2_REG.Byte=0;

	}
}

void PWM_setDutyCycle(Pwm_Number a_pwmNum,uint8 a_duty)
{
	if(a_pwmNum == PWM0)
	{
		TIMER0_OCR0_REG.Byte=((uint32)(a_duty* 255) / 100);
	}

	else if(a_pwmNum == PWM2)
	{
		TIMER2_OCR2_REG.Byte=((uint32)(a_duty* 255) / 100);
	}
}
