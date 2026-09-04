#!/usr/bin/env python3
"""
EMP-Guardian - Monitoring application
Author: Ciprian Ștefan Pleșca
License: MIT

Reads events from the serial port exposed by the firmware and logs
them locally. Can easily be extended with notifications (email, SMS,
webhook) depending on the operating environment.
"""

import argparse
import json
import logging
import time
from datetime import datetime, timezone
from pathlib import Path

import serial
import yaml

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s",
)
log = logging.getLogger("emp_monitor")


def load_config(path: str) -> dict:
    config_path = Path(path)
    if not config_path.exists():
        log.warning("Config file %s does not exist, using default values.", path)
        return {}
    with config_path.open("r", encoding="utf-8") as f:
        return yaml.safe_load(f) or {}


def append_to_log(log_file: str, event: dict) -> None:
    with open(log_file, "a", encoding="utf-8") as f:
        f.write(json.dumps(event, ensure_ascii=False) + "\n")


def parse_line(line: str) -> dict:
    event = {
        "timestamp": datetime.now(timezone.utc).isoformat(),
        "raw": line,
    }
    if line.startswith("ALERT:"):
        event["type"] = "alert"
        event["message"] = line[len("ALERT:"):]
    elif line.startswith("STATUS:"):
        event["type"] = "status"
        payload = line[len("STATUS:"):]
        if "=" in payload:
            key, value = payload.split("=", 1)
            event["key"] = key
            event["value"] = value
    else:
        event["type"] = "unknown"
    return event


def main() -> None:
    parser = argparse.ArgumentParser(description="EMP-Guardian Monitor")
    parser.add_argument("--port", required=True, help="Serial port (e.g. /dev/ttyUSB0 or COM3)")
    parser.add_argument("--baud", type=int, default=115200, help="Baud rate")
    parser.add_argument("--config", default="config.yaml", help="Configuration file")
    parser.add_argument("--log-file", default="emp_events.log", help="Event log file")
    args = parser.parse_args()

    config = load_config(args.config)
    log_file = config.get("log_file", args.log_file)

    try:
        ser = serial.Serial(args.port, args.baud, timeout=1)
        log.info("Connected to %s (%d baud)", args.port, args.baud)
    except serial.SerialException as e:
        log.error("Error opening serial port: %s", e)
        return

    log.info("Monitoring started. Ctrl+C to stop.")
    try:
        while True:
            raw_line = ser.readline().decode("utf-8", errors="replace").strip()
            if not raw_line:
                continue

            event = parse_line(raw_line)
            append_to_log(log_file, event)

            if event["type"] == "alert":
                log.warning("ALERT: %s", event.get("message", raw_line))
            elif event["type"] == "status":
                log.info("Status: %s = %s", event.get("key"), event.get("value"))
            else:
                log.debug("Unrecognized message: %s", raw_line)

            time.sleep(0.01)
    except KeyboardInterrupt:
        log.info("Monitoring stopped by user.")
    finally:
        ser.close()


if __name__ == "__main__":
    main()
