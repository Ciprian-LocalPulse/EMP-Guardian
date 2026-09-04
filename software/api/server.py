#!/usr/bin/env python3
"""
EMP-Guardian - Server API pentru expunerea evenimentelor
Autor: Ciprian Ștefan Pleșca
Licență: MIT

Server minimal (Flask) care citește jurnalul produs de emp_monitor.py
și îl expune printr-un API REST simplu, util pentru integrare cu
sisteme de monitorizare instituționale (dashboard, SCADA, SIEM etc.).
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
