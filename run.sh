#!/usr/bin/bash

echo "compiling"
clear
make
clear

echo "loading"
./A3 -l

echo "receiving"
./B3

echo "sending"
./A3 -c

echo "watching, refresh every 1000 centiseconds"
./viewData -c 1000
