####################################################
# 'make' configuration file for avr-gcc
# by fd0@koeln.ccc.de
####################################################

# controller
MCU = atmega168

# frequency
F_CPU = 16000000UL

# main application name (without .hex)
# eg 'test' when the main function is defined in 'test.c'
TARGET = unzap

# c sourcecode files
# eg. 'test.c foo.c foobar/baz.c'
SRC = $(shell echo *.c) usbdrv/usbdrv.c

# asm sourcecode files
# eg. 'interrupts.S foobar/another.S'
ASRC = usbdrv/usbdrvasm.S

# headers which should be considered when recompiling
# eg. 'global.h foobar/important.h'
HEADERS =

# include directories (used for both, c and asm)
# eg '. usbdrv/'
INCLUDES = . usbdrv


# use more debug-flags when compiling
DEBUG = 1


# hardware version is 2
HARDWARE_REV = 2


CFLAGS += -DHARDWARE_REV=2
ASFLAGS += -DHARDWARE_REV=2


# avrdude programmer protocol
PROG = usbasp
# avrdude programmer device
DEV = usb
# further flags for avrdude
AVRDUDE_FLAGS =

####################################################
# 'make' configuration
####################################################
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AS = avr-as
SIZE = avr-size
CP = cp
RM = rm -f
AVRDUDE = avrdude

# flags for the compiler (for .c files)
CFLAGS += -g -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -std=gnu99 -fshort-enums
CFLAGS += $(addprefix -I,$(INCLUDES))
# flags for the compiler (for .S files)
ASFLAGS += -g -mmcu=$(MCU) -DF_CPU=$(F_CPU) -x assembler-with-cpp
ASFLAGS += $(addprefix -I,$(INCLUDES))
# flags for the linker
LDFLAGS += -mmcu=$(MCU)

# fill in object files
OBJECTS += $(patsubst %.c,%.o,$(SRC))
OBJECTS += $(patsubst %.S,%.o,$(ASRC))

# include config file
-include $(CURDIR)/config.mk

# include more debug flags, if $(DEBUG) is 1
ifeq ($(DEBUG),1)
	CFLAGS += -Wall -W -Wchar-subscripts -Wmissing-prototypes
	CFLAGS += -Wmissing-declarations -Wredundant-decls
	CFLAGS += -Wstrict-prototypes -Wshadow -Wbad-function-cast
	CFLAGS += -Winline -Wpointer-arith -Wsign-compare
	CFLAGS += -Wunreachable-code -Wdisabled-optimization
	CFLAGS += -Wcast-align -Wwrite-strings -Wnested-externs -Wundef
	CFLAGS += -Wa,-adhlns=$(basename $@).lst
	CFLAGS += -DDEBUG
endif

####################################################
# avrdude configuration
####################################################
ifeq ($(MCU),atmega8)
	AVRDUDE_MCU=m8
endif
ifeq ($(MCU),atmega48)
	AVRDUDE_MCU=m48
endif
ifeq ($(MCU),atmega88)
	AVRDUDE_MCU=m88
endif
ifeq ($(MCU),atmega168)
	AVRDUDE_MCU=m168
endif

AVRDUDE_FLAGS += -p $(AVRDUDE_MCU)

# configuration for automatic dependency detection
MAKEFILE_NAME = $(realpath $(firstword $(MAKEFILE_LIST)))

####################################################
# make targets
####################################################

.PHONY: all clean distclean avrdude-terminal depend

# main rule
all: $(TARGET).hex

$(TARGET).elf: $(OBJECTS) $(TARGET).o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# all objects (.o files)
$(OBJECTS): $(HEADERS)

# remove all compiled files
clean:
	$(RM) $(foreach ext,elf hex eep.hex map,$(TARGET).$(ext)) \
		$(foreach file,$(patsubst %.o,%,$(OBJECTS)),$(foreach ext,o lst lss,$(file).$(ext)))

# additionally remove the dependency makefile
distclean: clean
	$(RM) $(MAKEFILE_NAME).dep

# avrdude-related targets
install program: program-$(TARGET)

avrdude-terminal:
	$(AVRDUDE) $(AVRDUDE_FLAGS) -c $(PROG) -P $(DEV) -t

program-%: %.hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -c $(PROG) -P $(DEV) -U flash:w:$<

program-eeprom-%: %.eep.hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -c $(PROG) -P $(DEV) -U eeprom:w:$<

# special programming targets
%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	@echo "========================================"
	@echo "$@ compiled for: $(MCU)"
	@echo -n "size for $< is "
	@$(SIZE) -A $@ | grep '\.sec1' | tr -s ' ' | cut -d" " -f2
	@echo "========================================"

%.eep.hex: %.elf
	$(OBJCOPY) --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex -j .eeprom $< $@

%.lss: %.elf
	$(OBJDUMP) -h -S $< > $@

# automatic dependency detection
depend:
	$(CC) $(CFLAGS) -MM $(CDEFS) $(CINCS) $(SRC) $(ASRC) > $(MAKEFILE_LIST).dep

-include $(MAKEFILE).dep
