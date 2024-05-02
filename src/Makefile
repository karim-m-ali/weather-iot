MCU=atmega32a
DF_CPU=16000000u

C=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-Os -DF_CPU=$(DF_CPU) -mmcu=$(MCU) 

TARGET_C_FILE=main.c
TARGET_HEX_FILE=/dev/null

HEADERS=$(wildcard mcal/*.h) $(wildcard hal/*.h) $(wildcard app/*.h)
SOURCES=$(wildcard mcal/*.c) $(wildcard hal/*.c) $(wildcard app/*.c)

all: firmware.hex

main.elf: $(SOURCES) $(TARGET_C_FILE) $(HEADERS)
	$(C) $(CFLAGS) -c -o main.o $(TARGET_C_FILE)
	$(C) $(CFLAGS) -mmcu=$(MCU) main.o $(SOURCES) -o $@

firmware.hex: main.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	cp $@ $(TARGET_HEX_FILE)

flash_usbasp: firmware.hex
	doas avrdude -P usb -c usbasp -p $(MCU) -U flash:w:$<:i

flash_arduino: firmware.hex
	doas avrdude -P /dev/ttyUSB0 -c arduino -p $(MCU) -U flash:w:$<:i

flash: flash_arduino

clean:
	/bin/rm -f *.o *.elf *.hex

.PHONY: all flash flash_usbasp flash_arduino clean
