#!/bin/bash
set -euo pipefail

PROGNAME=$(basename $0)

die() {
    echo "$PROGNAME: $*" >&2
    exit 1
}

usage() {
    if [ "$*" != "" ] ; then
        echo "Error: $*"
        echo
    fi

    cat << EOF
Usage: spumoni $PROGNAME [OPTION ...]
An example Spumoni command.
EOF

    exit 1
}

status() {
    local icons=(face-angel face-clown face-in-love face-laughing)
    local icon_count=${#icons[@]}
    local icon=${icons[RANDOM % icon_count]}
    cat <<EOF
{
    "iconName": "$icon",
    "toolTipText": "Example",
    "refresh": 0.2,
    "actions": [
        {
            "id": "hello",
            "iconName": "face-angel",
            "text": "Hello"
        },
        {
            "id": "world",
            "text": "World"
        }
    ]
}
EOF
}

activate() {
    local reason=$1
    kdialog --msgbox $reason
}

activate_action() {
    local id=$1
    kdialog --msgbox "Action $id"
}

while [ $# -gt 0 ] ; do
    case "$1" in
    -h|--help)
        ;;
    --status)
        status
        exit
        ;;
    --activate)
        activate "$2"
        exit
        ;;
    --activate-action)
        activate_action "$2"
        exit
        ;;
    *)
        usage "Unknown option '$1'"
        ;;
    esac
    shift
done
