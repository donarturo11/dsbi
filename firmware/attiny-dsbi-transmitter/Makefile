# vim: sts=4 sw=4 et:
PROJECT_NAME = attiny-dsbi-transmitter
CONFIG ?= attiny85
DEFINITIONS = 

include config/$(CONFIG)/Makefile.mk

# UART_MODE: 
# - SWUART (Very simple software UART written in pure C)
# - AVR305 (assembly software UART described in AVR305 datasheet with small modifications)
UART_MODE ?= SWUART
BAUD_RATE ?= 19200

UARTPORT ?= B
TX ?= 1
RX ?= 0

# UART definitions
UART_PORT = PORT${UARTPORT}
UART_PIN = PIN${UARTPORT}
UART_DDR = DDR${UARTPORT}
DEFINITIONS += -D$(UART_MODE)
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

TMPDIR ?= .tmp-$(MCU)
OUTPUT = out

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU)
CFLAGS += -Wall -Os
CFLAGS += -save-temps=obj
#CFLAGS += -flto
#CFLAGS += -mcall-prologues
#CFLAGS += -mtiny-stack
CFLAGS += -I.
CFLAGS += -I./config/$(CONFIG)/
CFLAGS += $(DEFINITIONS)
SECTIONS = -j .text 
SECTIONS += -j .data
SECTIONS += -j .bss 
SECTIONS += -j .bootloader
OBJECTS = main.o 
OBJECTS += buffer.o 
OBJECTS += adc.o 


ifeq ($(UART_MODE), SWUART)
OBJECTS += swuart.o
else ifeq ($(UART_MODE), AVR305)
OBJECTS += avr305.o 
endif

OBJ_TMP := $(patsubst %,$(TMPDIR)/%,$(OBJECTS))

.PHONY: all output tmpdir clean

all: $(OUTPUT)/$(FIRMWARENAME).hex $(OUTPUT)/$(FIRMWARENAME).bin $(OUTPUT)/$(FIRMWARENAME).asm

$(OUTPUT)/%.hex: $(OUTPUT)/%.elf | output
	$(OBJCOPY) -O ihex $< $@

$(OUTPUT)/%.bin: $(OUTPUT)/%.elf | output
	$(OBJCOPY) $(SECTIONS) -O binary $< $@

$(OUTPUT)/$(FIRMWARENAME).asm: | output
	avr-objdump --no-show-raw-insn $(SECTIONS) -m$(AVRARCH) -D -Sz $(OUTPUT)/$(FIRMWARENAME).elf > $@

$(OUTPUT)/$(FIRMWARENAME).elf: $(OBJ_TMP) | output
	$(CC) $(CFLAGS) -o $@ $^
	avr-size --format=avr --mcu=$(MCU) $(OUTPUT)/$(FIRMWARENAME).elf

$(TMPDIR)/%.S: src/%.c | tmpdir
	$(CC) $(CFLAGS) -S -o $@ $<

$(TMPDIR)/%.o: src/%.c | tmpdir
	$(CC) $(CFLAGS) -c -o $@ $<

$(TMPDIR)/%.o: src/%.S | tmpdir
	$(CC) $(CFLAGS) -x assembler-with-cpp -c -o $@ $<

fuse:
	$(DUDE) $(DUDEFLAGS) -e $(FUSES)

flash:
	$(DUDE) $(DUDEFLAGS) -Uflash:w:$(OUTPUT)/$(FIRMWARENAME).hex:i

simulavr:
	simulavr -d $(MCU) -o $(TMPDIR)/tracelist-$(MCU).txt
	@echo -e "\nPress Ctrl+C to abort\n"
	simulavr -m 22000000 -d $(MCU) -F $(F_CPU) -c vcd:$(TMPDIR)/tracelist-$(MCU).txt:$(TMPDIR)/trace-$(MCU).vcd -f $(OUTPUT)/$(FIRMWARENAME).elf

tmpdir:
	@mkdir -p $(TMPDIR)

output:
	@mkdir -p $(OUTPUT)

clean:
	$(RM) -Rvf $(TMPDIR)

