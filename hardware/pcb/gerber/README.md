# PCB Files (Gerber)

**Author:** Ciprian Ștefan Pleșca
**License:** CERN-OHL-S v2 (see `../../../LICENSE-HARDWARE`)

This directory will contain the Gerber files and the KiCad PCB project for the EMP-Guardian base board, once the schematic in [`hardware/schematics/`](../../schematics/) is finalized and verified.

## Naming convention

- `emp_guardian_v1_gerber.zip` — complete Gerber package (RS-274X) for manufacturing
- `emp_guardian_v1.kicad_pcb` — KiCad PCB source file

## Manufacturing notes

- A minimum of 4 layers is recommended for proper isolation of the ground plane from fast-signal traces.
- Traces between the sensor and the conditioning circuit must be kept as short as possible, to minimize parasitic inductance.
