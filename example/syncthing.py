#!/usr/bin/env python3
"""
Spumoni command to control Syncthing
"""
import argparse
import json
import subprocess
import sys
import webbrowser


SYNCTHING_URL = "http://localhost:8384"

TOOLTIP = {
    False: "Syncthing is not running",
    True: "Syncthing is running",
}


def is_syncthing_running() -> bool:
    try:
        subprocess.run(["pidof", "syncthing"], check=True, stdout=subprocess.DEVNULL)
        return True
    except subprocess.CalledProcessError:
        return False


def do_status():
    running = is_syncthing_running()

    if running:
        actions = [{
            "id": "primary",
            "text": "Open Syncthing page",
        }, {
            "id": "stop",
            "text": "Stop Syncthing"
        }]
    else:
        actions = [{
            "id": "primary",
            "text": "Start Syncthing",
        }]

    out = {
        "iconName": "syncthing",
        "toolTipText": TOOLTIP[running],
        "refresh": 10,
        "actions": actions,
    }
    if not running:
        out["overlayIconName"] = "emblem-error"
    print(json.dumps(out))


def do_activate():
    if is_syncthing_running():
        webbrowser.open_new_tab(SYNCTHING_URL)
    else:
        subprocess.run("syncthing")


def do_stop():
    subprocess.run(["killall", "syncthing"])


def main():
    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description=__doc__)

    parser.add_argument("--activate")
    parser.add_argument("--activate-action")
    parser.add_argument("--status", action="store_true")

    args = parser.parse_args()

    if args.status:
        do_status()
    if args.activate == "primary":
        do_activate()
    if args.activate_action:
        if args.activate_action == "primary":
            do_activate()
        elif args.activate_action == "stop":
            do_stop()

    return 0


if __name__ == "__main__":
    sys.exit(main())
# vi: ts=4 sw=4 et
