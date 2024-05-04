 /******************************************************************************
 *
 * Module: RTC
 *
 * File Name: ds1307.c
 *
 * Description: Source file for the ds1307 Real Time Clock driver
 *
 * Author: Mohamed Hossam
 *
 *******************************************************************************/

#include "ds1307.h"
#include "../mcal/twi.h"

uint8_t RTC_setTime(RTC_Time_t time)
{
	uint8_t i;/* Loop Iterator */

	/* Send Start bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
        return RTC_ERROR;

    /* Send Device Address with R/W = 0 (write) */
    TWI_writeByte(RTC_TWI_ADDRESS);
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return RTC_ERROR; 

    /* Send The Required Register Address */
    TWI_writeByte(0x00);
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return RTC_ERROR;

    /* Send The Time To The RTC Registers */
    for (i = 0; i < 7; i++)
    {
    	TWI_writeByte(time.timeArr[i]);
    	if (TWI_getStatus() != TWI_MT_DATA_ACK)
        	return RTC_ERROR;
    }

    /* Send the Stop Bit */
    TWI_stop();

    return RTC_SUCCESS;
}

uint8_t RTC_getTime(RTC_Time_t *time)
{
	uint8_t i;/* Loop Iterator */

	/* Send Start bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
        return RTC_ERROR;

    /* Send Device Address with R/W = 0 (Write) */
    TWI_writeByte(RTC_TWI_ADDRESS);
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return RTC_ERROR; 

    /* Send The Required Register Address */
    TWI_writeByte(0x00);
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return RTC_ERROR;

    /* Send the Repeated Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_REP_START)
        return RTC_ERROR;

    /* Send Device Address with R/W = 1 (Read) */
    TWI_writeByte(RTC_TWI_ADDRESS | 0x01);
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return RTC_ERROR;

    /* Get The Time From The RTC Registers */
    for (i = 0; i < 6; i++)
    {
    	time->timeArr[i] = TWI_readByteWithACK();
    	if (TWI_getStatus() != TWI_MR_DATA_ACK)
        	return RTC_ERROR;
    }

    /* Read The Last Byte With NACK */
    time->timeArr[i] = TWI_readByteWithNACK();
    if (TWI_getStatus() != TWI_MR_DATA_NACK)
        return RTC_ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    return RTC_SUCCESS;
}
