/**
 * EMP-Guardian - Unit tests for the detection module (v2)
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * No external framework - plain assert()-based tests, meant as a clear
 * starting point. Build/run: `make test` from firmware/, or directly:
 *   gcc -Isrc tests/test_emp_detector.c src/emp_detector.c -o test_run && ./test_run
 *
 * emp_detector_hal_read_adc() is mocked here so tests run without hardware.
 */

#include <stdio.h>
#include <assert.h>
#include "emp_detector.h"
#include "config.h"

static uint16_t s_mock_adc_value = 0;

uint16_t emp_detector_hal_read_adc(void) {
    return s_mock_adc_value;
}

/* Feeds a fixed value for `count` samples and returns true if any of
 * them was reported as a confirmed event. */
static bool feed_constant(uint16_t value, int count) {
    bool any_detected = false;
    for (int i = 0; i < count; i++) {
        s_mock_adc_value = value;
        if (emp_detector_is_emp(emp_detector_read())) {
            any_detected = true;
        }
    }
    return any_detected;
}

static void test_no_false_positive_on_steady_noise(void) {
    emp_detector_init();
    /* Small, steady noise around a fixed level - amplitude never exceeds
     * baseline + margin, so this should never trigger. */
    bool detected = false;
    for (int i = 0; i < 200; i++) {
        s_mock_adc_value = 500 + (i % 5); /* +/- a few counts of jitter */
        if (emp_detector_is_emp(emp_detector_read())) {
            detected = true;
        }
    }
    assert(detected == false);
    printf("PASS: no false positive on steady low-level noise\n");
}

static void test_rejects_slow_industrial_switching(void) {
    /* This is the core improvement over v1: a signal that reaches a HIGH
     * amplitude, but does so gradually (small per-sample delta), must be
     * rejected by the slope criterion, even though a pure amplitude
     * threshold would have falsely triggered on it. */
    emp_detector_init();
    bool detected = false;

    uint16_t value = 500;
    for (int i = 0; i < 400; i++) {
        s_mock_adc_value = value;
        if (emp_detector_is_emp(emp_detector_read())) {
            detected = true;
        }
        if (value < 3000) {
            value += 5; /* slow ramp: well under EMP_SLOPE_THRESHOLD_ADC per sample */
        }
    }
    assert(detected == false);
    printf("PASS: slow-rising high-amplitude signal (industrial switching) is rejected\n");
}

static void test_detects_genuine_fast_transient(void) {
    /* A sudden, large, sustained jump - satisfies both the amplitude
     * criterion (far above baseline + margin) and the slope criterion
     * (delta far above EMP_SLOPE_THRESHOLD_ADC) for several consecutive
     * samples, so it must be confirmed. */
    emp_detector_init();

    /* Establish a stable low baseline first. */
    for (int i = 0; i < 50; i++) {
        s_mock_adc_value = 500;
        emp_detector_is_emp(emp_detector_read());
    }

    bool detected = feed_constant(3800, 5);
    assert(detected == true);
    printf("PASS: genuine fast, sustained transient is detected\n");
}

static void test_ignores_single_sample_spike(void) {
    /* A single-sample spike satisfies both criteria for one sample, but
     * does not sustain for EMP_CONFIRM_WINDOW_SAMPLES, so it must NOT be
     * confirmed - this is exactly what the confirmation window is for. */
    emp_detector_init();
    for (int i = 0; i < 50; i++) {
        s_mock_adc_value = 500;
        emp_detector_is_emp(emp_detector_read());
    }

    s_mock_adc_value = 4000; /* one isolated fast/high sample */
    bool detected_on_spike = emp_detector_is_emp(emp_detector_read());

    s_mock_adc_value = 500; /* immediately back to normal */
    bool detected_after = emp_detector_is_emp(emp_detector_read());

    assert(detected_on_spike == false);
    assert(detected_after == false);
    printf("PASS: isolated single-sample spike does not falsely confirm\n");
}

static void test_latch_and_rearm_cycle(void) {
    /* After a confirmed event, the detector must not immediately
     * re-trigger on every subsequent sample - it stays LATCHED until
     * emp_detector_reset_latch() is called, then passes through
     * COOLDOWN before returning to IDLE. */
    emp_detector_init();
    for (int i = 0; i < 50; i++) {
        s_mock_adc_value = 500;
        emp_detector_is_emp(emp_detector_read());
    }

    bool detected = feed_constant(3800, 5);
    assert(detected == true);
    assert(emp_detector_get_state() == EMP_DETECTOR_STATE_LATCHED);

    /* While latched, further samples must not report new events. */
    bool retriggered_while_latched = feed_constant(3800, 10);
    assert(retriggered_while_latched == false);

    emp_detector_reset_latch();
    assert(emp_detector_get_state() == EMP_DETECTOR_STATE_COOLDOWN);

    /* Feed enough normal samples to complete the cooldown period. */
    feed_constant(500, EMP_COOLDOWN_SAMPLES + 5);
    assert(emp_detector_get_state() == EMP_DETECTOR_STATE_IDLE);

    printf("PASS: latch -> reset -> cooldown -> idle cycle behaves correctly\n");
}

static void test_baseline_tracks_slow_drift(void) {
    /* The adaptive baseline should follow a slow, legitimate drift in
     * ambient noise (e.g. a temperature-related sensor offset change)
     * so that the detector does not eventually start false-triggering
     * on normal conditions just because they slowly moved. */
    emp_detector_init();

    uint16_t value = 500;
    for (int i = 0; i < 300; i++) {
        s_mock_adc_value = value;
        emp_detector_is_emp(emp_detector_read());
        if (value < 900) {
            value += 1; /* very slow drift */
        }
    }

    uint16_t baseline = emp_detector_get_baseline();
    /* Baseline should have followed the drift reasonably closely -
     * it won't match exactly (EWMA lag), but should be well within
     * the adaptive margin of the final value. */
    assert(baseline > (value - EMP_ADAPTIVE_MARGIN_ADC));
    printf("PASS: adaptive baseline tracks slow legitimate drift (baseline=%u, value=%u)\n",
           baseline, value);
}

int main(void) {
    test_no_false_positive_on_steady_noise();
    test_rejects_slow_industrial_switching();
    test_detects_genuine_fast_transient();
    test_ignores_single_sample_spike();
    test_latch_and_rearm_cycle();
    test_baseline_tracks_slow_drift();

    printf("\nAll tests passed.\n");
    return 0;
}
