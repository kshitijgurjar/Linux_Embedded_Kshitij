#!/bin/bash

files=$(find $2 -type f -size -$1k)  #variable with 'find' command 

echo "Showing all files smaller than $1 bytes in $2\n\n"


echo $files             #enlist all files which are less than 1024 bytes

for i in $files ; do    #for loop
    rm $i
done

echo "Files succesfully deleted which are below 1k size"