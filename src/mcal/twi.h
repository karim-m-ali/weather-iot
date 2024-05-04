 /******************************************************************************
 *
 * Module: TWI
 *
 * File Name: twi.h
 *
 * Description: Header file for the AVR TWI driver
 *
 * Author: Mohamed Hossam
 *
 *******************************************************************************/
#ifndef TWI_H_
#define TWI_H_

#include <stdint.h>

/*******************************************************************************
 * 									macros
 *******************************************************************************/
/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 * 							   Types Declarations
 *******************************************************************************/
typedef struct{
 uint8_t address;
 uint16_t bit_rate; /* bit rate in kilobit per second */
}TWI_ConfigType;
/*******************************************************************************
 * 							  Functions Prototypes
 *******************************************************************************/
/*
 * Description :
 * a function to initialize TWI
 */
void TWI_init(const TWI_ConfigType* Config_Ptr);

/*
 * Description :
 * send start bit
 */
void TWI_start();

/*
 * Description :
 * send stop bit
 */
void TWI_stop();

/*
 * Description:
 * a function to send byte
 */
void TWI_writeByte(uint8_t data);

/*
 * Description:
 * a function to receive byte and send acknowledge
 */
uint8_t TWI_readByteWithACK();

/*
 * Description:
 * a function to receive byte without sending acknowledge
 */
uint8_t TWI_readByteWithNACK();

/*
 * Description:
 * a function to get the status of the last operation
 */
uint8_t TWI_getStatus();
#endif /* TWI_H_ */
