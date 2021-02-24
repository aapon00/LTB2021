#!/bin/bash

# Don't run unless you need to create more devices 

AZ=$(which az)
num_python_devices=1000
IOT_HUB="dicelab"
start=1
#/zfs/dicelab/bin/az login

while [ $start -le $num_python_devices ]
do
	$AZ iot hub device-identity create \
		--device-id python_device$start \
		--hub-name $IOT_HUB
	start=$(( $start + 1 ))
done
