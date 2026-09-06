# Hardware Specifications

**Author:** Ciprian Ștefan Pleșca

These are the reference specifications for the v1 variant of the project. They are indicative — they must be adjusted/validated based on the components actually available and the operating environment.

## Microcontroller

- **Recommended family:** STM32F4 (e.g., STM32F411/STM32F407), ARM Cortex-M4 @ 100 MHz
- **Alternatives supported in firmware (portable with moderate effort):** RP2040, ESP32-S3, MSP430FR
- **ADC:** minimum 12-bit, ≥ 2 MSPS
- **Non-volatile memory:** for the event log (internal Flash or external EEPROM)

## Sensor

- **Type:** Rogowski coil with ferrite core, or broadband antenna (biconical/log-periodic for lab variants)
- **Frequency band:** 10 kHz – 1 GHz (design target; actual performance depends on the sensor used)
- **Conditioning circuit:** instrumentation amplifier + limiter (protection diodes) + anti-aliasing filter

See [`assets/rogowski_sensor_mount.svg`](../assets/rogowski_sensor_mount.svg) for the mounting geometry and the mutual-inductance formula, cross-checked against `software/calculator/emp_calculator.py`'s `rogowski_sensitivity()`.

## Shielding actuator

- **Technology:** fast-switching MOSFET/IGBT or solid-state relays (SSR)
- **Target switching time:** < 5 µs
- **Current/voltage:** sized according to the protected load — specify per project

## Passive shielding (recommended as baseline)

- **Material:** copper mesh or perforated sheet metal
- **Target attenuation:** > 80 dB up to 10 GHz (reference value for an industrial-grade Faraday-cage enclosure — validated through measurement, never assumed)

See [`assets/enclosure_cross_section.svg`](../assets/enclosure_cross_section.svg) for the wall build-up and seam/gasket detail — the seam, not the flat panel, is typically the weak point in a real enclosure.

## Power supply

- **Main voltage:** 5V DC (or per specific project)
- **Backup:** Li-ion battery or supercapacitor, sized for at least a few seconds of autonomous operation

## Interfaces

- UART (debug and basic telemetry)
- I2C / SPI (auxiliary sensors, external memory)
- Ethernet (optional, for integration into monitoring networks)

## Environmental conditions

- **Operating temperature:** -40 °C … +85 °C (industrial target; validate against chosen components)
- **Humidity:** per installation environment — specify per project

## Important note

The values above are **reference/design** specifications, not certified test results. Any implementation intended for an institutional or military environment must be validated by an accredited laboratory, per applicable standards (see [`docs/compliance.md`](compliance.md)).
