#!/bin/bash
path="/home/"$USER

for file in $(find $path -type f -name "*.c") ; do
    name=$(basename "$file")
    lines_match=$(grep -e "$name" "$file" | wc -l)
    if [ $lines_match -ge 1 ] ; then
        mv "$file" "$file""_new"
    fi
done

exit 0
