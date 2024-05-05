/**
 * @file storage.h
 * @brief Functions of storage measurement.
 *
 * This file contains declarations for functions related to storage measurement
 * and management.
 * It stores any data along with current data and time automatically.
 * It uses circular queue to handle storage in EEPROM.
 *
 * @author Your Name
 * @date Data of last edit
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* The size is application dependent */
#define STORAGE_BLOCK_DATA_SIZE 3

typedef enum : uint8_t {
  STORAGE_OK = 0,
  STORAGE_ERROR = 1,
} storage_status_t;

typedef struct {
  uint32_t utc_year : 12, utc_month : 4, utc_day : 5, utc_hour : 6;
  uint8_t utc_minute : 6;
} storage_timestamp_t;

storage_status_t storage_init(void);

storage_status_t storage_enqueue_block(const uint8_t *p_data);

storage_status_t storage_get_length(uint8_t *p_length);

storage_status_t storage_get_block(uint8_t index, uint8_t *p_data,
                                   storage_timestamp_t *p_timestamp);

#endif /* STORAGE_H */
