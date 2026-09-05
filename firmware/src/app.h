/**
 * EMP-Guardian - Portable application loop
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 *
 * This is the ONE piece of logic every board's main entry point calls.
 * It contains no board-specific code (that's the HAL's job - see
 * include/hal.h), so the exact same sequence runs on STM32, ESP32,
 * RP2040, or a future port.
 *
 * Typical board main():
 *
 *     int main(void) {
 *         emp_guardian_app_init();
 *         while (1) {
 *             emp_guardian_app_step();
 *             // board-appropriate pacing: busy-wait, sleep_ms(), a
 *             // FreeRTOS tick delay, or (in a production build) simply
 *             // nothing, if step() is instead called from a timer ISR.
 *         }
 *     }
 */

#ifndef EMP_APP_H
#define EMP_APP_H

/**
 * Runs emp_hal_board_init() followed by the one-time init sequence for
 * every core module (detector, shield, comms), the optional
 * power-on self-test, and the initial baseline calibration. Call
 * exactly once, before the first emp_guardian_app_step().
 */
void emp_guardian_app_init(void);

/**
 * Runs exactly one detection cycle: reads a sample, feeds it to the
 * detector state machine, and if an event is confirmed, activates the
 * shield, reports it, waits for reset, and re-arms the detector.
 * Call repeatedly - from a superloop, an RTOS task, or a timer ISR.
 */
void emp_guardian_app_step(void);

#endif /* EMP_APP_H */
