#!/bin/bash

if [ "$#" -lt 3 ] ; then
    echo "Error: not enough parameters!"
    exit 1
fi

echo "$1 $2 $3"
find "$1" -exec grep -Hn "$2" "{}" \; > "$3"
#grep -e ".*\s" "$3" | sort -k 2 -k 3 -o="$3" "$3"

exit 0
