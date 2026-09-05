/**
 * EMP-Guardian - Hardware Abstraction Layer (HAL) contract
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * This header is the ONE contract every board port must satisfy. None of
 * the portable modules (app.c, emp_detector.c, shield_control.c, comms.c)
 * may include a vendor SDK header directly - they only ever call the
 * functions declared here.
 *
 * A board port is a single .c file under firmware/boards/<board>/ that
 * implements every function below against that board's real SDK
 * (STM32Cube HAL, Pico SDK, ESP-IDF, ...). See firmware/boards/stm32/
 * and firmware/boards/rp2040/ for two reference ports.
 *
 * firmware/tests/ satisfies this same contract with plain mocks, so the
 * portable modules can be unit-tested on a host machine with no hardware
 * and no board SDK at all.
 */

#ifndef EMP_HAL_H
#define EMP_HAL_H

#include <stdint.h>
#include <stdbool.h>

/**
 * One-time board bring-up: clocks, GPIO direction/mode, ADC peripheral,
 * UART peripheral, and any other board-specific init. Called once, first,
 * by emp_guardian_app_init() before any other module is initialized.
 */
void emp_hal_board_init(void);

/**
 * Samples the sensor's analog front-end and returns the raw ADC reading.
 * Resolution/scale is board-dependent (e.g. 0-4095 for a 12-bit ADC);
 * emp_detector.c's thresholds are tuned in raw ADC counts, so keep the
 * same resolution across ports or re-tune config.h accordingly.
 */
uint16_t emp_detector_hal_read_adc(void);

/**
 * Drives the shield/disconnect actuator GPIO. `pin` is reserved for
 * multi-actuator boards; single-actuator reference ports ignore it.
 * `level` true = actuator engaged (shield active).
 */
void emp_hal_gpio_write(int pin, bool level);

/** Busy/blocking delay, in milliseconds. */
void emp_hal_delay_ms(uint32_t ms);

/** Initializes the telemetry UART at the given baud rate. */
void emp_hal_uart_init(uint32_t baudrate);

/** Blocking write of `len` bytes from `data` over the telemetry UART. */
void emp_hal_uart_write(const char *data, int len);

#endif /* EMP_HAL_H */
