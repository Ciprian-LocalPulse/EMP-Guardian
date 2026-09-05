/**
 * EMP-Guardian - Firmware entry point
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * This file is intentionally board-agnostic: it contains no HAL calls
 * and no vendor SDK includes. The exact same main() is linked against
 * any board port (STM32, RP2040, a future port) - see
 * firmware/boards/<board>/ for the hardware-specific code, and app.c
 * for the portable init/step sequence this file calls.
 */

#include "app.h"

int main(void) {
    emp_guardian_app_init();

    while (1) {
        emp_guardian_app_step();

        /* Pacing between samples. This busy-wait is a placeholder for a
         * bare-metal superloop; a real deployment should instead drive
         * emp_guardian_app_step() from a timer ISR or an RTOS task at
         * the sample rate required by docs/hardware_specs.md, so the
         * detection latency budget in docs/theory_of_operation.md is
         * met deterministically. */
        for (volatile int i = 0; i < 10000; i++) { }
    }
}
