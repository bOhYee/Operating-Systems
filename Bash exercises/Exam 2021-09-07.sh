#!/bin/bash

if [ ! "$#" -ge 1 ] ; then
    echo "Error: insert at least the name of one file!"
    exit 1
fi

for file in $* ; do
    if [ -f "$file" ] ; then
        echo "$file is a regular file."
        dim=$(ls -l "$file" | tr -s " " | cut -f 5 -d " ")
        perm=$(ls -l "$file" | tr -s " " | cut -f 1 -d " "| cut -c 2,3)
        echo "File $(basename $file) has a dimension of $dim byte and the current user has $perm permission on this file."
    elif [ -d "$file" ] ; then
        echo "$file is a  directory"
        curr=$(pwd)
        cd $file
        ls_num=$(find . -maxdepth 1 -type d | grep -v -e "\.$" -e "\.\.$" | wc -l)
        ls_fold=$(find . -maxdepth 1 -type d | grep -v -e "\.$" -e "\.\.$")
        echo "Found $ls_num in $file"
        echo "Folders: "
        for dir in $ls_fold ; do
            echo -n "$dir"
        done
        echo ""
    else
        echo "Error: $file is neither a regular file nor a directory!"
        continue
    fi
done

exit 0
