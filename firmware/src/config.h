/**
 * EMP-Guardian - Firmware configuration
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 */

#ifndef CONFIG_H
#define CONFIG_H

/* ---- Detection: adaptive baseline ---- */
/* The noise floor baseline is tracked continuously with an exponentially
 * weighted moving average (EWMA): baseline += (value - baseline) >> ALPHA_SHIFT.
 * A larger shift means slower adaptation (more stable, but slower to follow
 * legitimate slow drifts in ambient noise). Tune on the real sensor. */
#define EMP_BASELINE_ALPHA_SHIFT     4       /* alpha = 1 / 2^SHIFT, e.g. shift=4 -> alpha=1/16 */

/* ---- Detection: amplitude criterion ---- */
/* An event is only amplitude-eligible if it exceeds the *adaptive* baseline
 * by this margin - not a fixed absolute ADC value. This makes the detector
 * robust to slow changes in ambient electromagnetic noise. */
#define EMP_ADAPTIVE_MARGIN_ADC      400     /* ADC counts above baseline considered anomalous */
#define EMP_HYSTERESIS_ADC           150     /* hysteresis applied while confirming, to avoid chatter */

/* ---- Detection: rate-of-rise criterion ---- */
/* This is the key addition over a pure amplitude threshold: ordinary
 * industrial switching transients can reach a high amplitude, but rise much
 * more slowly than a genuine fast EMP-type transient (see the dB/dt coupling
 * argument in the project wiki, page "Physics and Waveforms"). Requiring a
 * minimum sample-to-sample delta rejects slow-rising events regardless of
 * their peak amplitude. */
#define EMP_SLOPE_THRESHOLD_ADC      250     /* minimum ADC-count delta between consecutive samples */

/* ---- Detection: confirmation and cooldown ---- */
#define EMP_CONFIRM_WINDOW_SAMPLES   2       /* consecutive qualifying samples required to confirm an event */
#define EMP_COOLDOWN_SAMPLES         50      /* samples to wait after a reset before the baseline resumes adapting */

/* ---- Actuator ---- */
#define SHIELD_RESET_DELAY_MS        5000    /* default wait time before automatic reset */
#define SHIELD_AUTO_RESET            0       /* 0 = manual reset required (recommended in critical environments) */

/* ---- Communication ---- */
#define UART_BAUDRATE                115200

/* ---- Power-on self-test ---- */
#define SELF_TEST_ON_BOOT            1

#endif /* CONFIG_H */
