/*
 ================================================================================================================================
 * File Name   : uart.c
 *
 * Module Name : UART
 * 
 * Description : Source File for Uart Module
 *
 * Created On  : Oct 25, 2023
 *
 * Author Name : Ahmed Abd Elgleel
 ================================================================================================================================
 */
#include "uart_interface.h"
#include "uart_private.h" /* To use the UART Registers */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *UART_Config)
{
	uint16 ubrr_value=0;
	/* U2X = 1 for double transmission speed */
	UART_UCSRA_REG.Byte=0;
	UART_UCSRA_REG.Bits.U2X_BIT=1;

	/*Enable Sending and transmitting TXEN=1 AND RXEN=1 in register UCSRB*/
	UART_UCSRB_REG.Bits.RXEN_BIT=1;
	UART_UCSRB_REG.Bits.TXEN_BIT=1;

	/*Set Bit URSEL (URSEL=1) to access register UCSRC as UCSRC and UBRRH Mapped on same location as  declared in uart private file  */
	UART_UCSRC_REG.Bits.URSEL_BIT=1;

	/*Write the Selected Parity*/
	UART_UCSRC_REG.Byte|=(UART_Config->parity);

	/*Write the selected stop bits*/
	UART_UCSRC_REG.Byte|=(UART_Config->stop_bit);

	/*Character size*/
	/*NOTE:
	 * 		UCSZ0 bit ,UCSZ1 bit in register UCSRC and UCSZ2 bit in register USCRB
	 * 		ARE responsible for the Character size selection but kindly Becarful
	 * 		Those 3 bits are not in the same register as mentioned so we have two diffent
	 * 		ways for implemtation first way is just make and if-elseif-elseif-elseif-else statments
	 * 		but you will use more code size but i choosed the second way as following
	 * 		for example if we use 8 bit data
	 * 		(UART_Config->bit)=0011
	 * 		(UART_Config->bit)>>1=0110
	 * 		((UART_Config->bit)>>1)&6=0110 & 0110 = 0110
	 * 		  */
	UART_UCSRC_REG.Byte|=(((UART_Config->bit_data)<<1)&6);
	UART_UCSRC_REG.Byte|=(((UART_Config->bit_data)<<2)&4);

	/* Calculate the UBRR register value */
	ubrr_value = (uint16)(((F_CPU / ((UART_Config->baud_rate) * 8UL))) - 1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UART_UBRRH_REG.Byte = ubrr_value>>8;
	UART_UBRRL_REG.Byte = ubrr_value;
}


/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(!(UART_UCSRA_REG.Bits.UDRE_BIT));

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UART_UDR_REG.Byte = data;

	/* Another Way
	UART_UDR_REG.Byte= data;
	while(!(UART_UCSRA_REG.Bits.TXC_BIT)); // Wait until the transmission is complete TXC = 1
	UART_UCSRA_REG.Bits.TXC_BIT=1; // Clear the TXC flag
	*/

}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(!(UART_UCSRA_REG.Bits.RXC_BIT));

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
    return UART_UDR_REG.Byte;
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}
	*******************************************************************/
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
