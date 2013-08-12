
TARGET=threads
MCU=atmega328

# list source files here
C_SRC=main.c thread.c serial.c clock.c led.c
ASM_SRC=switch.S prepare_stack.S

OBJ=$(C_SRC:.c=.o) $(ASM_SRC:.S=.o)

OBJCOPY=avr-objcopy
FORMAT=ihex

CC=avr-gcc
AS=avr-as
C_INCLUDE_PATH=/usr/avr/include
ASM_INCLUDE_PATH=/usr/avr/include
CFLAGS=-mmcu=$(MCU) -I$(C_INCLUDE_PATH) -std=c99
ASMFLAGS=-mmcu=$(MCU) -I$(ASM_INCLUDE_PATH)

# Programming the board
AVRDUDE=avrdude
PORT=/dev/ttyUSB0
AVRDUDE_PROGRAMMER=arduino
AVRDUDE_PART=atmega328p
BAUDRATE=57600
AVRDUDE_FLAGS=-F -p $(AVRDUDE_PART) -P $(PORT) -c $(AVRDUDE_PROGRAMMER) -b $(BAUDRATE)
AVRDUDE_WRITE_FLASH=-U flash:w:$(TARGET).hex

# generate a .hex file suitable for uploading to the MCU
all: $(TARGET).hex

# compile a .c to a .o
%.o : %.S
	$(CC) $(CFLAGS) -c -o $@ $<

# compile a .c to a .o
%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# link the .o files
$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

# convert to a .hex
$(TARGET).hex : $(TARGET).elf
	$(OBJCOPY) -O $(FORMAT) $< $@

# upload the program to the board
program: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

clean: 
	rm -rf *.o *.elf *.hex
