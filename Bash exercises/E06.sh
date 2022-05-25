#!/bin/bash

# At least directory parameter and a file parameter must be defined
if [ "$#" -lt 2 ] ; then
    echo "Error: not enough arguments!"
    exit 1
fi

if [ ! -d $1 ] ; then
    mkdir $1
fi

dir=$1
shift
for str in $* ; do
    echo -n "Copy file $str in directory $dir? (yes/no): "
    read res
    if [ $res = "yes" ] ; then
        cp $str $dir
    fi
done


exit 0
