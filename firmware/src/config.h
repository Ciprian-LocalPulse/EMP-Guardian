/**
 * EMP-Guardian - Configurare firmware
 * Autor: Ciprian Ștefan Pleșca
 * Licență: MIT
 */

#ifndef CONFIG_H
#define CONFIG_H

/* ---- Detecție ---- */
#define EMP_THRESHOLD_ADC        3000    /* prag orientativ, se calibrează pe senzorul real */
#define EMP_CONFIRM_WINDOW_US    2       /* durata minimă (us) cât semnalul trebuie să rămână peste prag */
#define EMP_HYSTERESIS_ADC       150     /* histerezis pentru a evita oscilația în jurul pragului */

/* ---- Actuator ---- */
#define SHIELD_RESET_DELAY_MS    5000    /* timp implicit de așteptare înainte de reset automat */
#define SHIELD_AUTO_RESET        0       /* 0 = necesită reset manual (recomandat în medii critice) */

/* ---- Comunicație ---- */
#define UART_BAUDRATE             115200

/* ---- Auto-test la pornire ---- */
#define SELF_TEST_ON_BOOT         1

#endif /* CONFIG_H */
