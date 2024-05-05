/**
 * @file esp01.h
 * @brief Driver of ESP-01 WiFi Module
 * @author Karim M. Ali <https://github.com/kmuali/>
 * @date May 3, 2024
 */

#ifndef ESP01_H
#define ESP01_H

#include <stdint.h>

/* This is application depended. */
#define ESP01_RX_BUF_SIZE 200

#define ESP01_BAUD_RATE 9600

typedef enum : uint8_t {
  ESP01_OK = 0,
  ESP01_ERROR,
  ESP01_DROP,
} esp01_status_t;

esp01_status_t esp01_init_as_access_point(const char *str_ssid,
                                          const char *str_pass);
esp01_status_t esp01_run_server(const char *str_port,
                                const char *(*h_respond)(const char *));
esp01_status_t esp01_kill_server(void);

#endif /* ESP01_H */
