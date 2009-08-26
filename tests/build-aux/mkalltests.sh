#! /bin/sh
set -e
srcdir=$(pwd)
root=$(dirname "$0")
cd "$root"
rm -f alltests.mk

dst=$srcdir/0_slc/2_manyargs
echo "Generating tests in $dst..."
for ns in 1 2 3 4; do
  for ng in 1 2 10 20; do
    (cd "$dst" && python ./gencall.py $ns $ng int >int_${ns}_$ng.c)
    (cd "$dst" && python ./gencall.py $ns $ng float >fp_${ns}_$ng.c)
  done
done
dst=$srcdir/0_slc/2_manylocals
echo "Generating tests in $dst..."
for ns in 1 2 3 40; do
  (cd "$dst" && python ./genmany.py $ns 0 >many$ns.c)
  (cd "$dst" && python ./genmany.py $ns 1 >manyf$ns.c)
done

echo "Generating Make rules..."
exec >alltests.mk
echo 'TESTSUITE = ' 

(cd "$srcdir" && find . -name \*.c) \
  | grep -v '/x_' | grep -v '\.ut\.\|\.t\.\|/_' \
  | sed -e 's/^\.\/\(.*\)$/TESTSUITE += \1/g' | sort

echo 'XTESTSUITE = '

(cd "$srcdir" && find . -name \*.c) \
  | grep '/x_' | grep -v '\.ut\.\|\.t\.\|/_' \
  | sed -e 's/^\.\/\(.*\)$/XTESTSUITE += \1/g' | sort

chmod -w alltests.mk
