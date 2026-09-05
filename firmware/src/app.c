/**
 * EMP-Guardian - Portable application loop
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * Implementation of the board-independent init/step sequence declared
 * in app.h. This file must never include a vendor SDK header directly
 * - all hardware access goes through firmware/include/hal.h.
 */

#include "app.h"
#include "emp_detector.h"
#include "shield_control.h"
#include "comms.h"
#include "hal.h"
#include "config.h"

void emp_guardian_app_init(void) {
    emp_hal_board_init();

    emp_detector_init();
    shield_control_init();
    comms_init();

#if SELF_TEST_ON_BOOT
    if (!emp_detector_self_test()) {
        comms_send_alert("SELF_TEST_FAIL");
    } else {
        comms_send_alert("SELF_TEST_OK");
    }
#endif

    /* One-shot burst calibration so the adaptive baseline starts close
     * to the real ambient noise floor instead of ramping up from zero. */
    emp_detector_recalibrate();
}

void emp_guardian_app_step(void) {
    uint16_t adc_value = emp_detector_read();

    if (emp_detector_is_emp(adc_value)) {
        shield_control_activate();
        comms_send_alert("EMP DETECTED");
        comms_send_status("shield_state", (int)shield_control_get_state());
        comms_send_status("baseline", (int)emp_detector_get_baseline());

        shield_control_wait_reset();

        /* Re-arm the detector: LATCHED -> COOLDOWN -> IDLE. See
         * docs/theory_of_operation.md for why cooldown exists. */
        emp_detector_reset_latch();
    }
}
