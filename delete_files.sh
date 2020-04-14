# Author :Kshitij H. Gurjar 
# Date:31March2020 Time::4:31:50

# This is the second assignment of T2 where a shell script is written to remove the files less than 1024 bytes from the specified directory.
# Give the arguments as specified in format "./(script_name).sh 1024 /(directory_path)" 
# e.g. "delete_files.sh 1024 /home/kshitij/delete_file_folder"


# to find the files as per condition, "find" command is used which argument of type -file and size as 1k.
# "echo" is used to print the statements


# atlast a 'for' loop runs through all files collected by '$files' variable and removes them using remove "rm" command.


# Follwoing is the script
#!/bin/bash

if [ $1 == "help" ] ;then
    echo "___________________________________"
    echo "Help for this program"
    echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    echo "Enter the input as ./file_name.sh (space) 1024(size) (space) directory_path "
    echo "for e.g ./delete_files.sh 1024 ./home/kshitij/delete_files_folder"
    echo "Sizes are of units 'bits'"
    echo "Thank You "
    echo "___________________________________"
else
    if [[ $1 =~ ^[+-]?[0-9]+$ ]]; then       #Checking weather the 'size' is INTEGER or not
        files=$(find $2 -type f -size -$1k)  #variable with 'find' command 
        echo "Showing all files smaller than $1 bytes in $2\n\n"
        echo $files             #enlist all files which are less than 1024 bytes
        for i in $files ; do    #for loop
            rm $i
        done
        echo "Files succesfully deleted which are below 1k size"
    else
    echo "Invalid 'size' input.Enter size as INTEGER e.g 1024" 
    fi
fi
