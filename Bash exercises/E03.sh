#!/bin/bash

if [ "$#" -lt 1 ] ; then
    echo "Error: not enough arguments!"
    exit 1
fi

n=0
while read a b ; do
      vett[n]=$b
      let n=n+1
      vett[n]=$a
      let n=n+1
done < $1

i=0
echo -n "" > $1
while [ $i -lt $n ] ; do
    a=${vett[$i]}
    let i=i+1
    b=${vett[$i]}
    let i=i+1
    echo $a " " $b >> $1
done

exit 0
