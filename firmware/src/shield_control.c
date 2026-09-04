/**
 * EMP-Guardian - Shielding actuator control
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * Portability note: this file is written generically, with HAL functions
 * (emp_hal_gpio_write, emp_hal_delay_ms) that must be implemented for the
 * target board. An example HAL implementation for the STM32 HAL Library
 * is included below, commented out.
 */

#include "shield_control.h"
#include "config.h"

/* Generic HAL layer - implement these functions for your board */
extern void emp_hal_gpio_write(int pin, bool level);
extern void emp_hal_delay_ms(uint32_t ms);

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

/* ------------------------------------------------------------------
 * Example HAL implementation for the STM32 HAL Library (commented out).
 * Uncomment and adapt for your board.
 *
 * #include "stm32f4xx_hal.h"
 *
 * void emp_hal_gpio_write(int pin, bool level) {
 *     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, level ? GPIO_PIN_SET : GPIO_PIN_RESET);
 * }
 *
 * void emp_hal_delay_ms(uint32_t ms) {
 *     HAL_Delay(ms);
 * }
 * ------------------------------------------------------------------ */
