#! /bin/sh
# $Id$

# Run script for native executables.
set -eu
basename=${0##*/}
program=${1:?}
datafile=${2:?}
fdatafile=${3:?}
DEBUGGER=${DEBUGGER:-gdb --args}
shift
shift
shift
if ! test -x "$program"; then
    echo "$basename: $program: not an executable" >&2
    exit 1
fi
if ! test -r "$datafile"; then
    echo "$basename: $datafile: file not found or not readable" >&2
    exit 1
fi
if ! test -r "$fdatafile"; then
    echo "$basename: $fdatafile: file not found or not readable" >&2
    exit 1
fi

b=$(basename "$program")
d=$(dirname "$program")
d=$(cd "$d" && pwd)

do_debug=
if test -n "${DEBUG:-}"; then
  do_debug=$DEBUGGER
fi
if test -n "${VERBOSE:-}"; then
  echo "$basename: running: ${RUNNER_PREFIX:-} $do_debug $d/$b $datafile $fdatafile ${*:-}" >&2
fi
exec ${RUNNER_PREFIX:-} $do_debug "$d/$b" "$datafile" "$fdatafile" ${1:+"$@"}
