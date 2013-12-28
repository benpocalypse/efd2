###############################################################################
# Makefile for the project SpriteDemo
###############################################################################

## General Flags
PROJECT = efd2
GAME = efd2
MCU = atmega644
TARGET = $(GAME).elf
CC = avr-gcc
INFO=../gameinfo.properties

## Kernel settings
KERNEL_DIR = ../../../../kernel
KERNEL_OPTIONS  = -DVIDEO_MODE=3 -DINTRO_LOGO=0 -DSCROLLING=0 -DSOUND_CHANNEL_4_ENABLE=0 
KERNEL_OPTIONS += -DMAX_SPRITES=14 -DRAM_TILES_COUNT=12 -DSCREEN_TILES_V=28 ##-DFIRST_RENDER_LINE=24 
KERNEL_OPTIONS += -DVRAM_TILES_V=30 ##-DOVERLAY_LINES=4 

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=28636360UL -Os -fsigned-char -ffunction-sections -fno-inline
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 
CFLAGS += $(KERNEL_OPTIONS)


## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += -Wl,-Map=$(GAME).map 
LDFLAGS += -Wl,-gc-sections 


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = uzeboxVideoEngineCore.o  uzeboxCore.o uzeboxSoundEngine.o uzeboxSoundEngineCore.o uzeboxVideoEngine.o 
OBJECTS += globals.o gamemanager.o logicmanager.o player.o enemy.o map.o input.o $(GAME).o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Include Directories
INCLUDES = -I"$(KERNEL_DIR)" 

## Build
all: $(TARGET) $(GAME).hex $(GAME).eep $(GAME).lss $(GAME).uze size

## Compile Kernel files
uzeboxVideoEngineCore.o: $(KERNEL_DIR)/uzeboxVideoEngineCore.s
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

uzeboxSoundEngineCore.o: $(KERNEL_DIR)/uzeboxSoundEngineCore.s
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

uzeboxCore.o: $(KERNEL_DIR)/uzeboxCore.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uzeboxSoundEngine.o: $(KERNEL_DIR)/uzeboxSoundEngine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uzeboxVideoEngine.o: $(KERNEL_DIR)/uzeboxVideoEngine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

gamemanager.o: ../gamemanager.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

globals.o: ../globals.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<
	
logicmanager.o: ../logicmanager.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

player.o: ../player.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

enemy.o: ../enemy.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<
	
map.o: ../map.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<
	
input.o: ../input.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<


## Compile game sources
$(GAME).o: ../efd2.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

%.uze: $(TARGET)
	-$(UZEBIN_DIR)packrom $(GAME).hex $@ $(INFO)

UNAME := $(shell sh -c 'uname -s 2>/dev/null || echo not')
AVRSIZEFLAGS := -A ${TARGET}
ifneq (,$(findstring MINGW,$(UNAME)))
AVRSIZEFLAGS := -C --mcu=${MCU} ${TARGET}
endif

size: ${TARGET}
	@echo
	@avr-size ${AVRSIZEFLAGS}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(GAME).* dep/* *.uze


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

