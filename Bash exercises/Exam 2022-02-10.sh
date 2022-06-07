#!/bin/bash

# Check of the number of arguments
if [ ! "$#" -eq  6 ] ; then
    echo "Error: not enough arguments!"
    exit 1
fi

# Check the validity of the file arguments ($1 and $6)
if [ ! -f $1 ] ; then
    echo "Error: indicated files are not valid!"
    exit 1
fi

# Create the initial and final padding
let i=0
pad=""
while [ $i -lt $4 ] ; do
    pad=$pad$5" "
    let i=i+1
done

# Elaboration of the file
while read  line ; do
    newLine=""
    for word in $line ; do
        if  [ "$word" = "$2" ] ; then
            word=$3
        fi
        newLine=$newLine$word" "
    done
    newLine=$pad$newLine$pad
    echo "$newLine" >> $6
done < $1

exit 0
