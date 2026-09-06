# EMP-Guardian - Firmware Makefile
# Author: Ciprian Ștefan Pleșca
# License: MIT
#
# Portable modules live in src/ and never touch a vendor SDK directly -
# they only call the contract in include/hal.h. Each board's real HAL
# implementation lives in boards/<board>/.
#
#   make test            - host-side unit tests (no board SDK needed)
#   make BOARD=stm32      - cross-compile for STM32 (needs a real
#                           STM32Cube project for stm32f4xx_hal.h etc.;
#                           see boards/stm32/hal_stm32.c's header comment)
#   make BOARD=rp2040      - RP2040 uses the Pico SDK's own CMake build,
#                           not this Makefile - see boards/rp2040/hal_rp2040.c
#   make BOARD=esp32       - ESP32 uses ESP-IDF's own idf.py build,
#                           not this Makefile - see boards/esp32/hal_esp32.c
#
# BOARD defaults to stm32, the only board with a Makefile-compatible
# vendor toolchain among the current reference ports.

BOARD       ?= stm32
TARGET      = emp_guardian
BUILD_DIR   = build/$(BOARD)
SRC_DIR     = src
INC_DIR     = include
BOARD_DIR   = boards/$(BOARD)

CC          = arm-none-eabi-gcc
CFLAGS      = -Wall -Wextra -O2 -std=c11 -I$(SRC_DIR) -I$(INC_DIR)
LDFLAGS     =

CORE_SOURCES  = $(wildcard $(SRC_DIR)/*.c)
BOARD_SOURCES = $(wildcard $(BOARD_DIR)/*.c)
SOURCES       = $(CORE_SOURCES) $(BOARD_SOURCES)
OBJECTS       = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SOURCES)))
VPATH         = $(SRC_DIR):$(BOARD_DIR)

.PHONY: all clean test

all: $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	arm-none-eabi-objcopy -O binary $< $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Host-side unit tests: no board, no cross-compiler - plain host gcc
# against the portable modules with the HAL mocked in tests/.
test:
	@mkdir -p build/test
	gcc -Wall -Wextra -I$(SRC_DIR) -I$(INC_DIR) \
		tests/test_emp_detector.c $(SRC_DIR)/emp_detector.c \
		-o build/test/test_emp_detector
	./build/test/test_emp_detector
	@echo
	gcc -Wall -Wextra -I$(SRC_DIR) -I$(INC_DIR) \
		tests/test_shield_control.c tests/hal_mock.c $(SRC_DIR)/shield_control.c \
		-o build/test/test_shield_control
	./build/test/test_shield_control
	@echo
	gcc -Wall -Wextra -I$(SRC_DIR) -I$(INC_DIR) \
		tests/test_app.c tests/hal_mock.c \
		$(SRC_DIR)/app.c $(SRC_DIR)/emp_detector.c $(SRC_DIR)/shield_control.c $(SRC_DIR)/comms.c \
		-o build/test/test_app
	./build/test/test_app

clean:
	rm -rf build
