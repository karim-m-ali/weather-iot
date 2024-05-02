/**
 * @file gpio.h
 * @brief Driver of General Purpose Input Output
 * @author Karim M. Ali <https://github.com/kmuali/>
 * @date March 8, 2024
 */

#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>
#include <stdint.h>

typedef enum : uint8_t {
  GPIO_OK = 0,
  GPIO_ERROR = 1,
} gpio_status_t;

typedef enum : uint8_t {
  GPIO_PORT_A = 0,
  GPIO_PORT_B = 1,
  GPIO_PORT_C = 2,
  GPIO_PORT_D = 3,
} gpio_port_t;

typedef enum : uint8_t {
  GPIO_PIN_0 = 0,
  GPIO_PIN_1 = 1,
  GPIO_PIN_2 = 2,
  GPIO_PIN_3 = 3,
  GPIO_PIN_4 = 4,
  GPIO_PIN_5 = 5,
  GPIO_PIN_6 = 6,
  GPIO_PIN_7 = 7,
} gpio_pin_t;

/* Port Functions */

gpio_status_t gpio_set_port_direction(gpio_port_t port, uint8_t b_is_out);
gpio_status_t gpio_set_port_data(gpio_port_t port, uint8_t data);
gpio_status_t gpio_get_port_data(gpio_port_t port, uint8_t *p_data);

/* Pin Functions */

gpio_status_t gpio_set_pin_direction(gpio_port_t port, gpio_pin_t pin,
                                     uint8_t b_is_out);
gpio_status_t gpio_set_pin_level(gpio_port_t port, gpio_pin_t pin,
                                 uint8_t b_is_high);
gpio_status_t gpio_get_pin_level(gpio_port_t port, gpio_pin_t pin,
                                 uint8_t *pb_is_high);

/* Extra Functions */

gpio_status_t gpio_set_pull_up(uint8_t b_is_disable);

#endif /* GPIO_H */
