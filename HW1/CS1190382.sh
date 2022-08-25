#!/bin/bash
#set -x
if [ "$1" = "-apriori" ]; then
	./apriori $2 $3 $4
elif [ "$1" = "-fptree" ]; then
	./fpt $2 $3 $4
elif [ "$1" = "-plot" ]; then
	./plot $2 $3
fi
