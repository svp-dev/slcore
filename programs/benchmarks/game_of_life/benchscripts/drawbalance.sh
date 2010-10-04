#!/bin/bash

BENCHMARK=$1
DIRC=../result/
ITER=$2


for(( CORE=2; CORE<=64; CORE<<=1 ))
do
		for(( CHUNK=10; CHUNK<=100; CHUNK+=10 ))
		do
			INFILE="${DIRC}${BENCHMARK}/balance/file/${CORE}_32_${CHUNK}_${ITER}_2"
			OUTFILE="${DIRC}${BENCHMARK}/balance/picture/${CORE}_32_${CHUNK}.png"
			if [ -f $INFILE ]
			then				 
						gnuplot << EOF
					
						set t png
						set o "$OUTFILE"
						
						set style fill solid
						
						set xlabel "Core ID"
						set ylabel "Load (scaled to average)"
						
						set title  "$BENCHMARK: Load Balance of Group family with $CORE cores, $BLOCK blocks and $CHUNK chunks"
						  
						set palette model RGB defined ( -10 'blue', 2 'white', 5 'yellow', 10 'red' )
						
						plot "$INFILE" u ( $0 + 1 ):1 notitle w boxes	palette
															
						set o
						set t										
						quit
EOF
			fi
		
		 done
done

