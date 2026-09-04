/**
 * EMP-Guardian - Unit tests for the detection module
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * Minimal tests, no external framework, intended as a starting point.
 * Run: gcc -I../src test_emp_detector.c ../src/emp_detector.c -o test_run && ./test_run
 *
 * Note: emp_detector_hal_read_adc() is mocked here, so the tests can
 * run without real hardware.
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
    s_mock_adc_value = 500; /* normal level, below threshold */
    for (int i = 0; i < 10; i++) {
        assert(emp_detector_is_emp(emp_detector_read()) == false);
    }
    printf("PASS: no false positives on normal signal\n");
}

static void test_detects_sustained_high_signal(void) {
    emp_detector_init();
    s_mock_adc_value = 3500; /* above EMP_THRESHOLD_ADC */
    bool detected = false;
    for (int i = 0; i < 5; i++) {
        if (emp_detector_is_emp(emp_detector_read())) {
            detected = true;
            break;
        }
    }
    assert(detected == true);
    printf("PASS: sustained signal above threshold is detected\n");
}

static void test_ignores_single_sample_spike(void) {
    emp_detector_init();
    s_mock_adc_value = 500;
    emp_detector_is_emp(emp_detector_read());

    s_mock_adc_value = 4000; /* isolated single-sample spike */
    bool detected_immediately = emp_detector_is_emp(emp_detector_read());

    s_mock_adc_value = 500; /* returns to normal immediately */
    bool still_detected = emp_detector_is_emp(emp_detector_read());

    assert(still_detected == false);
    (void)detected_immediately; /* may be true or false, depending on EMP_CONFIRM_WINDOW_US */
    printf("PASS: isolated spike does not remain 'latched'\n");
}

int main(void) {
    test_no_false_positive_on_normal_signal();
    test_detects_sustained_high_signal();
    test_ignores_single_sample_spike();
    printf("\nAll tests passed.\n");
    return 0;
}
