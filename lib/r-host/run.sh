#! /bin/sh
# $Id$

# Run script for native executables.

program=${1:?}
datafile=${2:?}
shift
shift
if ! test -x "$program"; then
    echo "$0: $program: not an executable" >&2
    exit 1
fi
if ! test -r "$datafile"; then
    echo "$0: $datafile: file not found or not readable" >&2
    exit 1
fi

b=$(basename "$program")
d=$(dirname "$program")
d=$(cd "$d" && pwd)

exec "$d/$b" "$datafile" "$@"
