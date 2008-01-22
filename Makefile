
# microcontroller and project specific settings
TARGET = unzap
F_CPU = 16000000UL
MCU = atmega48
HARDWARE_REV=1

OBJECTS += $(patsubst %.c,%.o,$(shell echo *.c))
HEADERS += $(shell echo *.h)
# CFLAGS += -Werror
LDFLAGS += -L/usr/local/avr/avr/lib
CFLAGS += -DHARDWARE_REV=$(HARDWARE_REV)

# no safe mode checks
AVRDUDE_FLAGS += -u

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

.PHONY: bootstrap fuse install-bootloader

bootstrap: fuse install-bootloader install

fuse-atmega8:
	$(AVRDUDE) $(AVRDUDE_FLAGS) -c avr910 -P $(ISP_DEV) -U hfuse:w:0xD0:m
	$(AVRDUDE) $(AVRDUDE_FLAGS) -c avr910 -P $(ISP_DEV) -U lfuse:w:0xE0:m

fuse-atmega168:
	$(AVRDUDE) $(AVRDUDE_FLAGS) -c avr910 -P $(ISP_DEV) -U hfuse:w:0xD7:m
	$(AVRDUDE) $(AVRDUDE_FLAGS) -c avr910 -P $(ISP_DEV) -U lfuse:w:0xE0:m
	$(AVRDUDE) $(AVRDUDE_FLAGS) -c avr910 -P $(ISP_DEV) -U efuse:w:0x01:m

install-bootloader: bootloader.hex program-isp-bootloader
