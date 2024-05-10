
/*
 * author : Ahmed Aly Hussien Elwakad
 * version: 1.0
 * date of last edit: 10/5/2024
 *
 * */



#ifndef ADC_C
#define ADC_C
#include<avr/io.h>
#include"adc.h"

#define CLR_BIT(ADDR,BIT) (ADDR &= ~(1<<BIT))
#define SET_BIT(ADDR,BIT) (ADDR |= (1<<BIT))
#define TGL_BIT(ADDR,BIT) (ADDR ^= (1<<BIT))
#define GET_BIT(ADDR,BIT) ((ADDR & (1<<BIT))>>BIT)



ADC_status ADC_init(){
	SET_BIT(ADMUX,REFS0);
	CLR_BIT(ADMUX,REFS1);

	SET_BIT(ADMUX,ADLAR);

	SET_BIT(ADCSRA,ADPS2);
	SET_BIT(ADCSRA,ADPS1);
	SET_BIT(ADCSRA,ADPS0);

    SET_BIT(ADCSRA,ADEN);

    return ADC_OK;
}
ADC_status ADC_read(uint8_t * ADC_value,ChannelName channel){

switch(channel){
case ADC_channel0:break;
case ADC_channel1:break;
case ADC_channel2:break;
case ADC_channel3:break;
case ADC_channel4:break;
case ADC_channel5:break;
case ADC_channel6:break;
case ADC_channel7:break;
default:
	return ADC_Error;
}

	ADMUX&=0xF8;

	ADMUX|=channel;

	SET_BIT(ADCSRA,ADSC);

	while(GET_BIT(ADCSRA,ADSC));



	*ADC_value = ADCH;


	return ADC_OK;
}
#endif
