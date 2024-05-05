/**
 * @file usart.c
 * @brief Driver of Universal Synchronous/Asynchronous Receiver/Transmitter
 * @author Karim M. Ali <https://github.com/kmuali/>
 * @date May 3, 2024
 */

#include "usart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

static void (*gp_rx_complete_isr)(void);
static void (*gp_tx_complete_isr)(void);
static void (*gp_tx_ready_isr)(void);

usart_status_t usart_init(uint32_t baud_rate_bps, usart_parity_t parity,
                          uint8_t b_two_stop_bits, uint8_t b_asynchronous,
                          uint8_t b_double_speed, uint8_t b_multi_processor,
                          uint8_t b_active_low_clock, uint8_t b_disable_tx,
                          uint8_t b_disable_rx) {
  if (baud_rate_bps < USART_BAUD_RATE_MIN ||
      baud_rate_bps > USART_BAUD_RATE_MAX ||
      (b_asynchronous && b_double_speed) || (b_disable_tx && b_disable_rx) ||
      (b_asynchronous && b_active_low_clock)) {
    return USART_ERROR;
  }
  switch (parity) {
  case USART_PARITY_NONE:
  case USART_PARITY_EVEN:
  case USART_PARITY_ODD:
    break;
  default:
    return USART_ERROR;
  }

  uint8_t factor = b_asynchronous ? 2 : b_double_speed ? 8 : 16;
  uint32_t ubrr = F_CPU / (factor * baud_rate_bps) - 1;

  UBRRH = ubrr >> 8;
  UBRRL = ubrr;

  UCSRA &= ~(1 << U2X | 1 << MPCM);
  UCSRA |= !!b_double_speed << U2X | !!b_multi_processor << MPCM;

  UCSRB = !b_disable_rx << RXEN | !b_disable_tx << TXEN;

  UCSRC = 1 << URSEL | !!b_asynchronous << UMSEL | !!(parity & 2) << UPM1 |
          !!(parity & 1) << UPM0 | !!b_two_stop_bits << USBS | 1 << UCSZ1 |
          1 << UCSZ0 | !!b_active_low_clock << UCPOL;

  return USART_OK;
}

usart_status_t usart_configure_isr(void (*p_rx_complete_isr)(void),
                                   void (*p_tx_complete_isr)(void),
                                   void (*p_tx_ready_isr)(void)) {
  gp_rx_complete_isr = p_rx_complete_isr;
  gp_tx_complete_isr = p_tx_complete_isr;
  gp_tx_ready_isr = p_tx_ready_isr;

  UCSRB &= ~(1 << RXCIE | 1 << TXCIE | 1 << UDRIE);
  UCSRB |= (p_rx_complete_isr != NULL) << RXCIE |
           (p_tx_complete_isr != NULL) << TXCIE |
           (p_tx_ready_isr != NULL) << UDRIE;

  if (p_rx_complete_isr != NULL || p_tx_complete_isr != NULL ||
      p_tx_ready_isr != NULL) {
    sei();
  }

  return USART_OK;
}

usart_status_t usart_get_flags(usart_flags_t *p_flags) {
  if (p_flags == NULL) {
    return USART_ERROR;
  }
  *((volatile uint8_t *)p_flags) = UCSRA;
  return USART_OK;
}

usart_status_t usart_tx(uint8_t data) {
  while (!(UCSRA & (1 << UDRE))) {
    /* polling to data ready */
  }
  UDR = data;
  return USART_OK;
}

usart_status_t usart_rx(uint8_t *p_data) {
  if (p_data == NULL) {
    return USART_ERROR;
  }
  uint16_t timeout_ticks = 0;
  while (!(UCSRA & (1 << RXC))) {
    /* polling to receive complete */
    if (timeout_ticks++ == USART_RX_TIMEOUT_TICKS_MAX) {
      return USART_RX_TIMEOUT;
    }
  }
  *p_data = UDR;
  return USART_OK;
}

ISR(USART_RXC_vect) { gp_rx_complete_isr(); }
ISR(USART_TXC_vect) { gp_tx_complete_isr(); }
ISR(USART_UDRE_vect) { gp_tx_ready_isr(); }
