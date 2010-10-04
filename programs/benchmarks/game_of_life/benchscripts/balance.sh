#!/bin/bash

BENCHMARK=$1
DIRC=../results/
ITER=$2

for(( CORE=2; CORE<=64; CORE<<=1 ))
do
	for(( BLOCK=1; BLOCK<=32; BLOCK<<=1 ))                                                                                                                   
	do
		for(( CHUNK=10; CHUNK<=100; CHUNK+=10 ))
		do
			unset Ins
			INFILE="${DIRC}${BENCHMARK}/${BENCHMARK}_${CORE}_${BLOCK}_${CHUNK}_${ITER}_2_PART"
			OUTFILE="${DIRC}${BENCHMARK}/balance/file/${CORE}_${BLOCK}_${CHUNK}_${ITER}_2"
			if [ -f $OUTFILE ]
			then
				rm -f $OUTFILE
			fi
			if [ -f $INFILE ] 
				then
						Ins=(`grep '^[0-9]' < ${INFILE} | gawk  -v cn=$CORE  '{	
																																		if( FNR >= 5 && FNR <= 4+cn)
																																			 printf "%d ",$2		
																								   																																	
																																	}'`)
					  total=0
						for(( i=0; i<${#Ins[@]}; i++ ))
						do
								total=$[$total + ${Ins[$i]}]
						done
						average=$[$total / $CORE ]
						for(( j=0; j<${#Ins[@]}; j++ ))
						do 
							 speed=`echo "scale=5; ${Ins[$j]} / $average * 100 - 100 " | bc `
							 printf "%-10.3f\n" $speed >> $OUTFILE
						done
						
			fi
		done
	done
done
