# Schematics

**Author:** Ciprian Ștefan Pleșca
**License:** CERN-OHL-S v2 (see `../../LICENSE-HARDWARE`)

This directory is intended for KiCad source files (`.kicad_sch`) and PDF exports of the electrical schematics for:

- the sensor signal-conditioning circuit (sensor → limiter → ADC)
- the shielding actuator drive circuit
- the power supply circuit (main + backup)

## Status

The reference schematic (`emp_shield_v1`) is in progress. The functional block structure is described in [`docs/architecture.md`](../../docs/architecture.md) and can be used as a starting point for drawing the complete schematic in KiCad or another open-source EDA tool.

## Contributions

Contributions with complete, electrically verified (ERC) KiCad schematics are welcome — see [`CONTRIBUTING.md`](../../CONTRIBUTING.md) in the project root.
