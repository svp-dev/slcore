#! /bin/bash
sgr=${SGR:-sgr}
prog=${1:?program not specified}
data=${2:?data file containing numbers}
nrcpus=${3:?max nr of cpus not specified}
maxsize=${4:?max problem size not specified}

printf '# cores'
for ((m=40; m<$maxsize; m++)); do
   printf '\tM=%s' $m
done
echo

for ((ncpu=1; ncpu<$nrcpus; ncpu*=2)); do
   printf '%s' $ncpu
   for ((m=40; m<$maxsize; m ++)); do
       head -n$m <"$data" >rdata.tmp
       res=$("$sgr" -Ws,-q -Ws,-o -Ws,NumProcessors=$ncpu "$prog" data=file:rdata.tmp Pc=1 Pr= 2>&1 | tail -n1)
       if expr "$res" : "Deadlock" >/dev/null; then
	   res=XXX
       fi
       printf '\t%s' "$res"
   done
   echo
done


