/**
 * EMP-Guardian - EMP detection module (v2 - dual-criterion state machine)
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * Detection logic overview
 * -------------------------
 * A genuine fast electromagnetic transient is distinguished from ordinary
 * switching noise using TWO independent criteria, both of which must be
 * satisfied before an event is confirmed:
 *
 *   1. Amplitude  - the signal exceeds an *adaptively tracked* noise
 *                    baseline by a configurable margin (not a fixed
 *                    absolute threshold).
 *   2. Rate of rise - the signal changes by more than a configurable
 *                    amount between consecutive samples. This rejects
 *                    slow-rising events (e.g. industrial load switching)
 *                    that can reach a high amplitude but never approach
 *                    the dB/dt of a genuine fast transient.
 *
 * This mirrors the physical argument that induced voltage in any coupling
 * loop is proportional to dB/dt, not to peak field amplitude alone - see
 * the project wiki, "Physics and Waveforms".
 */

#ifndef EMP_DETECTOR_H
#define EMP_DETECTOR_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    EMP_DETECTOR_STATE_IDLE = 0,      /* tracking baseline, no anomaly seen */
    EMP_DETECTOR_STATE_RISING,        /* candidate event, awaiting confirmation window */
    EMP_DETECTOR_STATE_LATCHED,       /* event confirmed and reported; awaiting external reset */
    EMP_DETECTOR_STATE_COOLDOWN       /* just reset; baseline re-stabilizing before resuming detection */
} emp_detector_state_t;

/**
 * Initializes internal state (baseline, counters, state machine).
 */
void emp_detector_init(void);

/**
 * Reads the latest value sampled from the sensor.
 */
uint16_t emp_detector_read(void);

/**
 * Feeds one new sample into the detector state machine.
 * Returns true exactly once, at the sample where an event transitions
 * from "candidate" to "confirmed" (i.e. both criteria satisfied for
 * EMP_CONFIRM_WINDOW_SAMPLES consecutive samples).
 */
bool emp_detector_is_emp(uint16_t value);

/**
 * Returns the current state machine state (useful for telemetry/logging).
 */
emp_detector_state_t emp_detector_get_state(void);

/**
 * Returns the current adaptive baseline estimate (useful for telemetry).
 */
uint16_t emp_detector_get_baseline(void);

/**
 * Call after a protective action has been reset (e.g. after
 * shield_control_wait_reset() completes) to re-arm the detector.
 * Moves the state machine from LATCHED to COOLDOWN, and then to IDLE
 * once EMP_COOLDOWN_SAMPLES have elapsed without incident.
 */
void emp_detector_reset_latch(void);

/**
 * Runs a self-test at startup (checks the integrity of the sensor -> ADC chain).
 * Returns true if the self-test passed.
 */
bool emp_detector_self_test(void);

/**
 * Performs a one-shot baseline calibration by averaging a burst of samples.
 * Intended for use at startup, before entering the main loop, so the
 * adaptive baseline does not start from zero.
 */
void emp_detector_recalibrate(void);

#endif /* EMP_DETECTOR_H */
