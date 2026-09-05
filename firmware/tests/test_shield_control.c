/**
 * EMP-Guardian - Unit tests for shield_control.c
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * Build/run: from firmware/, `make test` (see Makefile), or directly:
 *   gcc -Isrc -Iinclude tests/test_shield_control.c tests/hal_mock.c \
 *       src/shield_control.c -o test_shield_run && ./test_shield_run
 *
 * Uses tests/hal_mock.c so this runs on a host machine with no board SDK.
 */

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "shield_control.h"
#include "config.h"

/* Mock state defined in hal_mock.c - read directly to assert on what
 * shield_control.c told the HAL to do. */
extern bool     s_mock_gpio_level;
extern int      s_mock_gpio_pin;
extern uint32_t s_mock_total_delay_ms;

static void test_init_deactivates_actuator(void) {
    s_mock_gpio_level = true; /* start "dirty" to prove init clears it */
    shield_control_init();
    assert(s_mock_gpio_level == false);
    assert(shield_control_get_state() == SHIELD_STATE_NORMAL);
    printf("PASS: init drives the actuator low and starts in NORMAL state\n");
}

static void test_activate_drives_actuator_high(void) {
    shield_control_init();
    shield_control_activate();
    assert(s_mock_gpio_level == true);
    assert(shield_control_get_state() == SHIELD_STATE_ACTIVE);
    printf("PASS: activate() drives the actuator high and enters ACTIVE state\n");
}

static void test_deactivate_drives_actuator_low(void) {
    shield_control_init();
    shield_control_activate();
    shield_control_deactivate();
    assert(s_mock_gpio_level == false);
    assert(shield_control_get_state() == SHIELD_STATE_NORMAL);
    printf("PASS: deactivate() drives the actuator low and returns to NORMAL state\n");
}

static void test_wait_reset_respects_manual_mode(void) {
    /* config.h ships with SHIELD_AUTO_RESET=0 (manual reset recommended
     * for critical environments) - wait_reset() must NOT auto-clear the
     * actuator or consume any delay in that mode. */
    shield_control_init();
    shield_control_activate();
    uint32_t delay_before = s_mock_total_delay_ms;

    shield_control_wait_reset();

#if SHIELD_AUTO_RESET
    assert(s_mock_gpio_level == false);
#else
    assert(s_mock_gpio_level == true);
    assert(s_mock_total_delay_ms == delay_before);
#endif
    printf("PASS: wait_reset() honors the configured SHIELD_AUTO_RESET mode\n");
}

int main(void) {
    test_init_deactivates_actuator();
    test_activate_drives_actuator_high();
    test_deactivate_drives_actuator_low();
    test_wait_reset_respects_manual_mode();

    printf("\nAll shield_control tests passed.\n");
    return 0;
}
