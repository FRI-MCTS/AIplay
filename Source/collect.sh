#!/bin/sh

DIR="*/"

if [ $# -gt 0 ]; then
	DIR=$1/*/
fi

for d in $DIR; do
	g=$d/general.csv

	if [ ! -e $g ]; then
		echo "Ignoring $d, not a grid directory."
		continue;
	fi

	IFS=';'
	read n r uct1 uct2 c t < $g

	new_name="N"$n"_R"$r"_UCT"$uct1"_"$uct2"_C"$c""
	echo "Renaming $d to $new_name."
	mv $d $new_name 2>/dev/null

	> times.csv
	echo "$t;" >> times.csv

done
