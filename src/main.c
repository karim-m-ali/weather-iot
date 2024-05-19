#include "app/server.h"
#include "app/storage.h"
#include "app/weather.h"
#include "hal/lcd.h"
#include <stdint.h>
#include <util/delay.h>

#define ROUTINE_FREQUENCY_MINUTES 10

void init(void);
void routine(void);

int main(void) {
  init();
  while (1) {
    routine();
#if ROUTINE_FREQUENCY_MINUTES < 10
#error "ROUTINE_FREQUENCY_MINUTES must be 10 at least"
#else
    _delay_ms(ROUTINE_FREQUENCY_MINUTES * 60e3);
#endif
  }
}

void get_entry(uint8_t index, server_entry_t *p_entry) {
  storage_get_block(index, p_entry->data.as_array, &p_entry->timestamp);
}

void assert_ok(char *title, uint8_t call_return, uint8_t excepted_return) {
  if (call_return != excepted_return) {
    lcd_locate_str(0, 0, "[FAILURE]");
    lcd_locate_str(1, 0, title);
    while (1) {
      /* Polling */
    }
  }
}

void init(void) {
  lcd_init();
  lcd_text("init start..", ' ');
  _delay_ms(2000); // for server_init
  assert_ok("init:server_init", server_init(), SERVER_OK);
  assert_ok("init:weather_init", weather_init(), Weather_OK);
  assert_ok("init:storage_init", storage_init(), STORAGE_OK);
  assert_ok("init:server_run", server_run(get_entry), SERVER_OK);
  lcd_text("init end..", ' ');
}

void routine(void) {
  server_entry_data_t entry_data;
  lcd_text("routine start..", ' ');
  assert_ok("routine:weather_measures",
            weather_measures(&entry_data.as_struct.temperature,
                             &entry_data.as_struct.humidity,
                             &entry_data.as_struct.light),
            Weather_OK);
  assert_ok("routine:storage_enqueue_block",
            storage_enqueue_block(entry_data.as_array), STORAGE_OK);
  lcd_text("routine end..", ' ');
}
