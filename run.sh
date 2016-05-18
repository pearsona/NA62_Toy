#!/bin/bash

clear

read -p "Would you like to compile? (y/n): " ans
if [ $ans = "y" ] || [ $ans = "Y" ]
then
    echo -e "\ncompiling"
    make

else
    read -p "Would you like to clean up the memory first? (NOTE: This is already done if you just compiled) (y/n): " ans
    if [ $ans = 'y' ] || [ $ans = 'Y' ]
    then
	./clean
    fi

fi

echo -e "\nloading and sending"
./A -l &


echo -e "\nreceiving"
./B &


#read -p "Command line arguments to viewData? e.g. -c1 would refresh every second (maybe longer depending on speed of other processes) and no arguments (i.e. press enter) would only output once: " ans

#echo -e "\nwatching"
#./viewData $ans
