#!/bin/bash

if [ ! -f $1 ] ; then
    echo "File passed is invalid."
    exit 1
fi

for pid in $(cat $1) ; do
    owner=$(ps -f "$pid" | tr -s " " | cut -d " " -f 1 | grep -v -e "UID")
    echo "Owner of process $pid is $owner"

    for child in $(ps -ef | tr -s " " | cut -d " " -f 2,3 | grep -e ".*\s$pid" | cut -d " " -f 1) ; do
        echo "$child is a child of $pid"
    done
done

exit 0
