/**
 * EMP-Guardian - Bucla principală firmware
 * Autor: Ciprian Ștefan Pleșca
 * Licență: MIT
 */

#include "emp_detector.h"
#include "shield_control.h"
#include "comms.h"
#include "config.h"

int main(void) {
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

    emp_detector_recalibrate();

    while (1) {
        uint16_t adc_value = emp_detector_read();

        if (emp_detector_is_emp(adc_value)) {
            shield_control_activate();
            comms_send_alert("EMP DETECTED");
            comms_send_status("shield_state", (int)shield_control_get_state());
            shield_control_wait_reset();
        }

        /* Pauză scurtă pentru a nu satura bucla de eșantionare;
         * într-o implementare reală, se recomandă declanșare pe
         * întrerupere de timer în loc de buclă de tip busy-wait. */
        for (volatile int i = 0; i < 10000; i++) { }
    }
}
