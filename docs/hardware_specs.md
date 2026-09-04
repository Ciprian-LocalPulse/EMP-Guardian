# Specificații hardware

**Autor:** Ciprian Ștefan Pleșca

Acestea sunt specificațiile de referință pentru varianta v1 a proiectului. Sunt orientative — trebuie ajustate/validate în funcție de componentele efectiv disponibile și de mediul de operare.

## Microcontroler

- **Familie recomandată:** STM32F4 (ex. STM32F411/STM32F407), ARM Cortex-M4 @ 100 MHz
- **Alternative suportate în firmware (portabile cu efort moderat):** RP2040, ESP32-S3, MSP430FR
- **ADC:** minim 12-bit, ≥ 2 MSPS
- **Memorie nevolatilă:** pentru jurnalul de evenimente (Flash internă sau EEPROM extern)

## Senzor

- **Tip:** bobină Rogowski cu nucleu de ferită sau antenă de bandă largă (biconică/log-periodică pentru variante de laborator)
- **Bandă de frecvență:** 10 kHz – 1 GHz (obiectiv de proiectare; performanța reală depinde de senzorul folosit)
- **Circuit de condiționare:** amplificator de instrumentație + limitator (diode de protecție) + filtru anti-aliasing

## Actuator de ecranare

- **Tehnologie:** MOSFET/IGBT de comutație rapidă sau relee cu stare solidă (SSR)
- **Timp de comutare țintă:** < 5 µs
- **Curent/tensiune:** dimensionate în funcție de sarcina protejată — a se specifica per proiect

## Ecranare pasivă (recomandată ca linie de bază)

- **Material:** plasă de cupru sau tablă perforată
- **Atenuare țintă:** > 80 dB până la 10 GHz (valoare de referință pentru incinte de tip cușcă Faraday de calitate industrială — se validează prin măsurători, nu se presupune implicit)

## Alimentare

- **Tensiune principală:** 5V DC (sau conform proiectului specific)
- **Rezervă:** baterie Li-ion sau supercondensator, dimensionată pentru minim câteva secunde de funcționare autonomă

## Interfețe

- UART (debug și telemetrie de bază)
- I2C / SPI (senzori auxiliari, memorie externă)
- Ethernet (opțional, pentru integrare în rețele de monitorizare)

## Condiții de mediu

- **Temperatură de operare:** -40 °C … +85 °C (țintă industrială; a se valida pe componentele alese)
- **Umiditate:** conform mediului de instalare — a se specifica per proiect

## Notă importantă

Valorile de mai sus sunt specificații **de referință/proiectare**, nu rezultate de testare certificată. Orice implementare destinată unui mediu instituțional sau militar trebuie validată printr-un laborator acreditat, conform standardelor aplicabile (vezi `docs/compliance.md`).
