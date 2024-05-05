/**
 * @file weather.h
 * @brief Functions of weather measurement.
 * @author Your Name
 * @date Data of last edit
 */

#ifndef WEATHER_H
#define WEATHER_H

#include <stdbool.h>
#include <stdint.h>

typedef enum : uint8_t {
  WEATHER_OK = 0,
  WEATHER_ERROR = 1,
} weather_status_t;

/* @brief Initializes all drivers which are used to measure weather parameters.
 *
 * This function initializes the weather monitoring system.
 *
 * @return WEATHER_OK
 */
weather_status_t weather_init(void);

/* @brief Measures weather parameters.
 *
 * This function measures temperature, humidity, and light intensity.
 *
 * @param[out] p_temperature_celisus Pointer to store the measured temperature
 *                                   in Celsius.
 * @param[out] p_humidity_percent Pointer to store the measured humidity
 *                                percentage.
 * @param[out] p_light_percent Pointer to store the measured light intensity
 *                             percentage.
 * @return WEATHER_OK
 */
weather_status_t weather_measure(uint8_t &p_temperature_celisus,
                                 uint8_t &p_humidity_percent,
                                 uint8_t &p_light_percent);

#endif /* WEATHER_H */
