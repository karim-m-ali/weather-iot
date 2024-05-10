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
#include "../hal/ds1307.h"

// Circular queue indices
static uint8_t storage_front = 0;
static uint8_t storage_rear = 0;

// Length of data
static uint8_t LengthOfData = 0;

// Function to calculate EEPROM address for a given block index
static uint16_t get_eeprom_address(uint8_t block_index) {
    if (block_index >= TOTAL_BLOCKS) {
        return STORAGE_ERROR;
    }
    return STORAGE_BASE_ADDRESS + (block_index * STORAGE_BLOCK_SIZE);
}

// Function to increment a circular queue index
static uint8_t increment_index(uint8_t index) {
    return (index + 1) % TOTAL_BLOCKS;
}

// Function to convert BCD to integer
static uint8_t bcd_to_int(uint8_t bcd) {
    return ((uint8_t)((bcd >> 4) * 10)) + ((uint8_t)(bcd & 0x0F));
}

// Function to initialize storage system
storage_status_t storage_init(void) {
    // Initialize EEPROM
    eeprom_busy_wait();
    
    // Read last version of rear and data length
    storage_rear = eeprom_read_byte(STORAGE_REAR_ADDRESS);
    LengthOfData = eeprom_read_byte(STORAGE_DATA_LENGTH_ADDRESS);
    
    return STORAGE_OK;
}

// Function to add a block of data
storage_status_t storage_enqueue_block(const uint8_t *p_data) {
    // Calculate timestamp
    RTC_Time_t timestamp;
    RTC_getTime(&timestamp);
    uint8_t arr[7] = {bcd_to_int(timestamp->timeArr[0]), bcd_to_int(timestamp->timeArr[1]),
                      bcd_to_int(timestamp->timeArr[2]), bcd_to_int(timestamp->timeArr[3]),
                      bcd_to_int(timestamp->timeArr[4]), bcd_to_int(timestamp->timeArr[5]),
                      bcd_to_int(timestamp->timeArr[6])};

    // Calculate EEPROM address for the given block index
    uint16_t eeprom_address = get_eeprom_address(storage_rear);
    
    // Write data to EEPROM
    eeprom_write_block(p_data, eeprom_address, STORAGE_BLOCK_DATA_SIZE);
    eeprom_address += STORAGE_BLOCK_DATA_SIZE;
    
    // Write timestamp to EEPROM
    eeprom_write_block(arr, eeprom_address, sizeof(arr));
    
    // Update storage rear
    storage_rear = increment_index(storage_rear);
    eeprom_write_byte(STORAGE_REAR_ADDRESS, storage_rear);
    
    // Update storage length of data
    if(LengthOfData >= TOTAL_BLOCKS){
        eeprom_write_byte(STORAGE_DATA_LENGTH_ADDRESS, LengthOfData);
    }
    else{
        eeprom_write_byte(STORAGE_DATA_LENGTH_ADDRESS, ++LengthOfData);
    }
    
    return STORAGE_OK;
}

// Function to get the number of blocks stored in circular queue
storage_status_t storage_get_length(uint8_t *p_length) {
    // Calculate length of data stored in circular queue
    *p_length = (LengthOfData >= TOTAL_BLOCKS) ? (TOTAL_BLOCKS) : (storage_rear - storage_front);
    
    return STORAGE_OK;
}

// Function to get block of data
storage_status_t storage_get_block(uint8_t index, uint8_t *p_data, timestamp_t *p_timestamp) {
    // Check if index is valid
    if (index >= TOTAL_BLOCKS) {
        return STORAGE_ERROR;
    }

    // Calculate EEPROM address for the given block index
    uint16_t eeprom_address = get_eeprom_address(index);
    
    // Read data from EEPROM
    eeprom_read_block(p_data, eeprom_address, STORAGE_BLOCK_DATA_SIZE);
    eeprom_address += STORAGE_BLOCK_DATA_SIZE;
    
    // Read timestamp from EEPROM
    uint8_t arr[7];
    eeprom_read_block(arr, eeprom_address, sizeof(arr));
    for (int i = 0; i < 7; i++)
    {
    	p_timestamp->timestampArr[i] = arr[i];
    }
    
    return STORAGE_OK;
}
