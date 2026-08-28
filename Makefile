#
# Makefile for XR871 Arduino WiFi Example
# This integrates with the XR871 SDK build system
#

# ----------------------------------------------------------------------------
# Project configuration
# ----------------------------------------------------------------------------
PRJ_NAME := xr871_arduino_wifi

# Source files
SRCS := \
	Arduino.c \
	Wire.cpp \
	SPI.cpp \
	PWM.cpp \
	WiFi.cpp \
	WiFiClient.cpp \
	WiFiUDP.cpp \
	WiFi_Station.ino

# Include paths
INCLUDES := \
	-I. \
	-I$(XR871SDK)/include \
	-I$(XR871SDK)/include/driver \
	-I$(XR871SDK)/include/driver/chip \
	-I$(XR871SDK)/include/kernel \
	-I$(XR871SDK)/include/kernel/os \
	-I$(XR871SDK)/include/libc \
	-I$(XR871SDK)/include/net \
	-I$(XR871SDK)/include/net/wlan \
	-I$(XR871SDK)/include/net/lwip-1.4.1/src/include \
	-I$(XR871SDK)/include/net/lwip-1.4.1/src/include/ipv4 \
	-I$(XR871SDK)/include/net/lwip-1.4.1/src/include/lwip

# ----------------------------------------------------------------------------
# SDK path (modify this to point to your XR871 SDK installation)
# ----------------------------------------------------------------------------
XR871SDK ?= ../03_SDK/xr871sdk

# ----------------------------------------------------------------------------
# Toolchain
# ----------------------------------------------------------------------------
CC_DIR := $(HOME)/tools/gcc-arm-none-eabi-4_9-2015q2/bin
CC_PREFIX := $(CC_DIR)/arm-none-eabi-

CC := $(CC_PREFIX)gcc
CXX := $(CC_PREFIX)g++
AS := $(CC_PREFIX)as
LD := $(CC_PREFIX)ld
AR := $(CC_PREFIX)ar
OBJCOPY := $(CC_PREFIX)objcopy
OBJDUMP := $(CC_PREFIX)objdump
SIZE := $(CC_PREFIX)size

# ----------------------------------------------------------------------------
# Compiler flags
# ----------------------------------------------------------------------------
CPU := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp

CFLAGS := $(CPU) -c -Os -gdwarf-2
CFLAGS += -Wall -Wextra -Werror=implicit-function-declaration
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -D__CONFIG_CHIP_XR871 -D__CONFIG_CPU_CM4F -D__CONFIG_OS_FREERTOS
CFLAGS += -D__CONFIG_LIBC_PRINTF_FLOAT -D__CONFIG_LIBC_WRAP_STDIO
CFLAGS += -D__CONFIG_ARCH_DUAL_CORE -D__CONFIG_ARCH_APP_CORE
CFLAGS += $(INCLUDES)

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions

# ----------------------------------------------------------------------------
# Linker flags
# ----------------------------------------------------------------------------
LDFLAGS := $(CPU) -Wl,--gc-sections
LDFLAGS += -T$(XR871SDK)/project/linker_script/gcc_xr871.ld
LDFLAGS += -L$(XR871SDK)/lib
LDFLAGS += -Wl,--whole-archive -lchip -Wl,--no-whole-archive
LDFLAGS += -los -lconsole -lutil -lc -llwip -lwlan
LDFLAGS += -lm -lgcc -lc -lstdc++

# ----------------------------------------------------------------------------
# Build rules
# ----------------------------------------------------------------------------
BUILD_DIR := build
OBJS := $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=.o)))
OBJS := $(OBJS:.cpp=.o)
OBJS := $(OBJS:.ino=.o)

.PHONY: all clean

all: $(BUILD_DIR)/$(PRJ_NAME).bin

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.ino | $(BUILD_DIR)
	$(CC) $(CFLAGS) -x c $< -o $@

$(BUILD_DIR)/$(PRJ_NAME).elf: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@
	$(SIZE) $@

$(BUILD_DIR)/$(PRJ_NAME).bin: $(BUILD_DIR)/$(PRJ_NAME).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/$(PRJ_NAME).lst: $(BUILD_DIR)/$(PRJ_NAME).elf
	$(OBJDUMP) -d $< > $@

clean:
	rm -rf $(BUILD_DIR)

# Print info
info:
	@echo "Project: $(PRJ_NAME)"
	@echo "Sources: $(SRCS)"
	@echo "SDK Path: $(XR871SDK)"
	@echo "CC: $(CC)"
	@echo "CXX: $(CXX)"
