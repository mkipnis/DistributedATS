#!/usr/bin/env python3

import os
import sys
import time
import subprocess
from pathlib import Path
import psutil

PROGNAME = "DataService"


def get_config_file():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} [start|stop|check] <config_file>")
        sys.exit(1)
    return sys.argv[2]


def get_paths(config_file):
    basedir = os.environ.get("BASEDIR_ATS", ".")
    dats_home = os.environ.get("DATS_HOME")
    if not dats_home:
        print("DATS_HOME is not set.")
        sys.exit(1)

    config_path = Path(basedir) / "config" / config_file
    log_path = Path(basedir) / "logs" / f"{PROGNAME}.{config_file}.console.log"
    binary_path = Path(dats_home) / "bin" / PROGNAME
    return binary_path, config_path, log_path, basedir


def source_dats_env():
    dats_home = os.environ.get("DATS_HOME")
    env_script = Path(dats_home) / "dats_env.sh"
    if env_script.exists():
        subprocess.call(f"source {env_script}", shell=True, executable="/bin/bash")


def check_process(config_path):
    for proc in psutil.process_iter(['pid', 'cmdline']):
        try:
            cmdline = proc.info['cmdline']
            if cmdline and PROGNAME in cmdline[0] and str(config_path) in ' '.join(cmdline):
                print(f"{PROGNAME} [{config_path.name}] is running - {proc.pid}")
                return proc.pid
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            continue
    print(f"{PROGNAME} [{config_path.name}] is not running")
    return None


def start_process(binary_path, config_path, log_path):
    print(f"Starting: {PROGNAME} -c {config_path}")
    for key in sorted(os.environ):
        print(f"{key}={os.environ[key]}")
    if check_process(config_path) is None:
        with open(log_path, "a") as f:
            subprocess.Popen([str(binary_path), "-c", str(config_path)],
                             stdout=f, stderr=subprocess.STDOUT)
        time.sleep(1)
        check_process(config_path)


def stop_process(config_path):
    cmd_str = f"{PROGNAME} -c {config_path}"
    print(f"Stopping: {cmd_str}")
    pid = check_process(config_path)
    if pid:
        os.system(f"pkill -SIGTERM -f \"{cmd_str}\"")
        time.sleep(1)

        for _ in range(10):
            if check_process(config_path) is None:
                break
            time.sleep(1)
        else:
            os.system(f"pkill -KILL -U {os.getuid()} -f \"{cmd_str}\"")


def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} [start|stop|check] <config_file>")
        sys.exit(1)

    command = sys.argv[1].lower()
    config_file = get_config_file()

    binary_path, config_path, log_path, basedir = get_paths(config_file)
    source_dats_env()

    if command == "start":
        start_process(binary_path, config_path, log_path)
    elif command == "stop":
        stop_process(config_path)
    elif command == "check":
        check_process(config_path)
    else:
        print("Unknown command")
        sys.exit(1)

    with open(log_path, "a") as f:
        f.write(f"{time.strftime('%Y%m%d.%H%M%S')} run-done : pid,{os.getpid()}\n")


if __name__ == "__main__":
    main()
