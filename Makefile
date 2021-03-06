# File:   Makefile
# Author: M. P. Hayes, UCECE
# Date:   17 October 2019
# Descr:  Makefile for game by Gordon Lay and Henry Mossman

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../../utils -I../../fonts -I../../drivers -I../../drivers/avr -I../../extra -I../../assignment/final
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
game.o: game.c ../../drivers/avr/eeprom.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../drivers/display.h ../../drivers/led.h ../../drivers/navswitch.h ../../extra/mmelody.h ../../extra/ticker.h ../../extra/tweeter.h ../../fonts/font3x5_1.h ../../utils/font.h ../../utils/task.h ../../utils/tinygl.h ../../utils/uint8toa.h ../../utils/pacer.h ../../drivers/button.h ../../drivers/ir_serial.h ../../drivers/ir.h ../../assignment/final/throwing.h ../../assignment/final/throwBall.h ../../assignment/final/catchBall.h
	$(CC) -c $(CFLAGS) $< -o $@

eeprom.o: ../../drivers/avr/eeprom.c ../../drivers/avr/eeprom.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ../../drivers/display.c ../../drivers/avr/system.h ../../drivers/display.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

led.o: ../../drivers/led.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/led.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../../drivers/ledmat.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ../../drivers/navswitch.c ../../drivers/avr/delay.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

mmelody.o: ../../extra/mmelody.c ../../drivers/avr/system.h ../../extra/mmelody.h
	$(CC) -c $(CFLAGS) $< -o $@

ticker.o: ../../extra/ticker.c
	$(CC) -c $(CFLAGS) $< -o $@

tweeter.o: ../../extra/tweeter.c ../../drivers/avr/system.h ../../extra/ticker.h ../../extra/tweeter.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../../utils/font.c ../../drivers/avr/system.h ../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

task.o: ../../utils/task.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../../utils/tinygl.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

uint8toa.o: ../../utils/uint8toa.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ../../utils/pacer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

button.o: ../../drivers/button.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/button.h
	$(CC) -c $(CFLAGS) $< -o $@

ir.o: ../../drivers/ir.c ../../drivers/avr/delay.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/ir.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_serial.o: ../../drivers/ir_serial.c ../../drivers/avr/delay.h ../../drivers/avr/system.h ../../drivers/ir.h ../../drivers/ir_serial.h
	$(CC) -c $(CFLAGS) $< -o $@

throwing.o: ../../assignment/final/throwing.c ../../utils/tinygl.h ../../drivers/navswitch.h ../../utils/uint8toa.h ../../utils/pacer.h ../../drivers/avr/system.h ../../assignment/final/throwing.h
	$(CC) -c $(CFLAGS) $< -o $@

throwBall.o: ../../assignment/final/throwBall.c ../../drivers/navswitch.h ../../drivers/avr/pio.h ../../utils/uint8toa.h ../../utils/pacer.h ../../drivers/avr/system.h ../../assignment/final/throwBall.h
	$(CC) -c $(CFLAGS) $< -o $@

catchBall.o: ../../assignment/final/catchBall.c ../../drivers/navswitch.h ../../drivers/avr/pio.h ../../utils/uint8toa.h ../../utils/pacer.h ../../drivers/avr/system.h ../../assignment/final/catchBall.h
	$(CC) -c $(CFLAGS) $< -o $@

# Link: create ELF output file from object files.
# Link: create output file (executable) from object files.
game.out: game.o eeprom.o pio.o system.o timer.o display.o led.o ledmat.o navswitch.o mmelody.o ticker.o tweeter.o font.o task.o tinygl.o uint8toa.o pacer.o button.o ir.o ir_serial.o throwing.o throwBall.o catchBall.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean:
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: game.out
	$(OBJCOPY) -O ihex game.out game.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start


