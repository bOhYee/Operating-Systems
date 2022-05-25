#!/bin/bash

if [ "$#" -lt 1 ] ; then
    echo "Error: not enough arguments!"
    exit 1
fi

# One line at a time
i=0
while read line ; do
    let i=i+1
    echo "Line "$i": "$line
done < $1

# One string at a time
i=0
for str in $(cat  $1) ; do
    let i=i+1
    echo "String "$i": "$str
done

exit 0
