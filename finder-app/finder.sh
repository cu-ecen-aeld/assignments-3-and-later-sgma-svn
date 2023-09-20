#!/bin/bash

filesDir="$1"
searchStr="$2"

if [ $# -ne 2 ]; then
	echo "Required Arguments not specified. Please specify as below"
	echo "$0 /path/to/search/directory search_string"
	exit 1
fi


if [ ! -d $filesDir ]; then
	echo "$fileDir does not exist"
	exit 1
fi

# Matching Files found
files=$(find $filesDir -type f -exec grep -l $searchStr {} + | wc -l)

# Number of Matching lines
lines=$(grep -r $searchStr $filesDir | wc -l)
echo "The number of files are $files and the number of matching lines are $lines"
