/*
 ================================================================================================================================
 * File Name   : external_eeprom.c
 *
 * Module Name : External EEPROM
 * 
 * Description : Source File For External EEPROM
 *
 * Created On  : Oct 27, 2023
 *
 * Author Name : Ahmed Abd Elgleel
 ================================================================================================================================
 */

#include "external_eeprom.h"
#include "twi_interface.h"
#include "util/delay.h"

uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data)
{
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8)(0xA0 | ((u16addr & 0x0700)>>7)));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	/* Send the required memory location address */
	TWI_writeByte((uint8)(u16addr));
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* write byte to eeprom */
	TWI_writeByte(u8data);
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}

uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data)
{
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700)>>7)));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	/* Send the required memory location address */
	TWI_writeByte((uint8)(u16addr));
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* Send the Repeated Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_REP_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=1 (Read) */
	TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700)>>7) | 1));
	if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
		return ERROR;


	/* Read Byte from Memory without send ACK */
	*u8data = TWI_readByteWithNACK();
	if (TWI_getStatus() != TWI_MR_DATA_NACK)
		return ERROR;

	/* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}

void EEPROM_writeArray(uint16 startaddress,uint8 *array,uint8 a_arraysize)
{
	uint8 i;
	for(i=0;i<a_arraysize;i++)
	{
		EEPROM_writeByte((startaddress+i),array[i]);
		_delay_ms(10);
	}
}

void EEPROM_readArray(uint16 startaddress,uint8 *array,uint8 a_arraysize)
{
	uint8 i;
	for(i=0;i<a_arraysize;i++)
	{
		EEPROM_readByte((startaddress+i),array[i]);
		_delay_ms(10);
	}
}
