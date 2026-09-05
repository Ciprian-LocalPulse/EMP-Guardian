/**
 * EMP-Guardian - Shielding actuator control
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * Portability note: this file calls exclusively through the HAL
 * contract in firmware/include/hal.h. See firmware/boards/<board>/
 * for the actual per-board implementations of emp_hal_gpio_write()
 * and emp_hal_delay_ms().
 */

#include "shield_control.h"
#include "config.h"
#include "hal.h"

#define SHIELD_ACTUATOR_PIN  0

static shield_state_t s_state = SHIELD_STATE_NORMAL;

void shield_control_init(void) {
    emp_hal_gpio_write(SHIELD_ACTUATOR_PIN, false);
    s_state = SHIELD_STATE_NORMAL;
}

void shield_control_activate(void) {
    emp_hal_gpio_write(SHIELD_ACTUATOR_PIN, true);
    s_state = SHIELD_STATE_ACTIVE;
}

void shield_control_deactivate(void) {
    emp_hal_gpio_write(SHIELD_ACTUATOR_PIN, false);
    s_state = SHIELD_STATE_NORMAL;
}

void shield_control_wait_reset(void) {
#if SHIELD_AUTO_RESET
    emp_hal_delay_ms(SHIELD_RESET_DELAY_MS);
    shield_control_deactivate();
#else
    /* Manual reset: stays in active state until an operator intervenes,
     * via the dedicated command in the communication layer. */
    (void)0;
#endif
}

shield_state_t shield_control_get_state(void) {
    return s_state;
}
