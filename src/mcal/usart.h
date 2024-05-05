/**
 * @file usart.h
 * @brief Driver of Universal Synchronous/Asynchronous Receiver/Transmitter
 * @author Karim M. Ali <https://github.com/kmuali/>
 * @date May 3, 2024
 */

#ifndef USART_H
#define USART_H

#include <stddef.h>
#include <stdint.h>

#define USART_BAUD_RATE_MAX (uint32_t)1000000
#define USART_BAUD_RATE_MIN (uint32_t)2400
#define USART_RX_TIMEOUT_TICKS_MAX ((uint16_t)(F_CPU / 1e3))

typedef enum : uint8_t {
  USART_OK = 0,
  USART_ERROR = 1,
  USART_RX_TIMEOUT,
} usart_status_t;

typedef enum : uint8_t {
  USART_PARITY_NONE = 0,
  USART_PARITY_EVEN = 2,
  USART_PARITY_ODD = 3,
} usart_parity_t;

typedef struct {
  volatile uint8_t b_rx_complete : 1, b_tx_complete : 1, b_data_reg_empty : 1,
      b_frame_error : 1, b_data_overrun : 1, b_parity_error : 1, unused : 2;
} usart_flags_t;

usart_status_t usart_init(uint32_t baud_rate_bps, usart_parity_t parity,
                          uint8_t b_two_stop_bits, uint8_t b_asynchronous,
                          uint8_t b_double_speed, uint8_t b_multi_processor,
                          uint8_t b_active_low_clock, uint8_t b_disable_tx,
                          uint8_t b_disable_rx);
usart_status_t usart_configure_isr(void (*p_rx_complete_isr)(void),
                                   void (*p_tx_complete_isr)(void),
                                   void (*p_tx_ready_isr)(void));
usart_status_t usart_get_flags(usart_flags_t *p_flags);
usart_status_t usart_tx(uint8_t data);
usart_status_t usart_rx(uint8_t *p_data);

#endif
