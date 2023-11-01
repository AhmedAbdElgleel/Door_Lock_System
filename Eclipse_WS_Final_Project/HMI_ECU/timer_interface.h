/*
 ================================================================================================================================
 * File Name   : timer_interface.h
 *
 * Module Name : TIMERS
 * 
 * Description : Header File For Timer 0 , Timer 1 , Timer 2 Modules
 *
 * Created On  : Oct 26, 2023
 *
 * Author Name : Ahmed Abd Elgleel
 ================================================================================================================================
 */
#ifndef TIMER_INTERFACE_H_
#define TIMER_INTERFACE_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	/*Important remark:
	 * 		Timer0 and Timer1 have the same Prescalers but stay alert timer2 has no Counter mode
	 * 		so it has additional prescales so when deal with timer0 or timer1 just use the
	 * 		prescales as in First Row without prefix _T2 but on deal with Timer2 use prescales ended with _T2*/
	TIMER_NO_CLOCK,TIMER_F_CPU_CLOCK,TIMER_F_CPU_8,TIMER_F_CPU_64,TIMER_F_CPU_256,TIMER_F_CPU_1024,EXT_CLK_FALLING,EXT_CLK_RISING,
	TIMER_NO_CLOCK_T2=0,TIMER_F_CPU_CLOCK_T2=1,TIMER_F_CPU_8_T2=2,TIMER_F_CPU_32_T2=3,TIMER_F_CPU_64_T2=4,TIMER_F_CPU_128_T2=5,TIMER_F_CPU_256_T2=6,TIMER_F_CPU_1024_T2=7
}Timer_Clock;

/* Choose the timer module u want to activate */
typedef enum
{
	Timer0,Timer1,Timer2
}Timer_ID_Type;

/* Choose the timer mode u want to use */
typedef enum
{
	/*Note timer2 does not have pwm mode*/
	Overflow_Mode,Compare_Mode
}Timer_Mode;

typedef struct
{
	uint16 timer_InitialValue;
	uint16 timer_compare_MatchValue;	/*it will be used in compare mode only*/
	Timer_ID_Type  timer_ID;
	Timer_Clock timer_clock;
	Timer_Mode  timer_mode;

}Timer_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * A function to set Call back for timers
 */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID );

/*
 * Description :
 * A function to initialize the timers
 */
void Timer_init(const Timer_ConfigType * Config_Ptr);

/*
 * Description :
 * A function to deinitialize the timers
 */
void Timer_DeInit(Timer_ID_Type timer_type);

#endif /* TIMER_INTERFACE_H_ */
