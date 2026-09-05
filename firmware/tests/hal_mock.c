/**
 * EMP-Guardian - Shared HAL mock for host-side unit tests
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * Implements every function in firmware/include/hal.h with plain,
 * inspectable state instead of real hardware, so any portable module
 * (shield_control.c, app.c, comms.c) can be unit-tested on a host
 * machine with no board SDK at all. Test files read the s_mock_*
 * globals directly to assert on what the code under test did.
 */

#include "hal.h"
#include <string.h>

bool     s_mock_board_init_called = false;
uint16_t s_mock_adc_value         = 0;
bool     s_mock_gpio_level        = false;
int      s_mock_gpio_pin          = -1;
uint32_t s_mock_total_delay_ms    = 0;
uint32_t s_mock_uart_baudrate     = 0;
char     s_mock_last_uart_write[256];

void emp_hal_board_init(void) {
    s_mock_board_init_called = true;
}

uint16_t emp_detector_hal_read_adc(void) {
    return s_mock_adc_value;
}

void emp_hal_gpio_write(int pin, bool level) {
    s_mock_gpio_pin   = pin;
    s_mock_gpio_level = level;
}

void emp_hal_delay_ms(uint32_t ms) {
    s_mock_total_delay_ms += ms;
}

void emp_hal_uart_init(uint32_t baudrate) {
    s_mock_uart_baudrate = baudrate;
}

void emp_hal_uart_write(const char *data, int len) {
    int copy_len = len < (int)sizeof(s_mock_last_uart_write) - 1
                       ? len
                       : (int)sizeof(s_mock_last_uart_write) - 1;
    memcpy(s_mock_last_uart_write, data, (size_t)copy_len);
    s_mock_last_uart_write[copy_len] = '\0';
}
