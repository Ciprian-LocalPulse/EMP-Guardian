# EMP-Guardian - Firmware Makefile
# Author: Ciprian Ștefan Pleșca
# License: MIT
#
# This Makefile is a generic starting point. For a real target
# (e.g. STM32F4), add the board-specific flags (mcu, linker script,
# startup file) per your chosen toolchain (e.g. arm-none-eabi-gcc).

TARGET      = emp_guardian
BUILD_DIR   = build
SRC_DIR     = src
INC_DIR     = include

CC          = arm-none-eabi-gcc
CFLAGS      = -Wall -Wextra -O2 -std=c11 -I$(SRC_DIR) -I$(INC_DIR)
LDFLAGS     =

SOURCES     = $(wildcard $(SRC_DIR)/*.c)
OBJECTS     = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

.PHONY: all clean test

all: $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	arm-none-eabi-objcopy -O binary $< $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test:
	@mkdir -p $(BUILD_DIR)
	gcc -Wall -I$(SRC_DIR) tests/test_emp_detector.c $(SRC_DIR)/emp_detector.c -o $(BUILD_DIR)/test_run
	./$(BUILD_DIR)/test_run
clean:
	rm -rf $(BUILD_DIR)
