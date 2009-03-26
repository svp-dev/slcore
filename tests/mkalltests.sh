#! /bin/sh
set -e
root=$(dirname "$0")
cd "$root"
rm -f alltests.mk
for ns in 1 2 3 4; do
  for ng in 1 2 10 20; do
    (cd 1_*/3_* && python ./gencall.py $ns $ng int >int_${ns}_$ng.sl)
    (cd 1_*/3_* && python ./gencall.py $ns $ng float >fp_${ns}_$ng.sl)
  done
done
exec >alltests.mk
echo 'TESTSUITE = ' 
find . -name \*.sl | grep -v '/x_' | sed -e 's/^\.\/\(.*\)$/TESTSUITE += \1/g'

chmod -w alltests.mk

