#! /bin/sh
set -e
root=$(dirname "$0")
cd "$root"
rm -f alltests.mk
for ns in 1 2 3 4; do
  for ng in 1 2 10 20; do
    (cd 2_manyglobals && python ./gencall.py $ns $ng int >int_${ns}_$ng.c)
    (cd 2_manyglobals && python ./gencall.py $ns $ng float >fp_${ns}_$ng.c)
  done
done
for ns in 1 2 3 40; do
  (cd 2_manylocals && python ./genmany.py $ns 0 >many$ns.c)
  (cd 2_manylocals && python ./genmany.py $ns 1 >manyf$ns.c)
done
exec >alltests.mk
echo 'TESTSUITE = ' 

find . -name \*.c \
  | grep -v '/x_' | grep -v '\.ut\.\|\.t\.\|/_' \
  | sed -e 's/^\.\/\(.*\)$/TESTSUITE += \1/g' | sort

echo 'XTESTSUITE = '

find . -name \*.c \
  | grep '/x_' | grep -v '\.ut\.\|\.t\.\|/_' \
  | sed -e 's/^\.\/\(.*\)$/XTESTSUITE += \1/g' | sort

chmod -w alltests.mk

