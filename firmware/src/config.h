/**
 * EMP-Guardian - Firmware configuration
 * Author: Ciprian Ștefan Pleșca
 * License: MIT
 */

#ifndef CONFIG_H
#define CONFIG_H

/* ---- Detection ---- */
#define EMP_THRESHOLD_ADC        3000    /* indicative threshold, calibrate on the real sensor */
#define EMP_CONFIRM_WINDOW_US    2       /* minimum duration (us) the signal must stay above threshold */
#define EMP_HYSTERESIS_ADC       150     /* hysteresis to avoid oscillation around the threshold */

/* ---- Actuator ---- */
#define SHIELD_RESET_DELAY_MS    5000    /* default wait time before automatic reset */
#define SHIELD_AUTO_RESET        0       /* 0 = manual reset required (recommended in critical environments) */

/* ---- Communication ---- */
#define UART_BAUDRATE             115200

/* ---- Power-on self-test ---- */
#define SELF_TEST_ON_BOOT         1

#endif /* CONFIG_H */
