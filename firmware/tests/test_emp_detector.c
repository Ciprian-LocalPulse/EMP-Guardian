/**
 * EMP-Guardian - Teste unitare pentru modulul de detecție
 * Autor: Ciprian Ștefan Pleșca
 * Licență: MIT
 *
 * Teste minimale, fără framework extern, gândite ca punct de plecare.
 * Rulare: gcc -I../src test_emp_detector.c ../src/emp_detector.c -o test_run && ./test_run
 *
 * Notă: emp_detector_hal_read_adc() este simulată aici (mock),
 * astfel încât testele să ruleze fără hardware real.
 */

#include <stdio.h>
#include <assert.h>
#include "emp_detector.h"

static uint16_t s_mock_adc_value = 0;

uint16_t emp_detector_hal_read_adc(void) {
    return s_mock_adc_value;
}

static void test_no_false_positive_on_normal_signal(void) {
    emp_detector_init();
    s_mock_adc_value = 500; /* nivel normal, sub prag */
    for (int i = 0; i < 10; i++) {
        assert(emp_detector_is_emp(emp_detector_read()) == false);
    }
    printf("PASS: fara falsi pozitivi pe semnal normal\n");
}

static void test_detects_sustained_high_signal(void) {
    emp_detector_init();
    s_mock_adc_value = 3500; /* peste EMP_THRESHOLD_ADC */
    bool detected = false;
    for (int i = 0; i < 5; i++) {
        if (emp_detector_is_emp(emp_detector_read())) {
            detected = true;
            break;
        }
    }
    assert(detected == true);
    printf("PASS: semnal susținut peste prag este detectat\n");
}

static void test_ignores_single_sample_spike(void) {
    emp_detector_init();
    s_mock_adc_value = 500;
    emp_detector_is_emp(emp_detector_read());

    s_mock_adc_value = 4000; /* vârf izolat de o singură eșantionare */
    bool detected_immediately = emp_detector_is_emp(emp_detector_read());

    s_mock_adc_value = 500; /* revine imediat la normal */
    bool still_detected = emp_detector_is_emp(emp_detector_read());

    assert(still_detected == false);
    (void)detected_immediately; /* poate fi true sau false, depinde de EMP_CONFIRM_WINDOW_US */
    printf("PASS: varf izolat nu ramane 'latched'\n");
}

int main(void) {
    test_no_false_positive_on_normal_signal();
    test_detects_sustained_high_signal();
    test_ignores_single_sample_spike();
    printf("\nToate testele au trecut.\n");
    return 0;
}
