#! /bin/bash

BENCHMARK=$1
DIRC=../result/
ITER=$2
DEST="${DIRC}${BENCHMARK}/speedup/file/block"
mkdir $DEST

for(( CK=10; CK<=100; CK+=10 ))
do
	INFILE="${DIRC}${BENCHMARK}/${BENCHMARK}_1_1_${CK}_${ITER}_1_PART"
	cycle=`grep '^[0-9]' < ${INFILE} | gawk '{if( FNR == 1)printf "%d", $1}'`
	i=$[$CK / 10 - 1 ]
	Standard[$i]=$cycle
done

for(( BLOCK=1; BLOCK<=32; BLOCK<<=1 ))
do
	OUTFILE="$DEST/$BLOCK"
	printf "# %-4s  %-5s  %-5s\n" "Chunk" "Core" "SpeedUp" > $OUTFILE		
	for(( CHUNK=10; CHUNK<=100; CHUNK+=10 ))
	do 
		index=$[$CHUNK / 10 - 1 ]
	  for(( CORE=2; CORE<=64; CORE<<=1 ))                                                                                                                   
		do
		   	INFILE="${DIRC}${BENCHMARK}/${BENCHMARK}_${CORE}_${BLOCK}_${CHUNK}_${ITER}_2_PART"
		  	if [ -f $INFILE ] 
		  	then
		  			printf "%-5d  %-5d  " ${CHUNK} ${CORE} >> $OUTFILE
						grep '^[0-9]' < ${INFILE} | gawk -v st=${Standard[$index]} '{
																																				 if( FNR == 1)
																																						printf "%-5.2f\n",st/$1																										
																									 										  }' >> $OUTFILE
			 	fi		
		done
		
		printf "\n"	>> $OUTFILE
		
	done
done
	
   
