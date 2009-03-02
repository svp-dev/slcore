#! /bin/sh
# $Id$

# Run script for native executables.

# For now, just run the command.
program=$1
shift

b=$(basename "$program")
d=$(dirname "$program")
d=$(cd "$d" && pwd)

exec "$d/$b" "$@"
