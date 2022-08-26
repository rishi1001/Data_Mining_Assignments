#!/bin/bash
#set -x
if [ "$1" = "-apriori" ]; then
	./apriori $2 $3 $4
elif [ "$1" = "-fptree" ]; then
	./fpt $2 $3 $4
elif [ "$1" = "-plot" ]; then
	python plot.py $2 $3
fi
