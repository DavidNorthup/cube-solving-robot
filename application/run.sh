#!/bin/bash

echo "Finiding the arduino devices"
# Locate the arduino devices
devices=$(ls /dev/ttyUSB*)
arg=$(echo "$devices" | tr '\r\n' ' ')
ports=($arg)

echo "Checking if recompilation needed"
make

echo "Starting: "
./csr.exe $arg