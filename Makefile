
# microcontroller and project specific settings
TARGET = unzap
F_CPU = 16000000UL
MCU = atmega48
HARDWARE_REV=2

SRC = $(shell echo *.c)
OBJECTS += $(patsubst %.c,%.o,$(SRC))
HEADERS += $(shell echo *.h)
# CFLAGS += -Werror
LDFLAGS += -L/usr/local/avr/avr/lib
CFLAGS += -DHARDWARE_REV=$(HARDWARE_REV)

# no safe mode checks
AVRDUDE_FLAGS += -u

# set name for dependency-file
MAKEFILE = Makefile

include avr.mk

.PHONY: all

all: $(TARGET).hex $(TARGET).eep.hex $(TARGET).lss
	@echo "==============================="
	@echo "$(TARGET) compiled for: $(MCU)"
	@echo -n "size is: "
	@$(SIZE) -A $(TARGET).hex | grep "\.sec1" | tr -s " " | cut -d" " -f2
	@echo "==============================="

$(TARGET): $(OBJECTS) $(TARGET).o

%.o: $(HEADERS)

.PHONY: install

# install: program-serial-$(TARGET) program-serial-eeprom-$(TARGET)
install: program-isp-$(TARGET)

.PHONY: clean clean-$(TARGET) clean-botloader

clean: clean-$(TARGET)

clean-$(TARGET):
	$(RM) $(TARGET) $(OBJECTS)

clean-bootloader:
	$(MAKE) -C bootloader clean

.PHONY: depend

depend:
	$(CC) $(CFLAGS) -M $(CDEFS) $(CINCS) $(SRC) $(ASRC) >> $(MAKEFILE).dep

fuses-atmega48:
	$(AVRDUDE) $(AVRDUDE_FLAGS) -c $(ISP_PROG) -P $(ISP_DEV) -e -U hfuse:w:0xD5:m -U lfuse:w:0xE7:m -U efuse:w:0x01:m

bootstrap: fuses-atmega48 install

-include $(MAKEFILE).dep
