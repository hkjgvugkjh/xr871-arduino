#
# XR871 Arduino Core Makefile v1.4.0
# Builds libxr871.a from SDK sources and Arduino core libraries
#

# Configuration
XR871SDK ?= $(CURDIR)/SDK
BUILD_DIR ?= $(CURDIR)/build
LIB_DIR ?= $(CURDIR)/lib

# Toolchain (from Board Manager)
TC_PATH ?= $(CURDIR)/tools/arm-none-eabi-gcc/bin
CC := $(TC_PATH)/arm-none-eabi-gcc
CXX := $(TC_PATH)/arm-none-eabi-g++
AR := $(TC_PATH)/arm-none-eabi-ar

# CPU/FPU options
CPU := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp

# Compiler flags
CFLAGS := $(CPU) -Os -w -std=gnu99 -ffunction-sections -fdata-sections
CFLAGS += -D__CONFIG_CHIP_XR871 -D__CONFIG_CPU_CM4F -D__CONFIG_OS_FREERTOS
CFLAGS += -D__CONFIG_LIBC_REDEFINE_GCC_INT32_TYPE
CFLAGS += -D__CONFIG_LIBC_PRINTF_FLOAT -D__CONFIG_LIBC_WRAP_STDIO
CFLAGS += -DARDUINO=10819 -DARDUINO_XR871 -DARDUINO_ARCH_XR871

# Include paths
CFLAGS += -I$(XR871SDK)/include
CFLAGS += -I$(XR871SDK)/include/driver
CFLAGS += -I$(XR871SDK)/include/driver/chip
CFLAGS += -I$(XR871SDK)/include/driver/cmsis
CFLAGS += -I$(XR871SDK)/include/kernel
CFLAGS += -I$(XR871SDK)/include/kernel/os
CFLAGS += -I$(XR871SDK)/include/kernel/os/FreeRTOS
CFLAGS += -I$(XR871SDK)/include/kernel/FreeRTOS
CFLAGS += -I$(XR871SDK)/include/kernel/FreeRTOS/portable/GCC/ARM_CM4F
CFLAGS += -I$(XR871SDK)/include/libc
CFLAGS += -I$(XR871SDK)/project/common/board
CFLAGS += -I$(XR871SDK)/project/common/board/xr871_evb_main
CFLAGS += -I$(CURDIR)

# C++ flags
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++98

# SDK source files to compile
SDK_SRCS := \
	$(XR871SDK)/src/driver/chip/system_chip.c \
	$(XR871SDK)/src/driver/chip/hal_gpio.c \
	$(XR871SDK)/src/driver/chip/hal_uart.c \
	$(XR871SDK)/src/driver/chip/hal_prcm.c \
	$(XR871SDK)/src/driver/chip/hal_timer.c \
	$(XR871SDK)/src/driver/chip/hal_nvic.c \
	$(XR871SDK)/src/driver/chip/hal_clock.c \
	$(XR871SDK)/src/driver/chip/hal_flash.c \
	$(XR871SDK)/src/driver/chip/hal_flashctrl.c \
	$(XR871SDK)/src/driver/chip/hal_flashcache.c \
	$(XR871SDK)/src/driver/chip/hal_global.c \
	$(XR871SDK)/src/driver/chip/hal_wdg.c \
	$(XR871SDK)/src/driver/chip/hal_pwm.c \
	$(XR871SDK)/src/driver/chip/hal_spi.c \
	$(XR871SDK)/src/driver/chip/hal_i2c.c \
	$(XR871SDK)/src/driver/chip/hal_i2s.c \
	$(XR871SDK)/src/driver/chip/hal_dmic.c \
	$(XR871SDK)/src/driver/chip/hal_adc.c \
	$(XR871SDK)/src/driver/chip/hal_dma.c \
	$(XR871SDK)/src/driver/chip/hal_crypto.c \
	$(XR871SDK)/src/driver/chip/hal_efuse.c \
	$(XR871SDK)/src/driver/chip/hal_csi.c \
	$(XR871SDK)/src/driver/chip/hal_irrx.c \
	$(XR871SDK)/src/driver/chip/hal_irtx.c \
	$(XR871SDK)/src/driver/chip/hal_mbox.c \
	$(XR871SDK)/src/driver/chip/hal_spinlock.c \
	$(XR871SDK)/src/driver/chip/hal_swd.c \
	$(XR871SDK)/src/driver/chip/hal_util.c \
	$(XR871SDK)/src/driver/chip/hal_wakeup.c \
	$(XR871SDK)/src/driver/chip/hal_xip.c \
	$(XR871SDK)/src/driver/chip/flashchip/flash_chip.c \
	$(XR871SDK)/src/driver/chip/flashchip/flash_default.c \
	$(XR871SDK)/src/libc/wrap_stdio.c \
	$(XR871SDK)/src/libc/wrap_malloc.c \
	$(XR871SDK)/src/libc/wrap_memmove.c \
	$(XR871SDK)/src/libc/wrap_memset.c

# Filter existing sources
SDK_SRCS := $(wildcard $(SDK_SRCS))

# Arduino Core source files
ARDUINO_CORE_SRCS := \
	$(CURDIR)/Arduino.c \
	$(CURDIR)/HardwareSerial.cpp \
	$(CURDIR)/Wire.cpp \
	$(CURDIR)/SPI.cpp \
	$(CURDIR)/ADC.cpp \
	$(CURDIR)/WiFi.cpp \
	$(CURDIR)/Audio.cpp \
	$(CURDIR)/OTA.cpp \
	$(CURDIR)/String.cpp

# Object files
SDK_OBJS := $(patsubst $(XR871SDK)/src/%.c,$(BUILD_DIR)/sdk/%.o,$(SDK_SRCS))
ARDUINO_CORE_OBJS := $(patsubst $(CURDIR)/%.c,$(BUILD_DIR)/core/%.o,$(ARDUINO_CORE_SRCS))
ARDUINO_CORE_OBJS := $(patsubst $(CURDIR)/%.cpp,$(BUILD_DIR)/core/%.o,$(ARDUINO_CORE_OBJS))

# Output library
LIB := $(LIB_DIR)/libxr871.a

# ============================================================
# Targets
# ============================================================

.PHONY: all clean lib info core sdk

all: lib

core: $(ARDUINO_CORE_OBJS)
	@echo "Built Arduino Core objects"

sdk: $(SDK_OBJS)
	@echo "Built SDK objects"

lib: $(LIB)

# Create directories
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)/sdk/driver/chip/flashchip
	@mkdir -p $(BUILD_DIR)/sdk/libc
	@mkdir -p $(BUILD_DIR)/core

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

# Compile SDK C files
$(BUILD_DIR)/sdk/%.o: $(XR871SDK)/src/%.c | $(BUILD_DIR)
	@echo "CC  $<"
	@$(CC) $(CFLAGS) -c $< -o $@ 2>&1 | grep -v "^$$\" | head -5

# Compile Arduino Core C files
$(BUILD_DIR)/core/%.o: $(CURDIR)/%.c | $(BUILD_DIR)
	@echo "CC  $<"
	@$(CC) $(CFLAGS) -c $< -o $@ 2>&1 | grep -v "^$$\" | head -5

# Compile Arduino Core C++ files
$(BUILD_DIR)/core/%.o: $(CURDIR)/%.cpp | $(BUILD_DIR)
	@echo "CXX $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@ 2>&1 | grep -v "^$$\" | head -5

# Create static library
$(LIB): $(SDK_OBJS) $(ARDUINO_CORE_OBJS) | $(LIB_DIR)
	@echo "AR  $@"
	@$(AR) rcs $@ $^
	@echo "Built: $$(ls -la $@ | awk '{print $$5, $$9}')"

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)

info:
	@echo "XR871 Arduino Core Build"
	@echo "SDK: $(XR871SDK)"
	@echo "CC:  $(CC)"
	@echo "SDK Sources: $(words $(SDK_SRCS)) files"
	@echo "Core Sources: $(words $(ARDUINO_CORE_SRCS)) files"
	@echo "Output: $(LIB)"
