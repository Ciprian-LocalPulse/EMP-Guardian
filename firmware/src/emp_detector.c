/**
 * EMP-Guardian - EMP detection module (v2 - dual-criterion state machine)
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * Portable implementation. Hardware access is isolated in
 * emp_detector_hal_read_adc() so this file can be unit-tested without
 * real hardware (see firmware/tests/).
 */

#include "emp_detector.h"
#include "config.h"
#include "hal.h"

/* emp_detector_hal_read_adc() is declared in hal.h and implemented per
 * board in firmware/boards/<board>/. */

static emp_detector_state_t s_state           = EMP_DETECTOR_STATE_IDLE;
static uint16_t              s_baseline        = 0;
static uint16_t              s_prev_value      = 0;
static uint8_t                s_confirm_counter = 0;
static uint16_t              s_cooldown_counter = 0;
static bool                   s_have_prev_value = false;

/* ------------------------------------------------------------------ */
/* Internal helpers                                                    */
/* ------------------------------------------------------------------ */

/* Updates the adaptive baseline using a fixed-point EWMA:
 *   baseline += (value - baseline) >> ALPHA_SHIFT
 * Computed in int32_t to correctly handle the case value < baseline. */
static void update_baseline(uint16_t value) {
    int32_t delta = (int32_t)value - (int32_t)s_baseline;
    s_baseline = (uint16_t)(s_baseline + (delta >> EMP_BASELINE_ALPHA_SHIFT));
}

/* Amplitude criterion: is the sample above the adaptive threshold?
 * A small amount of hysteresis is applied while a candidate event is
 * being confirmed, so a single noisy sample near the boundary doesn't
 * reset the confirmation counter. */
static bool amplitude_criterion_met(uint16_t value, bool confirming) {
    uint32_t threshold = (uint32_t)s_baseline + EMP_ADAPTIVE_MARGIN_ADC;
    if (confirming && threshold >= EMP_HYSTERESIS_ADC) {
        threshold -= EMP_HYSTERESIS_ADC;
    }
    return value >= threshold;
}

/* Rate-of-rise criterion: did the signal jump by more than the
 * configured slope threshold since the previous sample? Uses the
 * absolute value of the delta, since a fast *falling* edge immediately
 * following a fast rising edge is also characteristic of a genuine
 * transient (see the double-exponential waveform in the project wiki). */
static bool slope_criterion_met(uint16_t value) {
    if (!s_have_prev_value) {
        return false;
    }
    int32_t delta = (int32_t)value - (int32_t)s_prev_value;
    if (delta < 0) {
        delta = -delta;
    }
    return delta >= EMP_SLOPE_THRESHOLD_ADC;
}

/* ------------------------------------------------------------------ */
/* Public API                                                          */
/* ------------------------------------------------------------------ */

void emp_detector_init(void) {
    s_state            = EMP_DETECTOR_STATE_IDLE;
    s_baseline          = 0;
    s_prev_value        = 0;
    s_confirm_counter   = 0;
    s_cooldown_counter  = 0;
    s_have_prev_value   = false;
    /* Real peripheral initialization (ADC, DMA, timers) is done in the board HAL. */
}

uint16_t emp_detector_read(void) {
    return emp_detector_hal_read_adc();
}

bool emp_detector_is_emp(uint16_t value) {
    bool event_confirmed = false;

    switch (s_state) {

        case EMP_DETECTOR_STATE_IDLE: {
            /* Only adapt the baseline while nothing suspicious is happening -
             * this prevents the detector from "learning" a real event as
             * its new normal, which would suppress future detections. */
            update_baseline(value);

            bool amplitude_ok = amplitude_criterion_met(value, false);
            bool slope_ok      = slope_criterion_met(value);

            if (amplitude_ok && slope_ok) {
                s_confirm_counter = 1;
                s_state = EMP_DETECTOR_STATE_RISING;
            }
            break;
        }

        case EMP_DETECTOR_STATE_RISING: {
            /* Do not update the baseline while a candidate event is in
             * progress - the whole point is to compare against the
             * pre-event noise floor. */
            bool amplitude_ok = amplitude_criterion_met(value, true);

            if (amplitude_ok) {
                s_confirm_counter++;
                if (s_confirm_counter >= EMP_CONFIRM_WINDOW_SAMPLES) {
                    s_state = EMP_DETECTOR_STATE_LATCHED;
                    event_confirmed = true;
                }
            } else {
                /* Candidate event did not sustain - treat as a false
                 * alarm (e.g. an isolated noise spike) and drop back. */
                s_confirm_counter = 0;
                s_state = EMP_DETECTOR_STATE_IDLE;
            }
            break;
        }

        case EMP_DETECTOR_STATE_LATCHED: {
            /* Already reported. Waiting for emp_detector_reset_latch()
             * to be called once the protective action has been handled. */
            break;
        }

        case EMP_DETECTOR_STATE_COOLDOWN: {
            s_cooldown_counter++;
            if (s_cooldown_counter >= EMP_COOLDOWN_SAMPLES) {
                s_state = EMP_DETECTOR_STATE_IDLE;
                s_cooldown_counter = 0;
            }
            break;
        }
    }

    s_prev_value = value;
    s_have_prev_value = true;
    return event_confirmed;
}

emp_detector_state_t emp_detector_get_state(void) {
    return s_state;
}

uint16_t emp_detector_get_baseline(void) {
    return s_baseline;
}

void emp_detector_reset_latch(void) {
    if (s_state == EMP_DETECTOR_STATE_LATCHED) {
        s_state = EMP_DETECTOR_STATE_COOLDOWN;
        s_cooldown_counter = 0;
        s_confirm_counter = 0;
    }
}

bool emp_detector_self_test(void) {
    uint16_t sample = emp_detector_hal_read_adc();
    /* A minimal self-test: check that the ADC responds within a
     * plausible range (not stuck at 0 or at maximum value). */
    return (sample > 10 && sample < 4085);
}

void emp_detector_recalibrate(void) {
    /* One-shot burst average, used at startup so the EWMA baseline does
     * not begin from zero and slowly ramp up over many samples. */
    uint32_t sum = 0;
    const int samples = 32;
    for (int i = 0; i < samples; i++) {
        sum += emp_detector_hal_read_adc();
    }
    s_baseline = (uint16_t)(sum / samples);
}
