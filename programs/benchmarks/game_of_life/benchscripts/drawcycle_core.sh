#!/bin/bash

BENCHMARK=$1
DIRC=../result/
ITER=$2

for(( CORE=2; CORE<=64; CORE<<=1 ))
do
	DEST="${DIRC}${BENCHMARK}/cycle/picture/core"
	mkdir $DEST
	INFILE="${DIRC}${BENCHMARK}/cycle/file/core/${CORE}"
	OUTFILE="${DEST}/${CORE}.png"
	if [ -f $INFILE ]
	then
		  gnuplot << EOF
		  
			set t png font "FreeSans, 11" size 800,600
			set o "$OUTFILE"	
			
			set title "$BENCHMARK: Time \n relating to \n Number of Concurrent Chunks and Chunk Size \n in a $CORE + 2-core system "
			set pm3d explicit at s
			set palette model RGB defined( 0 'black', 1 'white')
			
			
			set xyplane 0
			
			set xtics nomirror out rotate by -45
			set xlabel "Chunk Size (Square)" 
			
			set ytics nomirror out rotate by 45  
			set ylabel "Concurrent Chunks" 
			
			set zlabel "Time(sec)" rotate by 90 
						
			
			splot "$INFILE" u 1:2:3:ytic(2) notitle w pm3d, "" u 1:2:3:ytic(2) notitle w points 
			
		
			set o
			set t										
			quit
EOF
	fi
done

