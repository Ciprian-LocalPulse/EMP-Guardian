# Threat Model

**Author:** Ciprian Ștefan Pleșca

## Purpose

This document defines which types of EMP events EMP-Guardian is designed to detect and mitigate, and what is explicitly outside the project's scope.

## In scope (defensive)

- **Naturally occurring EMP:** severe geomagnetic storms (Carrington-type events), which induce intense currents in long power transmission networks.
- **High-intensity industrial electrostatic discharges (ESD)** in industrial environments.
- **Accidental electromagnetic interference** from high-power equipment (radar, transmitters, industrial machinery).
- **Human-caused EMP events over which the operating entity has no control** — the system reacts defensively, regardless of cause.

## Explicitly out of scope

- The project **does not contain** and **will never contain** schematics, code, or instructions for generating, amplifying, or directing an electromagnetic pulse.
- The project **is not** an attack guide and must not be used as one.
- Any pull request or contribution introducing offensive functionality will be rejected.

## Actors and target use cases

| Actor | Use case |
|---|---|
| Critical infrastructure operator (power, water, telecom) | Protecting control equipment (SCADA/ICS) |
| Research institution / university | Study platform for electromagnetic hardening |
| Government / military entity | Protection of command centers, communications, sensitive equipment (per their own standards, e.g. MIL-STD-188-125) |
| Data center operator | Additional line of defense for critical equipment |
| Amateur radio operators / independent researchers | Experimentation, education, open-source contributions |

## Explicit limitations

- The system offers **additional** protection, not a 100% guarantee, especially against extreme-intensity events.
- Real-world effectiveness depends on the quality of the hardware implementation (passive shielding, sensor quality, experimentally validated reaction time).
- [`docs/compliance.md`](compliance.md) details the legal obligations associated with using and distributing this project.
