/**
 * EMP-Guardian - HAL implementation for ESP32 (ESP-IDF)
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * BUILD NOTE: requires ESP-IDF (idf.py build system). This file is a
 * real skeleton against ESP-IDF's actual driver APIs, but has NOT
 * been validated against physical hardware - see MANIFESTO.md's
 * epistemic-status section. GPIO/ADC pin numbers below are
 * illustrative defaults; adjust to your PCB.
 *
 * IMPORTANT PORTABILITY CAVEAT: ESP32's ADC is significantly slower
 * and noisier than the >=1 MSPS / 12-bit target in docs/hardware_specs.md.
 * If you port to ESP32, re-validate the detection latency budget in
 * docs/theory_of_operation.md against ESP32's real ADC performance -
 * do not assume the STM32-derived latency estimates carry over.
 */

#include "hal.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SHIELD_ACTUATOR_GPIO    GPIO_NUM_2
#define SENSOR_ADC_UNIT         ADC_UNIT_1
#define SENSOR_ADC_CHANNEL      ADC_CHANNEL_6   /* GPIO34 on most ESP32 boards */
#define TELEMETRY_UART_PORT     UART_NUM_0
#define TELEMETRY_UART_TX_PIN   GPIO_NUM_1
#define TELEMETRY_UART_RX_PIN   GPIO_NUM_3

static adc_oneshot_unit_handle_t s_adc_handle;

void emp_hal_board_init(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << SHIELD_ACTUATOR_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
    gpio_set_level(SHIELD_ACTUATOR_GPIO, 0);

    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = SENSOR_ADC_UNIT,
    };
    adc_oneshot_new_unit(&init_config, &s_adc_handle);

    adc_oneshot_chan_cfg_t chan_config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_11,
    };
    adc_oneshot_config_channel(s_adc_handle, SENSOR_ADC_CHANNEL, &chan_config);
}

uint16_t emp_detector_hal_read_adc(void) {
    int raw = 0;
    adc_oneshot_read(s_adc_handle, SENSOR_ADC_CHANNEL, &raw);
    return (uint16_t)raw;
}

void emp_hal_gpio_write(int pin, bool level) {
    (void)pin; /* single actuator pin in this reference design */
    gpio_set_level(SHIELD_ACTUATOR_GPIO, level ? 1 : 0);
}

void emp_hal_delay_ms(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void emp_hal_uart_init(uint32_t baudrate) {
    uart_config_t uart_config = {
        .baud_rate = (int)baudrate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_param_config(TELEMETRY_UART_PORT, &uart_config);
    uart_set_pin(TELEMETRY_UART_PORT, TELEMETRY_UART_TX_PIN, TELEMETRY_UART_RX_PIN,
                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(TELEMETRY_UART_PORT, 256, 0, 0, NULL, 0);
}

void emp_hal_uart_write(const char *data, int len) {
    uart_write_bytes(TELEMETRY_UART_PORT, data, (size_t)len);
}
