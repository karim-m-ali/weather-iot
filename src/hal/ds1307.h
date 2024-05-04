 /******************************************************************************
 *
 * Module: RTC
 *
 * File Name: ds1307.h
 *
 * Description: Header file for the ds1307 Real Time Clock driver
 *
 * Author: Mohamed Hossam
 *
 *******************************************************************************/

#ifndef DS1307_H_
#define DS1307_H_

#include <stdint.h>

/*******************************************************************************
 * 									macros
 *******************************************************************************/
#define RTC_TWI_ADDRESS							0b11010000

#define RTC_ERROR 								0
#define RTC_SUCCESS 							1

/*******************************************************************************
 *                      		  Data Types        	                       *
 *******************************************************************************/
/* Data Structure To Store Time either in an Array Or a struct
 * where array indices access the struct fields in the same order */
typedef union{
	uint8_t timeArr[7];
	struct{
		uint8_t seconds;
		uint8_t minutes;
		uint8_t hours;
		uint8_t dayOfWeek;
		uint8_t dayOfMonth;
		uint8_t month;
		uint8_t year; 
	}time;
}RTC_Time_t;
/*******************************************************************************
 *                      	 Functions Prototypes                              *
 *******************************************************************************/
/*
 * Description :
 * a Function To Set The Time Of The RTC
 */
uint8_t RTC_setTime(RTC_Time_t time);


/*
 * Description :
 * a Function To Get The Time From The RTC
 */
uint8_t RTC_getTime(RTC_Time_t *time);

#endif
