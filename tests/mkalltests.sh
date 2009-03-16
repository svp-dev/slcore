#! /bin/sh
root=$(dirname "$0")
cd "$root"
exec >alltests.mk
echo 'TESTS = ' 
find . -name \*.sl | sed -e 's/^\.\/\(.*\)$/TESTS += \1/g'


