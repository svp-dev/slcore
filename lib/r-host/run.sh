#! /bin/sh
# $Id$

# Run script for native executables.

program=${1:?}
datafile=${2:?}
DEBUGGER=${DEBUGGER:-gdb --args}
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

do_debug=
if test -n "$DEBUG"; then
  do_debug=$DEBUGGER
fi
if test -n "$VERBOSE"; then
  echo "$0: running: $do_debug $d/$b $datafile $*" >&2
fi
exec $do_debug "$d/$b" "$datafile" "$@"
