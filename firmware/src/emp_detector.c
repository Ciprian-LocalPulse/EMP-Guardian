/**
 * EMP-Guardian - EMP detection module
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * Portable implementation of the detection algorithm. Hardware access
 * functions (ADC) are isolated in emp_detector_hal_read_adc() so that
 * this file can be unit-tested without real hardware (see firmware/tests/).
 */

#include "emp_detector.h"
#include "config.h"

/* Declared in the board-specific HAL layer (not included in this generic repo) */
extern uint16_t emp_detector_hal_read_adc(void);

static uint16_t s_baseline_noise = 0;
static uint8_t  s_above_threshold_counter = 0;
static bool     s_latched = false;

void emp_detector_init(void) {
    s_baseline_noise = 0;
    s_above_threshold_counter = 0;
    s_latched = false;
    /* Real peripheral initialization (ADC, DMA, timers) is done in the board HAL. */
}

uint16_t emp_detector_read(void) {
    return emp_detector_hal_read_adc();
}

bool emp_detector_is_emp(uint16_t value) {
    uint16_t effective_threshold = EMP_THRESHOLD_ADC;

    /* Hysteresis: if we are already "latched", we lower the exit threshold,
     * not the entry threshold, to avoid rapid oscillation. */
    if (s_latched) {
        effective_threshold = EMP_THRESHOLD_ADC - EMP_HYSTERESIS_ADC;
    }

    if (value >= effective_threshold) {
        s_above_threshold_counter++;
    } else {
        s_above_threshold_counter = 0;
        s_latched = false;
        return false;
    }

    /* Confirmation time window: we require the signal to stay above
     * threshold for a minimum number of consecutive samples, to filter
     * out isolated noise spikes. */
    if (s_above_threshold_counter >= EMP_CONFIRM_WINDOW_US) {
        s_latched = true;
        return true;
    }

    return false;
}

bool emp_detector_self_test(void) {
    uint16_t sample = emp_detector_hal_read_adc();
    /* A minimal self-test: check that the ADC responds within a
     * plausible range (not stuck at 0 or at maximum value). */
    return (sample > 10 && sample < 4085);
}

void emp_detector_recalibrate(void) {
    /* Simple recalibration based on a moving average of the background noise.
     * The full implementation (IIR filter/moving average) should be adapted
     * to the actual characteristics of the sensor used. */
    uint32_t sum = 0;
    const int samples = 32;
    for (int i = 0; i < samples; i++) {
        sum += emp_detector_hal_read_adc();
    }
    s_baseline_noise = (uint16_t)(sum / samples);
}
