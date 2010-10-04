#!/bin/bash


      gnuplot << EOF
		  
			set t png font "FreeSans, 11" size 1000,600
			set o "../result/benchmark.png"	
			
			set xlabel "Iterations" 
			
			  
			set ylabel "Chunk Number" 	
			
			
			set title  "Benchmark Property" 
			
			set key center top
		  plot "../data/benchmark_property" u 1:2 t "Thunderbird" w l 1, "" u 1:3 t "Rabbits"  w l 8, "" u 1:4 t "Herschel"  w l 16
			
			set o
			set t										
			quit
EOF
