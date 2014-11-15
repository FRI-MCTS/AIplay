#!/bin/sh

INPUT_FILE_XRSL="aiplay.xrsl"
OUTPUT_FILE_XRSL="aiplay_temp.xrsl"

INPUT_FILE_SH="aiplay.sh"
OUTPUT_FILE_SH="aiplay_temp.sh"

METHOD="fix"
NUM_GAMES=1200
REPEATS=1
PLAYER1_UCT="1000"
PLAYER2_UCT=$PLAYER1_UCT
CORES=1
MPIARGS="\$MPIARGS"

if [ $# -gt 0 ]; then
	METHOD=$1
fi
if [ $# -gt 1 ]; then
	NUM_GAMES=$2
fi
if [ $# -gt 2 ]; then
	PLAYER1_UCT=$3
fi
if [ $# -gt 3 ]; then
	PLAYER2_UCT=$4
fi
if [ $# -gt 4 ]; then
	REPEATS=$5
fi
if [ $# -gt 5 ]; then
	CORES=$6
fi

if [ $CORES -eq 1 ]; then
	MPIARGS="-np 1"
fi

sed -e "s/%(MPIARGS)/$MPIARGS/" $INPUT_FILE_SH > $OUTPUT_FILE_SH

sed -e	"s/%(NUM_GAMES)/$NUM_GAMES/" \
		-e  "s/%(REPEATS)/$REPEATS/" \
		-e	"s/%(PLAYER1_UCT)/$PLAYER1_UCT/" \
		-e  "s/%(PLAYER2_UCT)/$PLAYER2_UCT/" \
		-e  "s/%(METHOD)/$METHOD/" \
		-e	"s/%(CORES)/$CORES/" $INPUT_FILE_XRSL > $OUTPUT_FILE_XRSL

arcsub -c jost.arnes.si -o ./grid/ids aiplay_temp.xrsl

