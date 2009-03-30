#! /bin/bash
sgr=${SGR:-sgr}
prog=${1:?program not specified}
nrcpus=${2:?max nr of cpus not specified}
maxsize=${3:?max problem size not specified}

printf '# cores'
for ((m=2; m<$maxsize; ++m)); do
   printf '\tM=%s' $m
done
echo
for ((ncpu=1; ncpu<$nrcpus; ++ncpu)); do
   printf '%s' $ncpu
   for ((m=2; m<$maxsize; ++m)); do
       res=$("$sgr" -Ws,-o -Ws,NumProcessors=$ncpu "$prog" M=$m Pi= Pf= 2>&1 | cut -d';' -f1)
       if expr "$res" : "Deadlock" >/dev/null; then
	   res=XXX
       fi
       printf '\t%s' "$res"
   done

   echo
done


