#! /bin/bash
set -e
echo "Generating input data for BLAS suite..."
mkdir -p data
sizes=(100 200 500 1000 2000 5000 10000 20000 50000 100000)
true >extradist.mk
for sz in "${sizes[@]}"; do
   echo "$sz 2.0 v($sz) 1 v($sz) 1" >data/cvv.d$sz
   echo "$sz v($sz) 1 v($sz) 1 2.0 2.0" >data/vvcc.d$sz
   if test $sz = ${sizes[0]}; then
     for s in cvv vvcc; do
        echo "# USE IN MAKE CHECK" >>data/$s.d$sz
     done
   fi
   echo "EXTRA_DIST += data/cvv.d$sz data/vvcc.d$sz" >>extradist.mk
done
