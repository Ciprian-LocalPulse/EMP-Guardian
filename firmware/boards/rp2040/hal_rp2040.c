/**
 * EMP-Guardian - HAL implementation for RP2040 (Raspberry Pi Pico SDK)
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * BUILD NOTE: requires the Pico SDK and its CMake build system. This
 * file is a real skeleton against the Pico SDK's actual APIs, but has
 * NOT been validated against physical hardware - see MANIFESTO.md's
 * epistemic-status section. Pin numbers below are illustrative
 * defaults; adjust to your PCB.
 *
 * IMPORTANT PORTABILITY CAVEAT: RP2040's onboard ADC is 12-bit but
 * limited to ~500 KSPS, below the >=1 MSPS target in
 * docs/hardware_specs.md. If you port to RP2040, either accept the
 * reduced sample rate (and re-validate the latency budget in
 * docs/theory_of_operation.md accordingly) or use an external
 * high-speed ADC over SPI instead of the onboard one.
 */

#include "hal.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/uart.h"

#define SHIELD_ACTUATOR_PIN     15
#define SENSOR_ADC_GPIO         26      /* ADC0 on GPIO26 */
#define SENSOR_ADC_INPUT        0
#define TELEMETRY_UART_INST     uart0
#define TELEMETRY_UART_TX_PIN   0
#define TELEMETRY_UART_RX_PIN   1

void emp_hal_board_init(void) {
    gpio_init(SHIELD_ACTUATOR_PIN);
    gpio_set_dir(SHIELD_ACTUATOR_PIN, GPIO_OUT);
    gpio_put(SHIELD_ACTUATOR_PIN, 0);

    adc_init();
    adc_gpio_init(SENSOR_ADC_GPIO);
    adc_select_input(SENSOR_ADC_INPUT);
}

uint16_t emp_detector_hal_read_adc(void) {
    return adc_read(); /* 12-bit, 0-4095 */
}

void emp_hal_gpio_write(int pin, bool level) {
    (void)pin; /* single actuator pin in this reference design */
    gpio_put(SHIELD_ACTUATOR_PIN, level ? 1 : 0);
}

void emp_hal_delay_ms(uint32_t ms) {
    sleep_ms(ms);
}

void emp_hal_uart_init(uint32_t baudrate) {
    uart_init(TELEMETRY_UART_INST, baudrate);
    gpio_set_function(TELEMETRY_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(TELEMETRY_UART_RX_PIN, GPIO_FUNC_UART);
}

void emp_hal_uart_write(const char *data, int len) {
    uart_write_blocking(TELEMETRY_UART_INST, (const uint8_t *)data, (size_t)len);
}
