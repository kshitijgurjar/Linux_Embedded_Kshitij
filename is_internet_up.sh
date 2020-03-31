# Author : Kshitij Gurjar 121935013
# This is the 1st assignment where a shell script is written to check whether internet is connected or not.
# Here 'ncat' command is used where Connection to 8.8.8.8 port 53 is established and it returns a value 0 if connection is working.


#! /bin/bash

nc -z 8.8.8.8 53           # will return 0 if internet working
online=$?                  # variable storing the return value
if [ $online -eq 0 ]; then
    echo "Internet is UP"
else
    echo "Internet is DOWN"
fi
