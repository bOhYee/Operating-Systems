#!/bin/bash

# Check the number of parameter (4): file + 3 integers
if [ $# -ne 4 ] ; then
    echo "Error: not enough arguments!"
    exit 1
fi

# Check if the first parameter is the name of an existing file
if [ ! -f "$1" ] ; then
    echo "First parameter is not the name of a valid file!"
    exit 1
fi

file=$1
shift

# Check if integers are positive and ordered
for n in $* ; do
    if [ $n -lt 0 ] ; then
        echo "Integers are not all positive!"
        exit 1
    fi
done

# Last check: $1 must be <= than $2
if [ ! $1 -le $2 ] ; then
    echo "Error: N1 greater than N2!"
    exit 1
fi

for str in $(cat "$file") ; do
    if [ ! -f $str ] ; then
        echo "$str is not a valid file"
    else
        byteOccupied=$(ls -l "$str" | cut -d " " -f 5)
        if [ $byteOccupied -lt $1 ] ; then
            rm "$str"
        elif [ $byteOccupied -gt $2 ] ; then
            newFile=$str".compressed"
            touch "$newFile"

            posCopy=0
            count=1
            for i in $(cat "$str") ; do
                let shouldCopy=$posCopy*$3
                if [ $count -eq $shouldCopy -o $count -eq 1 ] ; then
                    echo -n "$i " >> "$newFile"
                    let posCopy=posCopy+1
                fi
                let count=count+1
            done
            echo "" >> "$newFile"
        fi
    fi
done

exit 0
