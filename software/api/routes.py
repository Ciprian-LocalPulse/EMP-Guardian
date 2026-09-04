"""
EMP-Guardian - Rute API
Autor: Ciprian Ștefan Pleșca
Licență: MIT
"""

import json
from pathlib import Path

from flask import jsonify, request


def register_routes(app):

    @app.route("/health", methods=["GET"])
    def health():
        return jsonify({"status": "ok"})

    @app.route("/events", methods=["GET"])
    def get_events():
        log_file = Path(app.config["LOG_FILE"])
        limit = int(request.args.get("limit", 50))

        if not log_file.exists():
            return jsonify({"events": []})

        lines = log_file.read_text(encoding="utf-8").strip().splitlines()
        events = [json.loads(line) for line in lines[-limit:]]
        return jsonify({"events": events, "count": len(events)})

    @app.route("/events/alerts", methods=["GET"])
    def get_alerts():
        log_file = Path(app.config["LOG_FILE"])
        if not log_file.exists():
            return jsonify({"alerts": []})

        lines = log_file.read_text(encoding="utf-8").strip().splitlines()
        events = [json.loads(line) for line in lines]
        alerts = [e for e in events if e.get("type") == "alert"]
        return jsonify({"alerts": alerts, "count": len(alerts)})
