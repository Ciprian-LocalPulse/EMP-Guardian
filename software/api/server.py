#!/usr/bin/env python3
"""
EMP-Guardian - API server for exposing events
Author: Ciprian Ștefan Pleșca
License: MIT

Minimal (Flask) server that reads the log produced by emp_monitor.py
and exposes it through a simple REST API, useful for integration with
institutional monitoring systems (dashboard, SCADA, SIEM, etc.).
"""

import json
from pathlib import Path

from flask import Flask, jsonify, request

from routes import register_routes

app = Flask(__name__)
app.config["LOG_FILE"] = "software/monitor/emp_events.log"

register_routes(app)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8080)
