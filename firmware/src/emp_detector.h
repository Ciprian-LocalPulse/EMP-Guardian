/**
 * EMP-Guardian - Modul de detecție EMP
 * Autor: Ciprian Ștefan Pleșca
 * Licență: MIT
 */

#ifndef EMP_DETECTOR_H
#define EMP_DETECTOR_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Inițializează ADC-ul și circuitele auxiliare folosite pentru detecție.
 */
void emp_detector_init(void);

/**
 * Citește ultima valoare eșantionată de la senzor.
 */
uint16_t emp_detector_read(void);

/**
 * Evaluează dacă valoarea curentă, combinată cu istoricul recent,
 * constituie un eveniment EMP confirmat (prag + fereastră temporală + histerezis).
 */
bool emp_detector_is_emp(uint16_t value);

/**
 * Rulează un auto-test la pornire (verifică integritatea lanțului senzor -> ADC).
 * Returnează true dacă auto-testul a trecut.
 */
bool emp_detector_self_test(void);

/**
 * Recalibrează pragul de detecție pe baza zgomotului de fond măsurat.
 */
void emp_detector_recalibrate(void);

#endif /* EMP_DETECTOR_H */
