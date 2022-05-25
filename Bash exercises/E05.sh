#!/bin/bash

if [ "$#" -lt 1 ] ; then
    echo "Error: not enough arguments!"
    exit 1
fi

for n in $(cat $1) ; do
    i=0
    while [ $i -lt $n ] ; do
        echo -n "*"
        let i=i+1
    done
    echo ''
done

exit 0
