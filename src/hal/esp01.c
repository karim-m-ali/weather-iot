/**
 * @file esp01.c
 * @brief Driver of ESP-01 WiFi Module
 * @author Karim M. Ali <https://github.com/kmuali/>
 * @date May 3, 2024
 */

#include "esp01.h"
#include "../mcal/usart.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define END(array) (array + sizeof(array))
#define ABS(data) ((data) < 0 ? -(data) : (data))

static volatile uint8_t g_rx_buf[ESP01_RX_BUF_SIZE], *gp_rx_buf_itr;
static volatile uint8_t gb_rx_buf_overflow;
static const char *(*gh_respond)(const char *) = NULL;
static void esp01_rx_complete_isr(void);

static esp01_status_t esp01_tx_str(const char *str) {
  for (; *str; ++str) {
    usart_tx(*str);
  }
  return ESP01_OK;
}

static esp01_status_t esp01_rx_g_buf(void) {
  gb_rx_buf_overflow = 0;

  for (gp_rx_buf_itr = g_rx_buf; gp_rx_buf_itr != END(g_rx_buf);
       ++gp_rx_buf_itr) {
    if (usart_rx((uint8_t *)gp_rx_buf_itr) == USART_RX_TIMEOUT) {
      *gp_rx_buf_itr = '\0';
      return ESP01_OK;
    }
  }

  *(END(g_rx_buf) - 1) = '\0';
  gb_rx_buf_overflow = 1;

  return ESP01_DROP;
}

static esp01_status_t esp01_parse_g_buf(void) {
  if (gb_rx_buf_overflow) {
    return ESP01_DROP;
  }
  if (strstr((char *)g_rx_buf, "\r\nOK\r\n") != NULL) {
    return ESP01_OK;
  }
  if (strstr((char *)g_rx_buf, "\r\nERROR\r\n") != NULL) {
    return ESP01_ERROR;
  }
  return ESP01_DROP;
}

static esp01_status_t esp01_init(void) {
  if (usart_init(ESP01_BAUD_RATE, USART_PARITY_NONE, 0, 0, 0, 0, 0, 0, 0) !=
          USART_OK ||
      usart_configure_isr(NULL, NULL, NULL) != USART_OK) {
    return ESP01_ERROR;
  }
  return ESP01_OK;
}
esp01_status_t esp01_init_as_access_point(const char *str_ssid,
                                          const char *str_pass) {
  esp01_status_t status = esp01_init();
  if (status != ESP01_OK) {
    return status;
  }
  if (gh_respond != NULL) {
    return ESP01_DROP;
  }
  esp01_tx_str("AT+CWMODE=2\r\n");
  esp01_rx_g_buf();
  status = esp01_parse_g_buf();
  if (status != ESP01_OK) {
    return status;
  }

  esp01_tx_str("AT+CWSAP=");
  esp01_tx_str("\"");
  esp01_tx_str(str_ssid);
  esp01_tx_str("\",\"");
  esp01_tx_str(str_pass);
  esp01_tx_str("\",11,4\r\n");
  esp01_rx_g_buf();
  return esp01_parse_g_buf();
}

esp01_status_t esp01_run_server(const char *str_port,
                                const char *(*h_respond)(const char *)) {
  if (h_respond == NULL) {
    return ESP01_ERROR;
  }

  gh_respond = h_respond;

  esp01_tx_str("AT+CIPMUX=1\r\n");
  esp01_rx_g_buf();
  esp01_status_t status = esp01_parse_g_buf();
  if (status != ESP01_OK) {
    return status;
  }

  esp01_tx_str("AT+CIPSERVER=1,");
  esp01_tx_str(str_port);
  esp01_tx_str("\r\n");
  esp01_rx_g_buf();

  usart_configure_isr(esp01_rx_complete_isr, NULL, NULL);

  return esp01_parse_g_buf();
}

esp01_status_t esp01_kill_server(void) {
  if (gh_respond == NULL) {
    return ESP01_OK;
  }
  usart_configure_isr(NULL, NULL, NULL);
  gh_respond = NULL;
  esp01_tx_str("AT+CIPSERVER=0\r\n");
  esp01_rx_g_buf();
  return esp01_parse_g_buf();
}

inline static esp01_status_t esp01_server_routine(void) {
  if (esp01_rx_g_buf() == ESP01_DROP) {
    return ESP01_DROP;
  }

  if (gp_rx_buf_itr == g_rx_buf) {
    return ESP01_OK;
  }

  if (strstr((char *)g_rx_buf, "CLOSED\r\n") == NULL &&
      strstr((char *)g_rx_buf, "CONNECT\r\n") == NULL) {
    return ESP01_DROP;
  }

  char *p_str_ipd = strstr((char *)g_rx_buf, "+IPD");
  if (p_str_ipd == NULL) {
    return ESP01_OK;
  }

  char *p_id_iter = strchr(p_str_ipd, ',') + 1;
  char *p_len_iter = strchr(p_id_iter, ',') + 1;
  char *p_data_iter = strchr(p_len_iter, ':') + 1;

  *(p_data_iter - 1) = '\0';
  uint8_t len = atoi(p_len_iter);

  *(p_len_iter - 1) = '\0';
  uint8_t id = atoi(p_id_iter);

  if (len > ABS((char *)END(g_rx_buf) - (char *)p_data_iter)) {
    return ESP01_DROP;
  }

  *(p_data_iter + len) = '\0';

  const char *replay = gh_respond(p_data_iter);
  uint8_t replay_len = strlen(replay);

  if (!replay_len) {
    return ESP01_OK;
  }

  char str_buf[10];
  sprintf(str_buf, "%i,%i", id, replay_len);

  esp01_tx_str("AT+CIPSEND=");
  esp01_tx_str(str_buf);
  esp01_tx_str("\r\n");
  if (esp01_rx_g_buf() != ESP01_OK) {
    return ESP01_ERROR;
  }

  esp01_tx_str(replay);
  esp01_tx_str("\r\n");

  return esp01_rx_g_buf();
}

static void esp01_rx_complete_isr(void) { esp01_server_routine(); }
