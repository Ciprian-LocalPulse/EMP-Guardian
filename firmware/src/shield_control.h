/**
 * EMP-Guardian - Control actuator de ecranare
 * Autor: Ciprian Ștefan Pleșca
 * Licență: MIT
 */

#ifndef SHIELD_CONTROL_H
#define SHIELD_CONTROL_H

#include <stdbool.h>

typedef enum {
    SHIELD_STATE_NORMAL = 0,
    SHIELD_STATE_ACTIVE = 1,
    SHIELD_STATE_FAULT  = 2
} shield_state_t;

void shield_control_init(void);
void shield_control_activate(void);
void shield_control_deactivate(void);
void shield_control_wait_reset(void);
shield_state_t shield_control_get_state(void);

#endif /* SHIELD_CONTROL_H */
