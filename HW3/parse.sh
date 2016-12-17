#!/bin/bash

if [ $# -lt 2 ]; then
    echo "This script needs at least two arguments" >&2
    exit 1
fi

if ! [ -e $1 ]; then
    echo "Input file does not appear to exist" >&2
    exit 1
fi

# Remove old contents of output file
> $2

grep -E "http://" $1 | while read -r line ; do
    tmpline=$(echo $line | sed 's/.*http:\/\//http:\/\//' | sed 's/">.*//' | sed 's/.*100bestwebsites.org.*//' | sed 's/.*\/wiki.*//' )
    if ! [ -z $tmpline ]; then
	echo $tmpline >> $2
    fi
done
