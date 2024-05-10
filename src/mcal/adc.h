
/*
 * author : Ahmed Aly Hussien Elwakad
 * version: 1.0
 * date of last edit: 5/10/2024
 *
 * */

#ifndef ADC_H
#define ADC_H


typedef enum {
	ADC_channel0,
	ADC_channel1,
	ADC_channel2,
	ADC_channel3,
	ADC_channel4,
	ADC_channel5,
	ADC_channel6,
	ADC_channel7,
	ADC_channel8,
	ADC_channel9,
	ADC_channel10,
	ADC_channel11,
	ADC_channel12,
	ADC_channel13,
	ADC_channel14,
	ADC_channel15,
	ADC_channel16,
	ADC_channel17
}ChannelName;

typedef enum {
ADC_OK ,
ADC_Error

}ADC_status;


ADC_status ADC_init();
ADC_status ADC_read(uint8_t * ADC_value,ChannelName channel);


#endif
