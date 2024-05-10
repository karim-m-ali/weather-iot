
/*
 * author : Ahmed Aly Hussien Elwakad
 * version: 1.0
 * date of last edit: 10/5/2024
 *
 * */


#ifndef WEATHER_H
#define WEATHER_H


typedef enum{
	Weather_OK,
	Weather_Error
}weather_status_t;



weather_status_t weather_init(void);

weather_status_t weather_measures(uint8_t * temperature
		                         ,uint8_t * humidity
								 ,uint8_t * light_percentage);

#endif
