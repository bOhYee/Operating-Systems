#!/bin/bash

# Check the number of arguments
if [ "$#" -ne 1 ] ; then
    echo "Program need only one argument to run correctly."
    exit 1
fi

# Check the file indicated
if [ ! -f "$1" ] ; then
    echo "Parameter does not indicate a regular file."
    exit 1
fi

testFile="TestFileTmp.txt"

while read line ; do
    num_words=$(echo "$line" | wc -w)
    let odd=$num_words%2

    if [ $odd -eq 1 ] ; then
        new_line=$(echo "$line" | tr [:upper:] [:lower:])
    else
        new_line=$(echo "$line" | tr [:lower:] [:upper:])
    fi

    echo "$new_line"
done < "$1" > "$testFile"

cp "$testFile" "$1"
rm "$testFile"

exit 0
