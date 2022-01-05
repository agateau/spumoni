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
    cat <<EOF
{
    "iconName": "kde",
    "toolTipText": "Example",
    "refresh": 0
}
EOF
}

action="USAGE"
while [ $# -gt 0 ] ; do
    case "$1" in
    -h|--help)
        ;;
    --status)
        action="STATUS"
        ;;
    *)
        usage "Unknown option '$1'"
        ;;
    esac
    shift
done

case "$action" in
USAGE)
    usage
    ;;
STATUS)
    status
    ;;
esac
