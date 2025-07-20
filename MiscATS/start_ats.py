#!/usr/bin/env python3

import subprocess
import time
import signal
import sys
import os
import json
import argparse
from pathlib import Path

# --- Environment Setup ---
dats_home = os.environ.get("DATS_HOME")
basedir_ats = os.environ.get("BASEDIR_ATS")

dats_env = os.environ.copy()

# --- Globals ---
stop_script = None
scripts_dir = None
json_path_ats = None
commands = []

# --- Cleanup Handler ---
def cleanup(signum=None, frame=None):
    print("\nCaught signal... cleaning up.")
    try:
        subprocess.run(["python3", str(stop_script), '--ats', str(json_path_ats)], check=False)
    except Exception as e:
        print(f"Error running stop script: {e}")
    print("Done.")
    sys.exit(1)

# --- Signal Registration ---
def register_signals():
    for sig in [signal.SIGINT, signal.SIGHUP, signal.SIGQUIT, signal.SIGABRT]:
        signal.signal(sig, cleanup)

# --- Load ATS Command List from JSON ---
def load_commands():
    try:
        with open(json_path_ats) as f:
            return [tuple(entry) for entry in json.load(f)]
    except Exception as e:
        print(f"Failed to load command list from {json_path_ats}: {e}")
        sys.exit(1)

# --- Start and Monitor Processes ---
def run_services():

    env_file = Path(dats_home) / "dats_env.sh"
    scripts_dir = Path(dats_home) / "scripts"


    for script, cfg in commands:
        time.sleep(1)
        print(f"Starting {script} {cfg}")
        dats_env["LOG_FILE_NAME"] = f"{cfg}.log"
        subprocess.Popen(
            f"source {env_file} && python3 {scripts_dir / script} start {cfg}",
            shell=True,
            executable="/bin/bash",
            env=dats_env
        )

    try:
        while True:
            for script, cfg in commands:
                subprocess.run([str(scripts_dir / script), "check", cfg], check=False)
            time.sleep(2)
    except KeyboardInterrupt:
        cleanup()

# --- Main ---
def main():
    global stop_script, scripts_dir, json_path_ats, commands

    parser = argparse.ArgumentParser(description="Start and monitor ATS components.")
    parser.add_argument(
        "--ats",
        type=Path,
        default=Path(__file__).resolve().parent / "ats.json",
        help="Path to ATS command JSON file (default: ats.json in script directory)"
    )

    args = parser.parse_args()

    # Validate env
    dats_home = os.environ.get("DATS_HOME")
    basedir_ats = os.environ.get("BASEDIR_ATS")

    if not dats_home or not basedir_ats:
        print("Error: DATS_HOME and BASEDIR_ATS environment variables must be set.")
        sys.exit(1)

    log_dir = Path(basedir_ats) / "logs"
    log_dir.mkdir(exist_ok=True)
    dats_env["DATS_LOG_HOME"] = str(log_dir)

    stop_script = "stop_ats.py"

    if not args.ats.exists():
        print(f"ATS JSON file does not exist: {args.ats}")
        sys.exit(1)

    json_path_ats = args.ats
    commands = load_commands()
    register_signals()
    run_services()

if __name__ == "__main__":
    main()
