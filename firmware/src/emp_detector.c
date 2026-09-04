/**
 * EMP-Guardian - Modul de detecție EMP
 * Autor: Ciprian Ștefan Pleșca
 * Licență: MIT
 *
 * Implementare portabilă a algoritmului de detecție. Funcțiile de acces
 * hardware (ADC) sunt izolate în emp_detector_hal_read_adc() astfel încât
 * acest fișier să poată fi testat unitar fără hardware real (vezi firmware/tests/).
 */

#include "emp_detector.h"
#include "config.h"

/* Declarată în stratul HAL specific plăcii (nu inclusă în acest repo generic) */
extern uint16_t emp_detector_hal_read_adc(void);

static uint16_t s_baseline_noise = 0;
static uint8_t  s_above_threshold_counter = 0;
static bool     s_latched = false;

void emp_detector_init(void) {
    s_baseline_noise = 0;
    s_above_threshold_counter = 0;
    s_latched = false;
    /* Inițializarea periferică reală (ADC, DMA, timere) se face în HAL-ul plăcii. */
}

uint16_t emp_detector_read(void) {
    return emp_detector_hal_read_adc();
}

bool emp_detector_is_emp(uint16_t value) {
    uint16_t effective_threshold = EMP_THRESHOLD_ADC;

    /* Histerezis: dacă suntem deja "latched", coborâm pragul de ieșire,
     * nu pragul de intrare, ca să evităm oscilația rapidă. */
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

    /* Fereastra temporală de confirmare: cerem ca semnalul să rămână
     * peste prag pentru un număr minim de eșantioane consecutive,
     * pentru a filtra vârfuri izolate de zgomot. */
    if (s_above_threshold_counter >= EMP_CONFIRM_WINDOW_US) {
        s_latched = true;
        return true;
    }

    return false;
}

bool emp_detector_self_test(void) {
    uint16_t sample = emp_detector_hal_read_adc();
    /* Un auto-test minimal: verificăm că ADC-ul răspunde într-un
     * interval plauzibil (nu blocat la 0 sau la valoare maximă). */
    return (sample > 10 && sample < 4085);
}

void emp_detector_recalibrate(void) {
    /* Recalibrare simplă bazată pe medie mobilă a zgomotului de fond.
     * Implementarea completă (filtru IIR/medie glisantă) se adaptează
     * la caracteristicile reale ale senzorului folosit. */
    uint32_t sum = 0;
    const int samples = 32;
    for (int i = 0; i < samples; i++) {
        sum += emp_detector_hal_read_adc();
    }
    s_baseline_noise = (uint16_t)(sum / samples);
}
