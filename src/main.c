/**
 * @file main.c
 * @brief Testing Implemented Drivers
 */

#include "hal/lcd.h"
#include "mcal/gpio.h"
#include <util/delay.h>

int main(void) {
  lcd_init();
  lcd_str("Hello");
  while (1) {
  }
}
