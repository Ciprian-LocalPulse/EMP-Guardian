# Changelog

All notable changes to this project are documented in this file.

The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project follows [Semantic Versioning](https://semver.org/) once a
first tagged release exists.

## [Unreleased]

### Added
- `firmware/include/hal.h` — a single hardware-abstraction contract
  (`emp_hal_board_init`, `emp_detector_hal_read_adc`, `emp_hal_gpio_write`,
  `emp_hal_delay_ms`, `emp_hal_uart_init`, `emp_hal_uart_write`) that every
  board port now implements against.
- `firmware/src/app.c` / `app.h` — the portable init/step application loop,
  identical across every board. `firmware/src/main.c` is now board-agnostic
  and simply drives this loop.
- `firmware/boards/stm32/hal_stm32.c`, `firmware/boards/rp2040/hal_rp2040.c`,
  and `firmware/boards/esp32/hal_esp32.c` — reference HAL ports for STM32
  (STM32Cube HAL), RP2040 (Pico SDK), and ESP32 (ESP-IDF).
- `software/calculator/emp_calculator.py` — a dependency-free engineering
  calculator (skin depth, shielding effectiveness across multiple named
  materials, Rogowski coil sensitivity, latency budget, E1-style waveform
  modeling), unit-tested against known reference values.
- `assets/system_block_diagram.svg`, `assets/enclosure_cross_section.svg`,
  `assets/rogowski_sensor_mount.svg` — technical reference diagrams,
  cross-linked from the relevant docs and from the calculator's functions.
- `docs/WHITEPAPER.md` — a publication-style paper documenting the system's
  design rationale, governing physics, and detection theory, with an
  explicit distinction between what is implemented/verified and what
  remains a design assumption.
- `docs/future_work.md` — tracks research directions (ML-based classifiers,
  fleet-level correlation, the SE "B" correction term) explicitly marked
  as not implemented, so scope discussions don't inflate what the current
  firmware actually does.
- `firmware/tests/hal_mock.c` — a shared HAL mock for host-side unit tests.
- `firmware/tests/test_shield_control.c` and `firmware/tests/test_app.c` —
  new unit/integration tests covering the shielding actuator and the full
  init/step orchestration (detector → shield → comms).
- `firmware/Makefile` — reworked to build per-board (`make BOARD=stm32`,
  `make BOARD=rp2040`) and to run the full host test suite (`make test`).
- `CHANGELOG.md`, `SUPPORT.md`, `.gitignore`, `.editorconfig`,
  `.github/ISSUE_TEMPLATE/`, `.github/PULL_REQUEST_TEMPLATE.md`,
  `.github/workflows/ci.yml`.

### Changed
- `firmware/src/emp_detector.c`, `firmware/src/shield_control.c`,
  `firmware/src/comms.c` — now call exclusively through `hal.h` instead of
  ad-hoc `extern` declarations.
- `README.md` — repository structure and quick-start sections updated to
  reflect `firmware/boards/`, `firmware/include/`, and the new test targets.

### Fixed
- The repository structure diagram in `README.md` referenced
  `.github/workflows/ci.yml` before that file actually existed; it now does.

## [0.1.0] — initial public release

- Initial detection firmware (dual-criterion state machine: adaptive
  amplitude threshold + rate-of-rise), shielding actuator control, and
  UART event reporting.
- Monitoring application and REST API (`software/monitor`, `software/api`).
- Hardware documentation: schematics/PCB/enclosure placeholders, BOM.
- SPICE simulation of a representative EMP pulse
  (`simulation/spice/emp_pulse_sim.sp`).
- Project documentation: architecture, theory of operation, hardware specs,
  threat model, test procedures, compliance notes, user manual.
