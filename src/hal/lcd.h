/**
 * @file lcd.c
 * @brief Driver of Character LCD 4x20
 * @author Karim M. Ali <https://github.com/kmuali/>
 * @date March 8, 2024
 */

#ifndef LCD_H
#define LCD_H

#include "../mcal/gpio.h"
#include <stdint.h>

/* Define control pins. */
#define LCD_PORT_CONTROL GPIO_PORT_A
#define LCD_PIN_RS GPIO_PIN_3
#define LCD_PIN_EN GPIO_PIN_2

/* Define data pins. */
#define LCD_PORT_DATA GPIO_PORT_B
#define LCD_PIN_D4 GPIO_PIN_0
#define LCD_PIN_D5 GPIO_PIN_1
#define LCD_PIN_D6 GPIO_PIN_2
#define LCD_PIN_D7 GPIO_PIN_4

typedef enum : uint8_t {
  LCD_OK,
  LCD_ERROR,
  LCD_TEXT_OVERFLOW,
} lcd_status_t;

typedef enum : uint8_t {
  /* Clears entire display and sets DDRAM address 0 in address counter.
   */
  LCD_CLEAR_DISPLAY = 0x01,

  /* Sets DDRAM address 0 in address counter.
   * Also returns display from being shifted to original position.
   * DDRAM contents remain unchanged.
   */
  LCD_RETURN_HOME = 0x02,

  /* Sets cursor move direction and specifies display shift.
   * These operations are performed during data write and read.
   */
  LCD_ENTRY_MODE_NONE = 0x04,
  LCD_ENTRY_MODE_INCREMENT = 0x07,
  LCD_ENTRY_MODE_DECREMENT = 0x05,

  /* Sets entire display (D) on/off, cursor on/off (C), and blinking of cursor
   * position character (B).
   */
  LCD_DISPLAY_OFF = 0x08,
  LCD_DISPLAY_ON_CURSOR_HIDE = 0x0C,
  LCD_DISPLAY_ON_CURSOR_SHOW = 0x0E,
  LCD_DISPLAY_ON_CURSOR_BLINK = 0x0F,

  /* Moves cursor and shifts display without changing DDRAM contents.
   */
  LCD_SHIFT_LEFT = 0x18,
  LCD_SHIFT_RIGHT = 0x1C,

  /* Sets interface data length (DL), number of display lines (N),
   * and character font (F).
   */
  LCD_SET_8_BIT_FONT_5X10 = 0x3C,
  LCD_SET_8_BIT_FONT_5X8 = 0x38,
  LCD_SET_4_BIT_FONT_5X10 = 0x2C,
  LCD_SET_4_BIT_FONT_5X8 = 0x28,

  /* Sets CGRAM address. CGRAM data is sent and received after this setting.
   */
  LCD_SET_CGRAM = 0x40,

  /* Sets DDRAM address. DDRAM data is sent and received after this setting.
   */
  LCD_SET_DDRAM_LINE_0 = 0x80,
  LCD_SET_DDRAM_LINE_1 = 0xC0,
  LCD_SET_DDRAM_LINE_2 = 0x94,
  LCD_SET_DDRAM_LINE_3 = 0xD4,
} lcd_command_t;

lcd_status_t lcd_init(void);
lcd_status_t lcd_char(char character);
lcd_status_t lcd_str(char *str);
lcd_status_t lcd_locate_cursor(uint8_t row, uint8_t col);
lcd_status_t lcd_locate_char(uint8_t row, uint8_t col, char character);
lcd_status_t lcd_locate_str(uint8_t row, uint8_t col, char *str);
lcd_status_t lcd_command(lcd_command_t command);
lcd_status_t lcd_text(char *text, char padding);
lcd_status_t lcd_custom_char(uint8_t char_code, uint8_t dot_matrix[8]);

#endif /* LCD_H */
