#! /bin/bash

BENCHMARK=$1
DIRC=../result/
ITER=$2
DEST="${DIRC}${BENCHMARK}/cycle/file/chunk"	
mkdir $DEST

for(( CHUNK=10; CHUNK<=100; CHUNK+=10 ))
do	
	INPFILE="${DIRC}${BENCHMARK}/${BENCHMARK}_1_1_${CHUNK}_${ITER}_1_PART"
	OUTFILE="$DEST/${CHUNK}"	
	printf "# %-4s  %-5s  %-5s\n" "Block" "Core" "Cycle" > $OUTFILE
	for(( BLOCK=1; BLOCK<=32; BLOCK<<=1 ))
	do 	
	  for(( CORE=2; CORE<=64; CORE<<=1 ))                                                                                                                   
		do
		   	INFILE="${DIRC}${BENCHMARK}/${BENCHMARK}_${CORE}_${BLOCK}_${CHUNK}_${ITER}_2_PART"		  	
		  	if [ -f $INFILE ] 
		  	then
		  			printf "%-5d  %-5d  " ${BLOCK} ${CORE} >> $OUTFILE
						grep '^[0-9]' < ${INFILE} | gawk  '{  if( FNR == 1)
																										printf "%5.3f\n",$1/1000000000																										
																							  }' >> $OUTFILE
			 	fi		
		done
		
		printf "\n"	>> $OUTFILE
		
	done
done

   