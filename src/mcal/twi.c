 /******************************************************************************
 *
 * Module: TWI
 *
 * File Name: twi.c
 *
 * Description: Source file for the AVR TWI driver
 *
 * Author: Mohamed Hossam
 *
 *******************************************************************************/

#include<avr/io.h>
#include"twi.h"

#define BIT_IS_CLEAR(REG,BIT) ( !(REG & (1<<BIT)) )

/*
 * Description :
 * a function to initialize TWI
 */
void TWI_init(const TWI_ConfigType* Config_Ptr)
{
	/* set bit rate */
	TWBR = (((F_CPU / Config_Ptr->bit_rate) / 1000) - 16) / 2;
	/* set address for slave mode */
	TWAR = Config_Ptr->address << 1;
	/* set prescaller 1 */
	TWSR = 0;
	/* enable TWI */
	TWCR = (1<<TWEN);
}

/*
 * Description :
 * send start bit
 */
void TWI_start()
{
	/* Clear interrupt flag
	 * enable TWI
	 * set start condition to 1 */
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTA);
	/* wait until start bit is sent */
	while(BIT_IS_CLEAR(TWCR, TWINT));
}

/*
 * Description :
 * send stop bit
 */
void TWI_stop()
{
	/* Clear interrupt flag
	 * enable TWI
	 * set stop condition to 1 */
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTO);
}

/*
 * Description:
 * a function to send byte
 */
void TWI_writeByte(uint8_t data)
{
	/* send data */
	TWDR= data;
	/* Clear interrupt flag
	 * enable TWI */
	TWCR = (1<<TWEN) | (1<<TWINT);
	/* wait until data is sent */
	while(BIT_IS_CLEAR(TWCR, TWINT));
}

/*
 * Description:
 * a function to receive byte and send acknowledge
 */
uint8_t TWI_readByteWithACK()
{
	/* Clear interrupt flag
	 * enable TWI
	 * enable acknowledge bit */
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
	/* wait until data is received */
	while(BIT_IS_CLEAR(TWCR, TWINT));
	/* return data */
	return TWDR;
}

/*
 * Description:
 * a function to receive byte without sending acknowledge
 */
uint8_t TWI_readByteWithNACK()
{
	/* Clear interrupt flag
	 * enable TWI */
	TWCR = (1<<TWEN) | (1<<TWINT);
	/* wait until data is received */
	while(BIT_IS_CLEAR(TWCR, TWINT));
	/* return data */
	return TWDR;
}

/*
 * Description:
 * a function to get the status of the last operation
 */
uint8_t TWI_getStatus()
{
	/* return the 5 bits status */
	return (TWSR & 0xF8);
}
