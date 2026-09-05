/**
 * EMP-Guardian - Integration tests for app.c (the portable init/step loop)
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * Exercises the full wiring between emp_detector, shield_control and
 * comms exactly as app.c orchestrates them, with the board replaced by
 * tests/hal_mock.c. This is the closest thing to an end-to-end test
 * that can run without real hardware.
 *
 * Build/run: from firmware/, `make test` (see Makefile), or directly:
 *   gcc -Isrc -Iinclude tests/test_app.c tests/hal_mock.c \
 *       src/app.c src/emp_detector.c src/shield_control.c src/comms.c \
 *       -o test_app_run && ./test_app_run
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "app.h"
#include "emp_detector.h"
#include "shield_control.h"
#include "config.h"

extern bool     s_mock_board_init_called;
extern uint16_t s_mock_adc_value;
extern bool     s_mock_gpio_level;
extern uint32_t s_mock_uart_baudrate;
extern char     s_mock_last_uart_write[256];

static void test_init_brings_up_board_and_modules(void) {
    s_mock_board_init_called = false;
    s_mock_adc_value = 500; /* plausible mid-range value for self-test */

    emp_guardian_app_init();

    assert(s_mock_board_init_called == true);
    assert(s_mock_uart_baudrate == UART_BAUDRATE);
    assert(shield_control_get_state() == SHIELD_STATE_NORMAL);
    /* recalibrate() should have pulled the baseline close to the flat
     * mock ADC value used during init. */
    assert(emp_detector_get_baseline() == 500);
    printf("PASS: app_init() brings up the board and every module in order\n");
}

static void test_step_is_quiet_on_normal_readings(void) {
    s_mock_adc_value = 500;
    emp_guardian_app_init();

    for (int i = 0; i < 20; i++) {
        emp_guardian_app_step();
    }

    assert(s_mock_gpio_level == false);
    assert(shield_control_get_state() == SHIELD_STATE_NORMAL);
    printf("PASS: app_step() stays quiet through normal, steady readings\n");
}

static void test_step_activates_shield_and_reports_on_transient(void) {
    s_mock_adc_value = 500;
    emp_guardian_app_init();

    /* Settle the baseline with a run of normal samples first. */
    for (int i = 0; i < 50; i++) {
        s_mock_adc_value = 500;
        emp_guardian_app_step();
    }

    /* Sustained fast, high-amplitude transient - must confirm. */
    for (int i = 0; i < 5; i++) {
        s_mock_adc_value = 3800;
        emp_guardian_app_step();
    }

    assert(s_mock_gpio_level == true);
    assert(shield_control_get_state() == SHIELD_STATE_ACTIVE);
    assert(strncmp(s_mock_last_uart_write, "STATUS:", 7) == 0);
    printf("PASS: app_step() activates the shield and reports over comms on a confirmed event\n");
}

int main(void) {
    test_init_brings_up_board_and_modules();
    test_step_is_quiet_on_normal_readings();
    test_step_activates_shield_and_reports_on_transient();

    printf("\nAll app tests passed.\n");
    return 0;
}
