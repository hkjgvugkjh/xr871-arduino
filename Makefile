#
# XR871 Arduino Core Makefile
# Integrates with XR871 SDK build system
#

# ----------------------------------------------------------------------------
# Project configuration
# ----------------------------------------------------------------------------
PRJ_NAME := xr871_arduino_core

# ----------------------------------------------------------------------------
# SDK path
# ----------------------------------------------------------------------------
XR871SDK ?= ./XR871SDK

# ----------------------------------------------------------------------------
# Toolchain (use system arm-none-eabi-gcc)
# ----------------------------------------------------------------------------
CC := arm-none-eabi-gcc
CXX := arm-none-eabi-g++
AR := arm-none-eabi-ar

# ----------------------------------------------------------------------------
# Compiler flags
# ----------------------------------------------------------------------------
CPU := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp

CFLAGS := $(CPU) -c -Os -gdwarf-2
CFLAGS += -Wall -Wextra
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -D__CONFIG_CHIP_XR871 -D__CONFIG_CPU_CM4F -D__CONFIG_OS_FREERTOS
CFLAGS += -D__CONFIG_LIBC_PRINTF_FLOAT -D__CONFIG_LIBC_WRAP_STDIO
CFLAGS += -DARDUINO=10819 -DARDUINO_XR871 -DARDUINO_ARCH_XR871

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++98

# ----------------------------------------------------------------------------
# Include paths
# ----------------------------------------------------------------------------
INCLUDES := \
	-I. \
	-I$(XR871SDK)/include \
	-I$(XR871SDK)/include/driver \
	-I$(XR871SDK)/include/driver/chip \
	-I$(XR871SDK)/include/driver/cmsis \
	-I$(XR871SDK)/include/kernel \
	-I$(XR871SDK)/include/kernel/os \
	-I$(XR871SDK)/include/kernel/FreeRTOS \
	-I$(XR871SDK)/include/kernel/FreeRTOS/portable/GCC/ARM_CM4F \
	-I$(XR871SDK)/include/libc \
	-I$(XR871SDK)/include/net \
	-I$(XR871SDK)/include/net/wlan \
	-I$(XR871SDK)/include/net/lwip-1.4.1 \
	-I$(XR871SDK)/include/net/lwip-1.4.1/ipv4 \
	-I$(XR871SDK)/include/net/lwip-1.4.1/lwip

CFLAGS += $(INCLUDES)
CXXFLAGS += $(INCLUDES)

# ----------------------------------------------------------------------------
# Source files
# ----------------------------------------------------------------------------
C_SRCS := Arduino.c
CPP_SRCS := String.cpp

# ----------------------------------------------------------------------------
# Build rules
# ----------------------------------------------------------------------------
BUILD_DIR := build
C_OBJS := $(addprefix $(BUILD_DIR)/,$(notdir $(C_SRCS:.c=.o)))
CPP_OBJS := $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SRCS:.cpp=.o)))
ALL_OBJS := $(C_OBJS) $(CPP_OBJS)

# Library output
LIB := $(BUILD_DIR)/lib$(PRJ_NAME).a

.PHONY: all clean lib info

all: lib

lib: $(LIB)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile C files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "CC  $<"
	@$(CC) $(CFLAGS) $< -o $@

# Compile C++ files
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@echo "CXX $<"
	@$(CXX) $(CXXFLAGS) $< -o $@

# Create static library
$(LIB): $(ALL_OBJS)
	@echo "AR  $@"
	@$(AR) rcs $@ $^

clean:
	rm -rf $(BUILD_DIR)

# Print info
info:
	@echo "Project: $(PRJ_NAME)"
	@echo "SDK Path: $(XR871SDK)"
	@echo "C sources: $(C_SRCS)"
	@echo "C++ sources: $(CPP_SRCS)"
	@echo "CC: $(CC)"
	@echo "CXX: $(CXX)"
