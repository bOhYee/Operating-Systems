#!/bin/bash

if [ "$#" -ne 2 ] ; then
    echo "Error: not enough arguments!"
    exit 1
fi

# $1 = source file
if [ ! -f $1 ] ; then
    echo "Source file not valid!"
    exit 1
fi

# $2 = destination path
if [ ! -d $2 ] ; then
  echo "Destination path not valid!"
  exit 1
fi

trans=0
percentage=0

startSpace=$(df $2 | grep -v "File system" | tr -s " " | cut -d " " -f 3)
fileSize=$(ls -l $1 | cut -d " " -f 5)
let fileSize=fileSize/1024


cp $1 $2 &
while [ $trans -lt $fileSize ] ; do
    currSpace=$(df $2 | grep -v "File system" | tr -s " " | cut -d " " -f 3)
    let trans=currSpace-startSpace
    let percentage=trans*100/fileSize
    echo "Progress: $percentage"
    sleep 1
done

exit 0
