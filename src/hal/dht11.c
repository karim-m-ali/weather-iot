/**
 * @file dht11.c
 * @brief Driver of DHT11 - Humidity and Temperature Sensor
 * @author Karim M. Ali <https://github.com/kmuali/>
 * @date May 8, 2024
 */

#include "dht11.h"
#include "util/delay.h"
#include <stddef.h>
#include <stdint.h>

static dht11_status_t dht11_rx_byte(uint8_t *p_data);

/* -------- Interface Functions ---------- */
dht11_status_t dht11_init(void) { return DHT11_OK; }

dht11_status_t dht11_read(uint8_t *p_temperature, uint8_t *p_humidity) {
  if (p_temperature == NULL || p_humidity == NULL) {
    return DHT11_ERROR;
  }
  /* Transmit request pulse */
  /* NOTE: Pulling low is at least 18ms so that DHT11 is reset.
   */
  gpio_set_pin_direction(DHT11_PORT, DHT11_PIN, 1);
  _delay_ms(18);
  gpio_set_pin_level(DHT11_PORT, DHT11_PIN, 0);
  _delay_ms(18); // at least 18ms to be detected
  gpio_set_pin_direction(DHT11_PORT, DHT11_PIN, 0);

  /* Receive response pulse */
  uint8_t b_is_high = 1;
  while (b_is_high) {
    gpio_get_pin_level(DHT11_PORT, DHT11_PIN, &b_is_high);
  }
  while (!b_is_high) {
    gpio_get_pin_level(DHT11_PORT, DHT11_PIN, &b_is_high);
  }
  while (b_is_high) {
    gpio_get_pin_level(DHT11_PORT, DHT11_PIN, &b_is_high);
  }

  /* Receive data */
  enum {
    RX_HUMIDITY_INTEGRAL,
    RX_HUMIDITY_DECIMAL,
    RX_TEMPERATURE_INTEGRAL,
    RX_TEMPERATURE_DECIMAL,
    RX_CHECKSUM,
    RX_BYTES_SIZE,
  };
  uint8_t checksum = 0;
  uint8_t rx_bytes[RX_BYTES_SIZE];

  for (uint8_t index = 0; index < RX_BYTES_SIZE; ++index) {
    if (dht11_rx_byte(&rx_bytes[index]) != DHT11_OK) {
      return DHT11_ERROR;
    }
    if (index < RX_CHECKSUM) {
      checksum += rx_bytes[index];
    }
  }

  /* Validate data */
  if (checksum != rx_bytes[RX_CHECKSUM]) {
    return DHT11_ERROR;
  }

  *p_humidity = rx_bytes[RX_HUMIDITY_INTEGRAL];
  *p_temperature = rx_bytes[RX_TEMPERATURE_INTEGRAL];

  return DHT11_OK;
}

/* -------- Static Functions ---------- */
static inline dht11_status_t dht11_rx_edge(uint16_t *p_iter_count,
                                           uint8_t b_wait_for_rising_edge) {
  uint8_t b_is_high = 0;
  for (*p_iter_count = 0; *p_iter_count < 0xFF; ++(*p_iter_count)) {
    gpio_get_pin_level(DHT11_PORT, DHT11_PIN, &b_is_high);
    if (!!b_is_high == !!b_wait_for_rising_edge) {
      break;
    }
  }
  if (*p_iter_count == 0xff) {
    return DHT11_ERROR;
  }
  return DHT11_OK;
}

static inline dht11_status_t dht11_rx_pulse(uint16_t *p_iter_count) {
  if (dht11_rx_edge(p_iter_count, 1) != DHT11_OK) {
    return DHT11_ERROR;
  }
  return dht11_rx_edge(p_iter_count, 0);
}

static inline dht11_status_t dht11_rx_bit(uint8_t *pb_is_high) {
  uint16_t iter_count;
  if (dht11_rx_pulse(&iter_count) != DHT11_OK) {
    return DHT11_ERROR;
  }
  /* NOTE: Sensor output of logic 0 takes ~26-28us and
   *       output of logic 1 takes ~70us.
   */
  *pb_is_high = iter_count >= 10;
  return DHT11_OK;
}

static inline dht11_status_t dht11_rx_byte(uint8_t *p_data) {
  *p_data = 0;
  uint8_t b_is_high;
  gpio_set_pin_level(DHT11_PORT, DHT11_PIN, 1);
  for (int8_t bit = 7; bit >= 0; --bit) {
    if (dht11_rx_bit(&b_is_high) != DHT11_OK) {
      return DHT11_ERROR;
    }
    *p_data |= !!b_is_high << bit;
  }
  return DHT11_OK;
}
