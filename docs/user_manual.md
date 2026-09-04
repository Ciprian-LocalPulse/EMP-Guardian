# User Manual

**Author:** Ciprian Ștefan Pleșca

## 1. Requirements

- STM32F4 development board (or compatible MCU) with the ARM GCC toolchain installed
- Programmer/debugger (ST-Link or equivalent)
- Python 3.9+ for the monitoring application
- EMP sensor wired according to the schematic in [`hardware/schematics/`](../hardware/schematics/)

## 2. Firmware installation

```bash
cd firmware
make clean && make
st-flash write build/emp_guardian.bin 0x08000000
```

## 3. Configuration

Edit [`firmware/src/config.h`](../firmware/src/config.h) to adjust:

- the detection threshold (`EMP_THRESHOLD_ADC`)
- the confirmation time-window duration
- the pins used for the actuator and communication

## 4. Starting the monitoring application

```bash
cd software/monitor
pip install -r requirements.txt
python emp_monitor.py --port /dev/ttyUSB0 --baud 115200
```

The application displays firmware-reported events in real time and saves them to a local log.

## 5. Interpreting alerts

| Message | Meaning |
|---|---|
| `EMP DETECTED` | Event confirmed, shielding activated automatically |
| `THRESHOLD_ADJUSTED` | Detection threshold was recalibrated |
| `SELF_TEST_OK` / `SELF_TEST_FAIL` | Result of the power-on self-test |

## 6. Reset after an event

After activation, the system by default waits a configurable interval (`shield_control_wait_reset`) before returning to normal state. For critical environments, manual, operator-controlled reset is recommended instead of automatic reset.

## 7. Troubleshooting

- **Cannot connect on the serial port:** check port access permissions (`sudo usermod -a -G dialout $USER` on Linux) and that the firmware was flashed successfully.
- **Frequent false positives:** increase the threshold (`EMP_THRESHOLD_ADC`) or adjust the confirmation time window; check the sensor cabling's shielding.
- **Reaction time too high:** check the ADC sampling frequency and remove blocking operations from the main loop.
