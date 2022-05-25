#!/bin/bash

# Check the number of arguments passed
if [ $# -ne 1 ] ; then
    echo "Error: only a file name must be passed!"
    exit 1
fi

# Check if the argument is a valid file name
if [ ! -f "$1" ] ; then
    echo "Error: the argument passed does not represent a valid file name!"
    exit 1
fi

# New file name
base=$(basename "$1" ".txt")
newFile=$base".xyz"

# Copy the content and clean the old file
cp "$1" "$newFile"
echo -n "" > "$1"

# Extract the number of lines and word for line to create the new strings to append
totLine=$(cat "$newFile" | wc -l)
while read line ; do
    wordNum=$(echo "$line" | wc -w)
    echo "$wordNum $totLine $line" >> "$1"
done < "$newFile"

# Sorting the lines
cat "$1" | sort -n -k 1 -o "$1"
echo "Task completed."

exit 0
