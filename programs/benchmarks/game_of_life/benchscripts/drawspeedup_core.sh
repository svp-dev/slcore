#!/bin/bash

BENCHMARK=$1
DIRC=../result/
ITER=$2

for(( CORE=2; CORE<=64; CORE<<=1 ))
do
	DEST="${DIRC}${BENCHMARK}/speedup/picture/core"
	mkdir $DEST
	INFILE="${DIRC}${BENCHMARK}/speedup/file/core/${CORE}"
	OUTFILE="${DEST}/${CORE}.png"
	if [ -f $INFILE ]
	then
		  gnuplot << EOF
		  
			set t png font "FreeSans, 11" size 1000,600
			set o "$OUTFILE"	
			
			set multiplot layout 1,2 title "$BENCHMARK: SpeedUp and Efficiency \n relating to \n Number of Concurrent Chunks and Chunk Size \n in a $CORE + 2-core system "
			set bmargin 5
			set pm3d explicit at s
			set palette model RGB defined( 0 'black', 5 'white')
			
			
			set xyplane 0
			
			set xtics nomirror out rotate by -45
			set xlabel "Chunk Size (Square)" 
			
			set ytics nomirror out rotate by 45  
			set ylabel "Concurrent Chunks" 
			
			set zlabel "SpeedUp \n (scaled to a 1+1-core system)" rotate by 90 
			
			set title  "SpeedUp" 
			
			
			splot "$INFILE" u 1:2:3:ytic(2) notitle w pm3d, "" u 1:2:3:ytic(2) notitle w points 
			
			set zlabel "Efficiency(%)\n (average of $CORE working cores)" rotate by 90 
			set title "Efficiency"
			
			splot "$INFILE" u 1:2:4:ytic(2) notitle w pm3d, "" u 1:2:4:ytic(2) notitle w points 
			
			unset multiplot										
			set o
			set t										
			quit
EOF
	fi
done

