#!/usr/bin/env python3

import os
import time
import subprocess
import argparse
import json
from pathlib import Path
import sys

def load_commands(json_path):
    try:
        with open(json_path) as f:
            return [tuple(entry) for entry in json.load(f)]
    except Exception as e:
        print(f"Error loading JSON file '{json_path}': {e}")
        sys.exit(1)

def stop_services(commands, scripts_dir):
    for script, config in commands:
        time.sleep(1)
        script_path = scripts_dir / script
        if not script_path.exists():
            print(f"Script not found: {script_path}")
            continue
        print(f"Stopping: {script} {config}")
        try:
            subprocess.run(["python3", str(script_path), "stop", config], check=False)
        except Exception as e:
            print(f"Failed to stop {script} with config {config}: {e}")

def main():
    parser = argparse.ArgumentParser(description="Stop ATS components from JSON list.")
    parser.add_argument(
        "--ats",
        type=Path,
        default=Path(__file__).resolve().parent / "ats_commands.json",
        help="Path to ATS JSON config list (default: ats_commands.json in script directory)"
    )

    args = parser.parse_args()

    dats_home = os.environ.get("DATS_HOME")
    if not dats_home:
        print("Error: DATS_HOME environment variable must be set.")
        sys.exit(1)

    scripts_dir = Path(dats_home) / "scripts"

    if not args.ats.exists():
        print(f"ATS JSON file does not exist: {args.ats}")
        sys.exit(1)

    commands = load_commands(args.ats)
    stop_services(commands, scripts_dir)

if __name__ == "__main__":
    main()
