# vim: sts=4 sw=4 et:
PROJECT_NAME = attiny-dsbi-transmitter
CONFIG ?= attiny85
DEFINITIONS = 

include config/$(CONFIG)/Makefile.mk

BAUD_RATE ?= 19200
UARTPORT ?= B
TX ?= 1
RX ?= 0

# UART definitions
UART_PORT = PORT${UARTPORT}
UART_PIN = PIN${UARTPORT}
UART_DDR = DDR${UARTPORT}
DEFINITIONS += -DBAUD_RATE=$(BAUD_RATE)

# software UART
DEFINITIONS += -DUART_PORT=$(UART_PORT)
DEFINITIONS += -DUART_DDR=$(UART_DDR)
DEFINITIONS += -DUART_PIN=$(UART_PIN)
DEFINITIONS += -DTXPIN=$(TX)
DEFINITIONS += -DRXPIN=$(RX)
##

FIRMWARENAME = $(PROJECT_NAME)-$(CONFIG)
CC = avr-gcc
OBJCOPY = avr-objcopy
DUDE = avrdude
PROGRAMMER ?= siprog
DUDEFLAGS = -v -p $(MCU) -c $(PROGRAMMER)

CFLAGS = -Wall -Os -I. -mmcu=$(MCU) -DF_CPU=$(F_CPU)
CFLAGS += -I./config/$(CONFIG)/
CFLAGS += $(DEFINITIONS)
OBJFLAGS = -j .text -j .data
OBJECTS = main.o 

all: $(FIRMWARENAME).hex $(FIRMWARENAME).bin $(FIRMWARENAME).asm

%.hex: %.elf
	$(OBJCOPY) $(OBJFLAGS) -O ihex $< $@

%.bin: %.elf
	$(OBJCOPY) $(OBJFLAGS) -O binary $< $@

$(FIRMWARENAME).asm: $(FIRMWARENAME).hex
	avr-objdump --no-show-raw-insn $(OBJFLAGS) -m$(AVRARCH) -D -S $(FIRMWARENAME).elf > $@

$(FIRMWARENAME).elf: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	avr-size --format=avr --mcu=$(MCU) $(FIRMWARENAME).elf

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -c -o $@ $<

fuse:
	$(DUDE) $(DUDEFLAGS) -e $(FUSES)

flash:
	$(DUDE) $(DUDEFLAGS) -Uflash:w:$(FIRMWARENAME).hex:i

simulavr:
	simulavr -d $(MCU) -o tracelist-$(MCU).txt
	@echo -e "\nPress Ctrl+C to abort\n"
	simulavr -m $(F_CPU)*1000000 -d $(MCU) -F $(F_CPU) -c vcd:tracelist-$(MCU).txt:trace-$(MCU).vcd -f $(FIRMWARENAME).elf

clean:
	$(RM) *.o *.elf *.hex *.asm *.bin tracelist*.txt *.vcd
