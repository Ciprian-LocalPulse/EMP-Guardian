/**
 * EMP-Guardian - EMP detection module
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 */

#ifndef EMP_DETECTOR_H
#define EMP_DETECTOR_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Initializes the ADC and auxiliary circuits used for detection.
 */
void emp_detector_init(void);

/**
 * Reads the latest value sampled from the sensor.
 */
uint16_t emp_detector_read(void);

/**
 * Evaluates whether the current value, combined with recent history,
 * constitutes a confirmed EMP event (threshold + time window + hysteresis).
 */
bool emp_detector_is_emp(uint16_t value);

/**
 * Runs a self-test at startup (checks the integrity of the sensor -> ADC chain).
 * Returns true if the self-test passed.
 */
bool emp_detector_self_test(void);

/**
 * Recalibrates the detection threshold based on the measured background noise.
 */
void emp_detector_recalibrate(void);

#endif /* EMP_DETECTOR_H */
