/**
 * @file server.c
 * @brief Application layer to manage TCP server and WiFi access point
 * @author Karim M. Ali <https://github.com/kmuali/>
 * @date May 19, 2024
 */

#include "server.h"
#include "../hal/esp01.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void (*gh_get_entry)(uint8_t index, server_entry_t *p_entry);
static char g_json_str[180];
static server_entry_t g_entry;

static uint8_t bcd_to_uint8(uint8_t bcd) {
  return (bcd >> 4) * 10 + (bcd & 0x0F);
}

static const char *gh_response_str(const char *request_json_str) {
  uint8_t index = 0;
  sscanf(request_json_str, "{\"index\": %hhu}", &index);
  gh_get_entry(index, &g_entry);
  sprintf(g_json_str,
          "{\"timestamp\":{\"hour\":%d,\"minute\":%d,\"second\":%d,"
          "\"dayOfMonth\":%d,\"month\":%d,\"year\":%d,\"dayOfWeek\":%d},"
          "\"data\":{\"temperature\":%d,\"humidity\":%d,\"light\":%d}}",
          bcd_to_uint8(g_entry.timestamp.time.hours),
          bcd_to_uint8(g_entry.timestamp.time.minutes),
          bcd_to_uint8(g_entry.timestamp.time.seconds),
          bcd_to_uint8(g_entry.timestamp.time.dayOfMonth),
          bcd_to_uint8(g_entry.timestamp.time.month),
          bcd_to_uint8(g_entry.timestamp.time.year),
          bcd_to_uint8(g_entry.timestamp.time.dayOfWeek),
          g_entry.data.as_struct.temperature, g_entry.data.as_struct.humidity,
          g_entry.data.as_struct.light);
  return g_json_str;
}

server_status_t server_init(void) {
  if (esp01_init_as_access_point(SERVER_SSID_STR, SERVER_PASSWD_STR) ==
      ESP01_OK) {
    return SERVER_OK;
  }
  return SERVER_ERROR;
}

server_status_t server_run(void (*h_get_entry)(uint8_t index,
                                               server_entry_t *p_entry)) {
  if (h_get_entry != NULL) {
    gh_get_entry = h_get_entry;
    if (esp01_run_server(SERVER_PORT_STR, gh_response_str) == ESP01_OK) {
      return SERVER_OK;
    }
  }
  return SERVER_ERROR;
}

server_status_t server_kill(void) {
  if (esp01_kill_server() == ESP01_OK) {
    return SERVER_OK;
  }
  return SERVER_ERROR;
}
