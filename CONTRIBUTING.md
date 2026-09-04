# Contributing to EMP-Guardian

Thank you for your interest in contributing. The project is maintained by Ciprian Ștefan Pleșca and is open to collaboration, under a few simple rules.

## Core principle

EMP-Guardian is a **strictly defensive** project. Every contribution must respect this principle:

- Code, schematics, or documentation describing how to generate, amplify, or direct an electromagnetic pulse as a weapon will **not** be accepted.
- Only the following are accepted: detection, measurement, shielding, mitigation, alerting, automated protective response, and technical/educational documentation.

## How to propose a change

1. Open an *issue* describing the problem or proposed improvement.
2. Fork the repository and work on a dedicated branch (`feature/<name>` or `fix/<name>`).
3. Follow the existing code style (see [`docs/architecture.md`](docs/architecture.md) for conventions).
4. Add tests for any firmware or software change, wherever possible.
5. Open a *pull request* with a clear description of the change and its motivation.

## Useful types of contributions

- Corrections and improvements to the detection algorithm.
- Support for additional microcontroller families (ESP32, RP2040, MSP430).
- Real calibration/test data (even partial).
- Documentation translations.
- Improvements to the monitoring application (UI, alerts, integrations).
- Compliance reports against civilian or military standards (no classified information).

## What is not accepted

- Any dual-use content oriented toward generating an EMP.
- Classified or export-restricted information (ITAR, EAR, EU Regulation 2021/821, etc.).
- Code without a license compatible with MIT / CERN-OHL-S.

## Code of conduct

See [`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md). In short: respect, collaboration, good faith.
