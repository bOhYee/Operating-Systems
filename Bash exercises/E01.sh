#!/bin/bash

if [ "$#" -lt 1 ] ; then
    echo "Error: not enough arguments!"
    exit 1
fi

# Read a file of numbers and calculates out as:
# out = 3*x*x + 4*y + z*5
while read x y z ; do
    let out=3*x*x
    let out=out+4*y
    let out=out+z*5
    echo $out
done < $1
