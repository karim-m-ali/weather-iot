

/*
 * author : Ahmed Aly Hussien Elwakad
 * version: 1.0
 * date of last edit: 5/10/2024
 *
 * */

#ifndef WEATHER_C
#define WEATHER_C

#include "weather.h"
#include "./hal/dht11.h"
#include "./mcal/adc.h"

#define LDR_pin 0

weather_status_t weather_init(void){
  ADC_init();
  dht11_init();
return Weather_OK;
}

weather_status_t weather_measures(uint8_t * temperature
		                         ,uint8_t * humidity
					 ,uint8_t * light_percentage){


	if(dht11_read ( temperature, humidity)){
		return Weather_Error;
	}
	if(ADC_read ( light_percentage,LDR_pin)){
			return Weather_Error;
		}

	return Weather_OK;
}


#endif
