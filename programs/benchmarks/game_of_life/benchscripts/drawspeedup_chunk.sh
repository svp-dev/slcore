#!/bin/bash

BENCHMARK=$1
DIRC=../result/
ITER=$2

for(( CHUNK=10; CHUNK<=100; CHUNK+=10 ))
do
	DEST="${DIRC}${BENCHMARK}/speedup/picture/chunk"
	mkdir $DEST
	INFILE="${DIRC}${BENCHMARK}/speedup/file/chunk/${CHUNK}"
	OUTFILE="${DEST}/${CHUNK}.png"
	if [ -f $INFILE ]
	then
		  gnuplot << EOF
		  
			set t png font "FreeSans, 11" size 800,600
			set o "$OUTFILE"	
			
			
			set pm3d explicit at s
			set palette model RGB defined( 0 'black', 5 'white')
			
			
			set xyplane 0
			
			set xtics nomirror out rotate by -45
			set xlabel "Concurrent Chunk(s)"
						
			set ytics nomirror out rotate by 45
			set ylabel "Number of Cores"
						
			set zlabel "SpeedUp \n (scaled to a 1+1-core system)" rotate by 90
						
			set title  "$BENCHMARK: SpeedUp relating to Number of Cores and Concurrent Chunk(s) for a ${CHUNK}x${CHUNK} Chunk Size" 
						
			splot "$INFILE" u 1:2:3:xtic(1):ytic(2) notitle w pm3d, "" u 1:2:3:xtic(1):ytic(2) notitle w points
			
			set o
			set t			
			quit
EOF
	fi
done

