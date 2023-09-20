#!/bin/bash

writeFileDir="$1"
writeStr="$2"

# Remove the file basename from the directory path
 writeDir=${writeFileDir%/*}/

# Check for number of arguments
if [ $# -ne 2 ]; then
	echo "Required Arguments not specified. Please specify as below"
	echo "$0 /path/to/writ/directory write_string"
	exit 1
fi

# Make the path of the desired directory
 mkdir -p $writeDir

# Create File
 touch $(basename $writeDir)

# Write File
echo $writeStr > $writeFileDir


