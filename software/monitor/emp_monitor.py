#!/usr/bin/env python3
"""
EMP-Guardian - Aplicație de monitorizare
Autor: Ciprian Ștefan Pleșca
Licență: MIT

Citește evenimente de pe portul serial expus de firmware și le
jurnalizează local. Poate fi extinsă ușor cu notificări (email, SMS,
webhook) în funcție de mediul de operare.
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
        log.warning("Fișierul de configurare %s nu există, se folosesc valori implicite.", path)
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
    parser.add_argument("--port", required=True, help="Port serial (ex: /dev/ttyUSB0 sau COM3)")
    parser.add_argument("--baud", type=int, default=115200, help="Baud rate")
    parser.add_argument("--config", default="config.yaml", help="Fișier de configurare")
    parser.add_argument("--log-file", default="emp_events.log", help="Fișier de jurnalizare a evenimentelor")
    args = parser.parse_args()

    config = load_config(args.config)
    log_file = config.get("log_file", args.log_file)

    try:
        ser = serial.Serial(args.port, args.baud, timeout=1)
        log.info("Conectat la %s (%d baud)", args.port, args.baud)
    except serial.SerialException as e:
        log.error("Eroare la deschiderea portului serial: %s", e)
        return

    log.info("Monitorizare pornită. Ctrl+C pentru oprire.")
    try:
        while True:
            raw_line = ser.readline().decode("utf-8", errors="replace").strip()
            if not raw_line:
                continue

            event = parse_line(raw_line)
            append_to_log(log_file, event)

            if event["type"] == "alert":
                log.warning("ALERTĂ: %s", event.get("message", raw_line))
            elif event["type"] == "status":
                log.info("Stare: %s = %s", event.get("key"), event.get("value"))
            else:
                log.debug("Mesaj neinterpretat: %s", raw_line)

            time.sleep(0.01)
    except KeyboardInterrupt:
        log.info("Monitorizare oprită de utilizator.")
    finally:
        ser.close()


if __name__ == "__main__":
    main()
