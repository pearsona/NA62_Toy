#!/bin/bash

read -p "would you like to compile? (y/n): " ans

if [ $ans = "y" ] || [ $ans = "Y" ]
then
    clear
    echo "compiling"
    make
fi

read -p "would you like to clean up the memory first? (NOTE: This is already done if you just compiled) (y/n): " ans

if [ $ans = 'y' ] || [ $ans = 'Y' ]
then
    ./ab3Clean
fi

echo "loading"
./A3 -l

echo "receiving"
./B3 &

echo "sending"
./A3 -c &


read -p "Command line arguments to viewData? (e.g. -c1 would be refresh every second, nothing would be only output once): " ans

echo "watching"
./viewData $ans
