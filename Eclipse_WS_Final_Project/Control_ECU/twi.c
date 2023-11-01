/*
 ================================================================================================================================
 * File Name   : twi.c
 *
 * Module Name : TWI(I2C)
 * 
 * Description : Source File for TWI Module
 *
 * Created On  : Oct 27, 2023
 *
 * Author Name : Ahmed Abd Elgleel
 ================================================================================================================================
 */

#include "twi_interface.h"
#include "twi_private.h"

void TWI_init(const I2c_ConfigType *i2c_config)
{
	/*
	 * Choose required bit rate & prescaler
	 * SCL frequency = (CPU clock frequency / ((16+2(TWBR))) * 4^TWPS)
	 * &0xFC to clear bits before insertion
	 */
	TWI_TWBR_REG.Byte = i2c_config->bit_rate;
	TWI_TWSR_REG.Byte = (TWI_TWSR_REG.Byte & 0xFC) |((i2c_config->i2c_prescaler)&0x03);

	/* Two Wire Bus address my address if any master device want to call me:master_slave_address  (used in case this MC is a slave device)
	   General Call Recognition: Off */
	/* my address = master_slave_address :) */
	TWI_TWAR_REG.Byte = i2c_config->address;

	/* enable TWI and clear all other bits in register TWCR -->0b0000 0100=0x04 */
	TWI_TWCR_REG.Byte=0x04;
}


void TWI_start(void)
{
    /*
	 * Clear the TWINT flag before sending the start bit TWINT=1 in TWCR Register
	 * send the start bit by TWSTA=1 in TWCR Register
	 * Enable TWI Module TWEN=1 in TWCR Register
	 * Clear all other bits in TWCR Register --->0b1010 0100=0xA4
	 */
	TWI_TWCR_REG.Byte=0xA4;
    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
	while(!(TWI_TWCR_REG.Bits.TWINT_BIT));

}

void TWI_stop(void)
{
    /*
	 * Clear the TWINT flag before sending the stop bit TWINT=1 in TWCR Register
	 * send the stop bit by TWSTO=1	in TWCR Register
	 * Enable TWI Module TWEN=1	in TWCR Register
	 * Clear all other bits in register TWCR Register --->0b1001 0100=0x94
	 */
	TWI_TWCR_REG.Byte=0x94;
}


void TWI_writeByte(uint8 data)
{
    /* Put data On TWI data Register */
    TWI_TWDR_REG.Byte = data;
    /*
	 * Clear the TWINT flag before sending the data TWINT=1 in Register TWCR
	 * Enable TWI Module TWEN=1	in Register TWCR
	 * Clear all other bits in register TWCR --->0b1000 0100=0x84
	 */
    TWI_TWCR_REG.Byte=0x84;

    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while(!(TWI_TWCR_REG.Bits.TWINT_BIT));
}



uint8 TWI_readByteWithACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 * Clear all other bits in register TWCR-->0b1100 0100=0xC4
	 */
    TWI_TWCR_REG.Byte=0xC4;

    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(!(TWI_TWCR_REG.Bits.TWINT_BIT));

    /* Read Data */
    return TWI_TWDR_REG.Byte;
}
uint8 TWI_readByteWithNACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 * Clear all other bits in register TWCR -->0b1000 0100=0x84
	 */
	TWI_TWCR_REG.Byte=0x84;

    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(!(TWI_TWCR_REG.Bits.TWINT_BIT));

    /* Read Data */
    return TWI_TWDR_REG.Byte;
}

uint8 TWI_getStatus(void)
{
    uint8 status;
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWI_TWSR_REG.Byte & 0xF8;
    return status;
}
