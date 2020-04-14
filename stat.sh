# Author : Kshitij H. Gurjar (121935013)
# Date : 2nd April 2020  Time : 21:25:56 (hr:mm:sec)


# This is 3rd assignment solution for Displaying RAM,CPU,Memory usage of the system and print them in Descending order of usage


# Here, most important command is "ps".
# "ps" stands for "Process Status". It is used to display processes running on Linux.
# Now, there are various arguments which are given to "ps" command.Some of them I have listed below which are used in this script.


#1. lstart => Determines the Date and Starting time of the process.
#2. pid    => Determines the 'process_ID' which is running.
#3. time   => Determines time in minutes and seconds that the process has been running.
#4. etime  => Determines total eclapsed time which the process took.
#5. comm   => Determines the command(application) name which is running.
#6. %mem   => Determines the memory consumption by the process in %.
#7. %cpu   => Determines the CPU usage by the process in %.
#8. --sort => used for sorting.
#9. head   => Determines total number of processes to be taken in account for tabulation of output of script.


# Here, "--sort = -%cpu" means that you have to sort the processes by virtue of their CPU usage desendingly.
# Also, "head -n 25" means total 25 processes will be included in the list output.



#Following is the shell script.
echo "______________________________________________________________________________"
echo " Shell Script Output of Kshitij (121935013) for RAM,CPU,Memory Usage"
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
if [ $1 == "help" ] ;then
    echo "___________________________________"
    echo "Help for this program"
    echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    echo "This script will give you RAM,CPU usage. "
    echo "In order to run this script, in Terminal go to specified directory where .sh file is saved"
    echo "and then in order to run the script, type-->>   ./file_name.sh"
    echo "Thank You "
    echo "___________________________________"
else
    echo "$(ps -eo lstart,time,etime,pid,comm,%mem,%cpu --sort=-%cpu | head -n 25)" 
    echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
fi
