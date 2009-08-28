#! /bin/bash
slr=${SLR:-slr}
prog=${1:?program not specified}
nrcpus=${2:?max nr of cpus not specified}
maxsize=${3:?max problem size not specified}

printf '# cores'
for ((m=1; m<$maxsize; ++m)); do
   printf '\tM=%s' $m
done
echo
for ((ncpu=1; ncpu<$nrcpus; ncpu*=2)); do
   printf '%s' $ncpu
   for ((m=1; m<$maxsize; ++m)); do
       res=$("$slr" -n $ncpu "$prog" N=$m 2>&1 | grep '^[0-9]' |cut -d' ' -f1)
       if test -z "$res"; then
	   res=XXX
       fi
       printf '\t%s' "$res"
   done

   echo
done


