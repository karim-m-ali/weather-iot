/**
 * @file gpio.c
 * @brief Driver of General Purpose Input Output
 * @author Karim M. Ali <https://github.com/kmuali/>
 * @date March 8, 2024
 */

#include "gpio.h"

#include <avr/io.h>
#include <stdint.h>

#define SET_REG8(REG, LEVEL) ((LEVEL) ? (REG = 0xFF) : (REG = 0x00))

#define SET_PIN_HIGH(REG, PIN) (REG |= (1 << (PIN)))

#define SET_PIN_LOW(REG, PIN) (REG &= ~(1 << (PIN)))

#define SET_PIN(REG, PIN, LEVEL)                                               \
  ((LEVEL) ? SET_PIN_HIGH(REG, PIN) : SET_PIN_LOW(REG, PIN))

#define GET_PIN(REG, PIN) (!!(REG & (1 << (PIN))))

/* Port Functions */

inline gpio_status_t gpio_set_port_direction(gpio_port_t port,
                                             uint8_t b_is_out) {
  switch (port) {
  case GPIO_PORT_A:
    SET_REG8(DDRA, b_is_out);
    break;
  case GPIO_PORT_B:
    SET_REG8(DDRB, b_is_out);
    break;
  case GPIO_PORT_C:
    SET_REG8(DDRC, b_is_out);
    break;
  case GPIO_PORT_D:
    SET_REG8(DDRD, b_is_out);
    break;
  default:
    return GPIO_ERROR;
  }
  return GPIO_OK;
}

inline gpio_status_t gpio_set_port_data(gpio_port_t port, uint8_t data) {
  switch (port) {
  case GPIO_PORT_A:
    PORTA = data;
    break;
  case GPIO_PORT_B:
    PORTB = data;
    break;
  case GPIO_PORT_C:
    PORTC = data;
    break;
  case GPIO_PORT_D:
    PORTD = data;
    break;
  default:
    return GPIO_ERROR;
  }
  return GPIO_OK;
}

inline gpio_status_t gpio_get_port_data(gpio_port_t port, uint8_t *p_data) {
  switch (port) {
  case GPIO_PORT_A:
    *p_data = PINA;
    break;
  case GPIO_PORT_B:
    *p_data = PINB;
    break;
  case GPIO_PORT_C:
    *p_data = PINC;
    break;
  case GPIO_PORT_D:
    *p_data = PIND;
    break;
  default:
    return GPIO_ERROR;
  }
  return GPIO_OK;
}

/* Pin Functions */

inline gpio_status_t gpio_set_pin_direction(gpio_port_t port, gpio_pin_t pin,
                                            uint8_t b_is_out) {
  if (pin > GPIO_PIN_7) {
    return GPIO_ERROR;
  }
  switch (port) {
  case GPIO_PORT_A:
    SET_PIN(DDRA, pin, b_is_out);
    break;
  case GPIO_PORT_B:
    SET_PIN(DDRB, pin, b_is_out);
    break;
  case GPIO_PORT_C:
    SET_PIN(DDRC, pin, b_is_out);
    break;
  case GPIO_PORT_D:
    SET_PIN(DDRD, pin, b_is_out);
    break;
  default:
    return GPIO_ERROR;
  }
  return GPIO_OK;
}

inline gpio_status_t gpio_set_pin_level(gpio_port_t port, gpio_pin_t pin,
                                        uint8_t b_is_high) {
  if (pin > GPIO_PIN_7) {
    return GPIO_ERROR;
  }
  switch (port) {
  case GPIO_PORT_A:
    SET_PIN(PORTA, pin, b_is_high);
    break;
  case GPIO_PORT_B:
    SET_PIN(PORTB, pin, b_is_high);
    break;
  case GPIO_PORT_C:
    SET_PIN(PORTC, pin, b_is_high);
    break;
  case GPIO_PORT_D:
    SET_PIN(PORTD, pin, b_is_high);
    break;
  default:
    return GPIO_ERROR;
  }
  return GPIO_OK;
}

inline gpio_status_t gpio_get_pin_level(gpio_port_t port, gpio_pin_t pin,
                                        uint8_t *pb_is_high) {

  if (pin > GPIO_PIN_7) {
    return GPIO_ERROR;
  }
  switch (port) {
  case GPIO_PORT_A:
    *pb_is_high = GET_PIN(PINA, pin);
    break;
  case GPIO_PORT_B:
    *pb_is_high = GET_PIN(PINB, pin);
    break;
  case GPIO_PORT_C:
    *pb_is_high = GET_PIN(PINC, pin);
    break;
  case GPIO_PORT_D:
    *pb_is_high = GET_PIN(PIND, pin);
    break;
  default:
    return GPIO_ERROR;
  }
  return GPIO_OK;
}

/* Extra Functions */

inline gpio_status_t gpio_set_pull_up(uint8_t b_is_disable) {
  SET_PIN(SFIOR, PUD, b_is_disable);
  return GPIO_OK;
}
