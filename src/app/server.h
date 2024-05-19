/**
 * @file server.h
 * @brief Application layer to manage TCP server and WiFi access point
 * @author Karim M. Ali <https://github.com/kmuali/>
 * @date May 19, 2024
 */

#ifndef SERVER_H
#define SERVER_H

#include "../hal/ds1307.h"
#include "weather.h"
#include <stdint.h>

#define SERVER_PORT_STR "12345"
#define SERVER_SSID_STR "Weather_AP"
#define SERVER_PASSWD_STR "10203040"

typedef enum : uint8_t {
  SERVER_OK = 0,
  SERVER_ERROR = 1,
} server_status_t;

typedef union {
  uint8_t as_array[3];
  struct {
    uint8_t temperature, humidity, light;
  } as_struct;
} server_entry_data_t;

typedef struct {
  RTC_Time_t timestamp;
  server_entry_data_t data;
} server_entry_t;

server_status_t server_init(void);
server_status_t server_run(void (*h_get_entry)(uint8_t index,
                                               server_entry_t *p_entry));
server_status_t server_kill(void);

#endif /* SERVER_H */
