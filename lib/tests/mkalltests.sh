#! /bin/sh
set -e
root=$(dirname "$0")
cd "$root"
rm -f alltests.mk
exec >alltests.mk
echo 'TESTSUITE = ' 

find . -name \*.c \
  | grep -v '/x_' | grep -v '\.ut\.\|\.t\.' \
  | sed -e 's/^\.\/\(.*\)$/TESTSUITE += \1/g' | sort

echo 'XTESTSUITE = '

find . -name \*.c \
  | grep '/x_' | grep -v '\.ut\.\|\.t\.' \
  | sed -e 's/^\.\/\(.*\)$/XTESTSUITE += \1/g' | sort

chmod -w alltests.mk
