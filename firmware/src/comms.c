/**
 * EMP-Guardian - Communication module (event reporting)
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * Portability note: this file calls exclusively through the HAL
 * contract in firmware/include/hal.h. See firmware/boards/<board>/
 * for the actual per-board implementations of emp_hal_uart_init()
 * and emp_hal_uart_write().
 */

#include "comms.h"
#include "config.h"
#include "hal.h"
#include <stdio.h>

void comms_init(void) {
    emp_hal_uart_init(UART_BAUDRATE);
}

void comms_send_alert(const char *message) {
    char buffer[64];
    int len = snprintf(buffer, sizeof(buffer), "ALERT:%s\r\n", message);
    if (len > 0) {
        emp_hal_uart_write(buffer, len);
    }
}

void comms_send_status(const char *key, int value) {
    char buffer[64];
    int len = snprintf(buffer, sizeof(buffer), "STATUS:%s=%d\r\n", key, value);
    if (len > 0) {
        emp_hal_uart_write(buffer, len);
    }
}
