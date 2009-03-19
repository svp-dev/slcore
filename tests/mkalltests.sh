#! /bin/sh
root=$(dirname "$0")
cd "$root"
rm -f alltests.mk
exec >alltests.mk
echo 'TESTS = ' 
find . -name \*.sl | grep -v '/x_' | sed -e 's/^\.\/\(.*\)$/TESTS += \1/g'
chmod -w alltests.mk

