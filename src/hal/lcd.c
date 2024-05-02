/**
 * @file lcd.c
 * @brief Driver of Character LCD 4x20
 * @author Karim M. Ali <https://github.com/kmuali/>
 * @date March 8, 2024
 */

#include "lcd.h"
#include <stdint.h>
#include <util/delay.h>

#define LCD_ROWS_NUM 4
#define LCD_COLS_NUM 20
#define LCD_CUSTOM_CHARS_NUM 8

inline static void lcd_data(uint8_t data) {
  gpio_set_pin_level(LCD_PORT_DATA, LCD_PIN_D4, data & 0x10);
  gpio_set_pin_level(LCD_PORT_DATA, LCD_PIN_D5, data & 0x20);
  gpio_set_pin_level(LCD_PORT_DATA, LCD_PIN_D6, data & 0x40);
  gpio_set_pin_level(LCD_PORT_DATA, LCD_PIN_D7, data & 0x80);

  gpio_set_pin_level(LCD_PORT_CONTROL, LCD_PIN_EN, 0);
  _delay_us(40);
  gpio_set_pin_level(LCD_PORT_CONTROL, LCD_PIN_EN, 1);
  _delay_us(1);

  gpio_set_pin_level(LCD_PORT_DATA, LCD_PIN_D4, data & 0x01);
  gpio_set_pin_level(LCD_PORT_DATA, LCD_PIN_D5, data & 0x02);
  gpio_set_pin_level(LCD_PORT_DATA, LCD_PIN_D6, data & 0x04);
  gpio_set_pin_level(LCD_PORT_DATA, LCD_PIN_D7, data & 0x08);

  gpio_set_pin_level(LCD_PORT_CONTROL, LCD_PIN_EN, 0);
  _delay_us(40);
  gpio_set_pin_level(LCD_PORT_CONTROL, LCD_PIN_EN, 1);
  _delay_us(1);
}

lcd_status_t lcd_init(void) {
  gpio_set_pin_direction(LCD_PORT_CONTROL, LCD_PIN_RS, true);
  gpio_set_pin_direction(LCD_PORT_CONTROL, LCD_PIN_EN, true);

  gpio_set_pin_direction(LCD_PORT_DATA, LCD_PIN_D7, true);
  gpio_set_pin_direction(LCD_PORT_DATA, LCD_PIN_D6, true);
  gpio_set_pin_direction(LCD_PORT_DATA, LCD_PIN_D5, true);
  gpio_set_pin_direction(LCD_PORT_DATA, LCD_PIN_D4, true);

  _delay_ms(40); // Wait for the LCD module to power up.

  lcd_command(LCD_CLEAR_DISPLAY);
  lcd_command(LCD_RETURN_HOME);

  lcd_command(LCD_SET_4_BIT_FONT_5X10);

  lcd_command(LCD_DISPLAY_ON_CURSOR_HIDE);
  lcd_command(LCD_CLEAR_DISPLAY);
  lcd_command(LCD_RETURN_HOME);

  return LCD_OK;
}

lcd_status_t lcd_command(lcd_command_t command) {
  gpio_set_pin_level(LCD_PORT_CONTROL, LCD_PIN_RS, 0);
  lcd_data(command);
  if (command == LCD_CLEAR_DISPLAY || command == LCD_RETURN_HOME) {
    _delay_ms(1.52);
  }
  return LCD_OK;
}

lcd_status_t lcd_char(char character) {
  gpio_set_pin_level(LCD_PORT_CONTROL, LCD_PIN_RS, 1);
  lcd_data(character);
  return LCD_OK;
}

lcd_status_t lcd_str(char *str) {
  gpio_set_pin_level(LCD_PORT_CONTROL, LCD_PIN_RS, 1);
  for (; *str; str++) {
    lcd_data(*str);
  }
  return LCD_OK;
}

lcd_status_t lcd_locate_cursor(uint8_t row, uint8_t col) {
  if (col >= LCD_ROWS_NUM) {
    return LCD_ERROR;
  }
  switch (row) {
  case 0:
    lcd_command(LCD_SET_DDRAM_LINE_0 + col);
    break;
  case 1:
    lcd_command(LCD_SET_DDRAM_LINE_1 + col);
    break;
  case 2:
    lcd_command(LCD_SET_DDRAM_LINE_2 + col);
    break;
  case 3:
    lcd_command(LCD_SET_DDRAM_LINE_3 + col);
    break;
  default:
    return LCD_ERROR;
  }
  return LCD_OK;
}

inline lcd_status_t lcd_locate_char(uint8_t row, uint8_t col, char character) {
  lcd_status_t status = lcd_locate_cursor(row, col);
  if (status != LCD_OK) {
    return status;
  }
  status = lcd_char(character);
  return status;
}

inline lcd_status_t lcd_locate_str(uint8_t row, uint8_t col, char *str) {
  lcd_status_t status;
  status = lcd_locate_cursor(row, col);
  if (status == LCD_OK) {
    status = lcd_str(str);
  }
  return status;
}

lcd_status_t lcd_text(char *text, char padding) {
  uint8_t row, col;
  for (row = 0; row < LCD_ROWS_NUM; ++row) {
    lcd_locate_cursor(row, 0);
    for (col = 0; col < LCD_COLS_NUM; ++col) {
      lcd_char(*text ? *(text++) : padding);
    }
  }
  if (*text) {
    return LCD_TEXT_OVERFLOW;
  }
  return LCD_OK;
}

lcd_status_t lcd_custom_char(uint8_t char_code, uint8_t dot_matrix[8]) {
  if (char_code >= LCD_CUSTOM_CHARS_NUM) {
    return LCD_ERROR;
  }
  lcd_command(LCD_SET_CGRAM + char_code * 8);
  for (uint8_t row = 0; row < 8; ++row) {
    lcd_char(dot_matrix[row]);
  }
  return LCD_OK;
}
