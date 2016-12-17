#!/bin/bash

rank=0

> $2

while read line; do
     ((rank++))
     echo "Performing measurement on" $line
     size=$(./perform-measurement.sh $line)
     
     if [ $size -ne 0 ]; then
	 echo "...success"
	 echo $rank $line $size >> $2
     else
	 echo "...failed"
     fi
done <$1
