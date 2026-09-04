/**
 * EMP-Guardian - Communication module (event reporting)
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 */

#include "comms.h"
#include "config.h"
#include <stdio.h>

/* Generic UART HAL layer - implement for your board */
extern void emp_hal_uart_init(uint32_t baudrate);
extern void emp_hal_uart_write(const char *data, int len);

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
