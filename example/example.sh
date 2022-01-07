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
Usage: $PROGNAME [OPTION ...] [foo] [bar]
<Program description>.

EOF

    exit 1
}

status() {
    local icons=(kde document-open gwenview kwrite)
    local icon_count=${#icons[@]}
    local icon=${icons[RANDOM % icon_count]}
    cat <<EOF
{
    "iconName": "$icon",
    "toolTipText": "Example",
    "refresh": 100
}
EOF
}

activate() {
    local reason=$1
    kdialog --msgbox $reason
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
        ;;
    *)
        usage "Unknown option '$1'"
        ;;
    esac
    shift
done
