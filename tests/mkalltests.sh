#! /bin/sh
root=$(dirname "$0")
cd "$root"
rm -f alltests.mk
exec >alltests.mk
echo 'TESTSUITE = ' 
find . -name \*.sl | grep -v '/x_' | sed -e 's/^\.\/\(.*\)$/TESTSUITE += \1/g'
chmod -w alltests.mk

