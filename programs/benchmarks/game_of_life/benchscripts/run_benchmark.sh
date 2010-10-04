#! /bin/bash

BENCHMARK=$1
DIRC=../result/
CONFIG=$3
ITER=$2
P=1

for(( CORE=1; CORE<=64; CORE<<=1 ))
do 
  for(( BLOCK=1; BLOCK<=32; BLOCK<<=1 ))                                                                                                                     
	do
	  for(( CHUNK=10; CHUNK<=100; CHUNK+=10 ))
	  do 
	  	if [ $CORE -eq 1 ]
	  	then
	  		MODE=1
	  	else
	  	  MODE=2
	  	fi
	  	if [ ! -d ${DIRC}${BENCHMARK} ]
	  	then 
	  		mkdir ${DIRC}${BENCHMARK}
	  	fi
			FFILE="${DIRC}${BENCHMARK}/${BENCHMARK}_${CORE}_${BLOCK}_${CHUNK}_${ITER}_${MODE}_FULL"
			PFILE="${DIRC}${BENCHMARK}/${BENCHMARK}_${CORE}_${BLOCK}_${CHUNK}_${ITER}_${MODE}_PART"
			if [ -f $FFILE ] && [ -f $PFILE ]
			then
				continue
			fi
			slr -X -Xr 100 -m ${CONFIG} ../life chunk=${CHUNK} cores=${CORE} block=${BLOCK} mode=${MODE} iter=${ITER} -f ${BENCHMARK} -n ${P} > ${FFILE} 2>&1   
			grep '^[0-9]' < ${FFILE} | gawk -v line=$CORE '{
																	if( FNR == 1)
																			printf "%-10s\n", "cycle"
																	if( FNR == 4)
																			printf "\n%-15s %-15s %-5s %-5s %-5s \n\n" , "PID/PNum.","InsTotal","Peff","Tocc","Focc"
																	if( FNR == line + 7 )
																			printf "\n%-10s\n\n" , "total"
																	if( FNR == line + 8 )
																	 		printf "\n%-10s\n\n" , "average"
																	if( FNR == 1 || $2 > 0 )
																			printf "%-15d %-15d %-5.1f %-5.1f %-5.1f \n", $1,$2,$13,$18,$22
																	 }' >> ${PFILE}
	  done
		
		if [ $CORE -eq 1 ]
		then
			break
		fi		
	done
done                                                                                                 

                                                                                                                                                                
