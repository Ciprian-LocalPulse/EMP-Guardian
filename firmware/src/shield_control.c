/**
 * EMP-Guardian - Control actuator de ecranare
 * Autor: Ciprian Ștefan Pleșca
 * Licență: MIT
 *
 * Notă de portabilitate: acest fișier este scris generic, cu funcții HAL
 * (emp_hal_gpio_write, emp_hal_delay_ms) care trebuie implementate pentru
 * placa țintă. Un exemplu de implementare HAL pentru STM32 HAL Library
 * este inclus mai jos, comentat.
 */

#include "shield_control.h"
#include "config.h"

/* Strat HAL generic - implementează aceste funcții pentru placa ta */
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
    /* Reset manual: rămâne în stare activă până la intervenția
     * unui operator, prin comanda dedicată din stratul de comunicație. */
    (void)0;
#endif
}

shield_state_t shield_control_get_state(void) {
    return s_state;
}

/* ------------------------------------------------------------------
 * Exemplu de implementare HAL pentru STM32 HAL Library (comentat).
 * Decomentează și adaptează pentru placa ta.
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
