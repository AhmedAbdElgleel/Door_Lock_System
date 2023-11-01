/*
 ================================================================================================================================
 * File Name   : GPIO_private.c
 *
 * Module Name : GPIO
 *
 * Description : Private Header file for GPIO driver Registers
 *
 * Created On  : Aug 17, 2023
 *
 * Author Name : Ahmed Abd Elgleel
 ================================================================================================================================
 */

#ifndef GPIO_PRIVATE_H_
#define GPIO_PRIVATE_H_

#include "std_types.h"
/*******************************************************************************
 *                  Registers type structure declaration                       *
 *******************************************************************************/

/* GPIO A Registers(PINA,PORTA,DDRA) type structure declarations */
typedef union
{
	uint8 Byte;
	struct
	{
		uint8 PA0:1;
		uint8 PA1:1;
		uint8 PA2:1;
		uint8 PA3:1;
		uint8 PA4:1;
		uint8 PA5:1;
		uint8 PA6:1;
		uint8 PA7:1;
	}Bits;
}GPIO_A_RegType;

/* GPIO B Registers(PINB,PORTB,DDRB) type structure declarations */
typedef union
{
	uint8 Byte;
	struct
	{
		uint8 PB0:1;
		uint8 PB1:1;
		uint8 PB2:1;
		uint8 PB3:1;
		uint8 PB4:1;
		uint8 PB5:1;
		uint8 PB6:1;
		uint8 PB7:1;
	}Bits;
}GPIO_B_RegType;

/* GPIO C Registers(PINC,PORTC,DDRC) type structure declarations */
typedef union
{
	uint8 Byte;
	struct
	{
		uint8 PC0:1;
		uint8 PC1:1;
		uint8 PC2:1;
		uint8 PC3:1;
		uint8 PC4:1;
		uint8 PC5:1;
		uint8 PC6:1;
		uint8 PC7:1;
	}Bits;
}GPIO_C_RegType;

/* GPIO D Registers(PIND,PORTD,DDRD) type structure declarations */
typedef union
{
	uint8 Byte;
	struct
	{
		uint8 PD0:1;
		uint8 PD1:1;
		uint8 PD2:1;
		uint8 PD3:1;
		uint8 PD4:1;
		uint8 PD5:1;
		uint8 PD6:1;
		uint8 PD7:1;
	}Bits;
}GPIO_D_RegType;

/*******************************************************************************
 * Registers definitions for each type struct in the memory mapped Registers   *
 *******************************************************************************/

#define GPIO_PIND_REG  		(*((volatile const GPIO_D_RegType * const)0x0030))			/* PIND  ---> $(0x30) */
#define GPIO_DDRD_REG  		(*((volatile GPIO_D_RegType * const)0x0031))				/* DDRD  ---> $(0x31) */
#define GPIO_PORTD_REG 		(*((volatile GPIO_D_RegType * const)0x0032))				/* PORTD ---> $(0x32) */

#define GPIO_PINC_REG  		(*((volatile const GPIO_C_RegType * const)0x0033))			/* PINC  ---> $(0x33) */
#define GPIO_DDRC_REG  		(*((volatile GPIO_C_RegType * const)0x0034))				/* DDRC  ---> $(0x34) */
#define GPIO_PORTC_REG 		(*((volatile GPIO_C_RegType * const)0x0035))				/* PORTC ---> $(0x35) */

#define GPIO_PINB_REG  		(*((volatile const GPIO_B_RegType * const)0x0036))			/* PINB  ---> $(0x36) */
#define GPIO_DDRB_REG  		(*((volatile GPIO_B_RegType * const)0x0037))				/* DDRB  ---> $(0x37) */
#define GPIO_PORTB_REG 		(*((volatile GPIO_B_RegType * const)0x0038))				/* PORTB ---> $(0x38) */

#define GPIO_PINA_REG  		(*((volatile const GPIO_A_RegType * const)0x0039))			/* PINA   ---> $(0x39) */
#define GPIO_DDRA_REG  		(*((volatile GPIO_A_RegType * const)0x003A))				/* DDRAA  ---> $(0x3A) */
#define GPIO_PORTA_REG 		(*((volatile GPIO_A_RegType * const)0x003B))				/* PORTA  ---> $(0x3B) */

#endif /* GPIO_PRIVATE_H_ */
