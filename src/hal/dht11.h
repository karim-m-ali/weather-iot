/**
 * @file dht11.h
 * @brief Driver of DHT11 - Humidity and Temperature Sensor
 * @author Karim M. Ali <https://github.com/kmuali/>
 * @date May 8, 2024
 */

#ifndef DHT11_H
#define DHT11_H

#include "../mcal/gpio.h"
#include <stdint.h>

#define DHT11_PORT GPIO_PORT_C
#define DHT11_PIN GPIO_PIN_6

typedef enum : uint8_t {
  DHT11_OK = 0,
  DHT11_ERROR = 1,
} dht11_status_t;

dht11_status_t dht11_init(void);

dht11_status_t dht11_read(uint8_t *p_temperature, uint8_t *p_humidity);

#endif /* DHT11_H */
