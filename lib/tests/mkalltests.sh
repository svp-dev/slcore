#! /bin/sh
set -e
root=$(dirname "$0")
cd "$root"
rm -f alltests.mk
exec >alltests.mk
echo 'TESTSUITE = ' 

find . -name \*.sl \
  | grep -v '/x_' \
  | sed -e 's/^\.\/\(.*\)$/TESTSUITE += \1/g' | sort

echo 'XTESTSUITE = '

find . -name \*.sl \
  | grep '/x_' \
  | sed -e 's/^\.\/\(.*\)$/XTESTSUITE += \1/g' | sort

chmod -w alltests.mk

