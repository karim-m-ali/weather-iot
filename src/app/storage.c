/**
 * @file storage.h
 * @brief Functions of storage measurement.
 *
 * This file contains definitions for functions related to storage measurement
 * and management.
 * It stores any data along with current data and time automatically.
 * It uses circular queue to handle storage in EEPROM.
 *
 * @author Mahmoud Gamal
 * @date May 10 2024
 */

#include "storage.h"
#include "../mcal/twi.h"
#include <avr/eeprom.h>
#include <stdint.h>

// Circular queue indices
static uint8_t g_storage_cursor = 0;

// Function to calculate EEPROM address for a given block index
static storage_status_t get_eeprom_address(uint8_t block_index, 
    void **pp_address) {
    if (block_index >= TOTAL_BLOCKS) {
        return STORAGE_ERROR;
    }
    *pp_address = (void *)(STORAGE_BASE_ADDRESS + (block_index * 
          STORAGE_BLOCK_SIZE));
    return STORAGE_OK;
}

// Function to increment a circular queue index
static void move_cursor(void) {
    g_storage_cursor = (g_storage_cursor + 1) % TOTAL_BLOCKS;
    eeprom_write_byte((void *)STORAGE_CURSOR_ADDRESS, g_storage_cursor);
}


// Function to initialize storage system
storage_status_t storage_init(void) {
    // Initialize EEPROM
    eeprom_busy_wait();

    // Read last version of rear and data length
    g_storage_cursor = eeprom_read_byte((const uint8_t *)STORAGE_CURSOR_ADDRESS);

    TWI_ConfigType twi_cfg = {.address = RTC_TWI_ADDRESS, .bit_rate = 100};
    TWI_init(&twi_cfg);
    
    return STORAGE_OK;
}

// Function to add a block of data
storage_status_t storage_enqueue_block(const uint8_t *p_data) {
  if (p_data == NULL) {
    return STORAGE_ERROR;
  }
    // Calculate timestamp
    RTC_Time_t timestamp;
    if (RTC_getTime(&timestamp) != RTC_SUCCESS) {
      return STORAGE_ERROR;
    }

    // Calculate EEPROM address for the given block index
    void *eeprom_address;
    if (get_eeprom_address(g_storage_cursor, &eeprom_address) != STORAGE_OK) {
      return STORAGE_ERROR;
    }
    
    // Write data to EEPROM
    eeprom_write_block(p_data, eeprom_address, STORAGE_BLOCK_DATA_SIZE);
    eeprom_address += STORAGE_BLOCK_DATA_SIZE;
    
    // Write timestamp to EEPROM
    eeprom_write_block(timestamp.timeArr, eeprom_address, sizeof(timestamp.timeArr));
    
    // Update storage rear
    move_cursor();
    
    return STORAGE_OK;
}

// Function to get the number of blocks stored in circular queue
storage_status_t storage_get_length(uint8_t *p_length) {
  if (p_length == NULL) {
    return STORAGE_ERROR;
  }
    // Calculate length of data stored in circular queue
    *p_length = TOTAL_BLOCKS;
    
    return STORAGE_OK;
}

// Function to get block of data
storage_status_t storage_get_block(uint8_t index, uint8_t *p_data, 
    RTC_Time_t *p_timestamp) {
    // Check if index is valid
    if (index >= TOTAL_BLOCKS || p_data == NULL) {
        return STORAGE_ERROR;
    }

    // Calculate EEPROM address for the given block index
    void *eeprom_address;
    get_eeprom_address(index, &eeprom_address);
    
    // Read data from EEPROM
    eeprom_read_block(p_data, eeprom_address, STORAGE_BLOCK_DATA_SIZE);
    eeprom_address += STORAGE_BLOCK_DATA_SIZE;
    
    // Read timestamp from EEPROM
    if (p_timestamp != NULL) {
      eeprom_read_block(p_timestamp->timeArr, eeprom_address, 
          sizeof(p_timestamp->timeArr));
    }
    
    return STORAGE_OK;
}
