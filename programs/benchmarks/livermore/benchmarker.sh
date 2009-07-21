#!/bin/bash
#------------------------------------
#	SVP Benchmarker
#    M.A.Hicks -- UvA (06/2009)
#------------------------------------
# The first parameter to this
# script must be a directory containing
# executable benchmarks.
#------------------------------------

BENCHMARKS="blah"
BENCHGLOB="bench*.bin.mtalpha"
BENCHFILTER=".bin.mtalpha"
RUNNER="slr"
VARYCORES=' -Ws,-o -Ws,NumProcessors='
VARYMEM=' -Ws,-o -Ws,MemoryBanks='
FPUS=" -Ws,-o -Ws,NumProcessorsPerFPU=1"
PROBLEM_SIZE="7000"
CORES="1 2 4 8 16"
CYCLES="CYCLES STORED HERE"
FLOPS="FLOPS STORED HERE"
INSNS="INSTRUCTIONS STORED HERE"
PSIZE_EXT=".psize"
EXP_FILE="bench.exp" #file in which benchmark override spec resides
TITLE="NO TITLE"
BASEDIR="WHOOPS"
TEMPDIR="/tmp"
START_TIME=`date`
RESINFILE="n"
RESULTS_FILE="results.csv"

NOTES="" #any information that should be written to the end of results

#How to capture output:
#CAPTURE_CYCLES='grep Cycles: | sed -e "s/ Cycles: //g"'
CAPTURE_CYCLES="grep ';' |cut -d ';' -f 1 | sed -e 's/ //g'"
CAPTURE_FLOPS="grep ';' | cut -d ';' -f 2 | cut -d ' ' -f 3 | sed -e 's/ //g'"
CAPTURE_INSNS="grep ';' | cut -d ';' -f 2 | cut -d ' ' -f 2 | sed -e 's/ //g'"


# No need to touch the variables below:
EXPNUM=0
ERRORS=f

# execute the first parameter with the runner and return the number of cycles
# second parameter is the number of cores to use
function execute {
	#execute the benchmark and store the result in a temp file
	
	RESULTSFILE="MGresult.tmp"
	RESULTSFULL="MGresults.log"
	
	#make temp dir = base dir	
	TEMPDIR=$BASEDIR
	
	#capture output of this experiment
	
	COMMAND="echo 1 $3 | $RUNNER -f - $VARYCORES$2 $VARYMEM$2 $FPUS $1  &> $TEMPDIR/$RESULTSFILE" 
	
	eval $COMMAND
	
	#now store all results, just in case we need them later
	echo '***RESULTS for (' "$START_TIME" ') @'  "$2 Cores; $3 Problem Size"'***' >> $BASEDIR/$RESULTSFULL 
	echo $COMMAND >> $BASEDIR/$RESULTSFULL
	cat $TEMPDIR/$RESULTSFILE >> $BASEDIR/$RESULTSFULL
	
	#now set the various variables
	GET_TEMP="cat $TEMPDIR/$RESULTSFILE | $CAPTURE_CYCLES"
	CYCLES=`eval $GET_TEMP`

	GET_TEMP="cat $TEMPDIR/$RESULTSFILE | $CAPTURE_FLOPS"
	FLOPS=`eval $GET_TEMP`

	GET_TEMP="cat $TEMPDIR/$RESULTSFILE | $CAPTURE_INSNS"
	INSNS=`eval $GET_TEMP`
	
	return 0
}

function infoout {
#	echo $1 1>&2
        echo -e "\E[36m$1" 1>&2
        echo -n -e "\E[00;00m" 1>&2
}

function resout {
	if [ "$RESINFILE" = "y" ]; then
		echo -n "$1" >> $BASEDIR/$RESULTS_FILE
	else
		echo -n "$1" 
	fi
}

function resnewline {
	if [ "$RESINFILE" = "y" ]; then
	        echo >> $BASEDIR/$RESULTS_FILE
	else
	        echo
	fi
}

#execute the benchmarks
function benchmark {

	#set the base directory of the experiments
	BASEDIR="$1"

	#check to see if an experiment file exists
	if [ -f $1/$EXP_FILE ]; then
		source $1/$EXP_FILE
		infoout "Found experiment file ($TITLE)."
	fi
	
	#output a new line in the results
	resout "*** Experiments started at $START_TIME ***"
	resnewline

	#first, print the header line
	resout "benchmark/cores"
	for CORE in $CORES;
	do
		resout ",$CORE"
		EXPNUM=$[$EXPNUM + 1]
	done
	#output the last fixed three elements of each exp
	resout ",FLOPS,INSNS,PSIZE"
	resnewline #to add new line

	#calculate the final number of experiments
	EXPNUM=$[$EXPNUM * (`ls -l $1/$BENCHGLOB | wc -l`)]

	infoout "Now executing $EXPNUM experiments..." 
	
	CUREXP=0 #first exp
	
	#iterate through each benchmark file
	for BENCHMARK in `ls $1/$BENCHGLOB`;
	do
		resout `basename $BENCHMARK | sed -e "s/$BENCHFILTER//g" -e 's/\///g'`

		#put a star after the name if a PSZIE override was found
		if [ -f $BENCHMARK$PSIZE_EXT ]; then
			resout '*'
		fi
		
		#now execute the benchmark for number of cores  and output the cycles
		for CORE in $CORES;
                do
                #across each problem size
                	for PSIZE in $PROBLEM_SIZE;
                	do
                		#check to see if we should override PSIZE
				if [ -f $BENCHMARK$PSIZE_EXT ]; then
					PSIZE=`cat $BENCHMARK$PSIZE_EXT`
				fi

 				#output progress
 				CUREXP=$[$CUREXP + 1]
 				echo -n " $CUREXP.." 1>&2
                                                                 
				execute $BENCHMARK $CORE $PSIZE
                	
	                	if [ "$CYCLES" = "" ]; then
	                		resout ",RUN_ERROR"
	                		ERRORS="t"
	                	else
		                	resout ",$CYCLES" #number of cycles
		                fi
	                done
                done
		
		#output the number of flops used by this benchmark
		resout ",$FLOPS"
		#output the number of instructions used by this benchmark
		resout ",$INSNS"
		#output the problem size
		resout ",$PSIZE"
		
		resnewline #start a new line for the next result	
	done

	#output and experiment notes
	if [ NOTES != "" ]; then
		resout "$NOTES"
		resnewline
	fi
	resout "***  Results End (from $START_TIME)  ***"
	resnewline
	
	infoout ""
	infoout "Experiments completed!"
	if [ "$ERRORS" = "t" ]; then
		infoout "*BUT* with errors!"
	fi
}

#call the main function
benchmark $1
