#!/bin/bash

AZ=$(which az)
num_devices=1000
IOTHUB="dicelab"
#TIER="32.list"
i=1

while [ $i -le $num_devices ]
do
	CONNECT_STRING=""
    while [ -z $CONNECT_STRING ]
	do
    	CONNECT_STRING=$($AZ iot hub device-identity show-connection-string --device-id python_device$i --hub-name $IOTHUB --query "connectionString" )
	done
	echo $CONNECT_STRING >> connection_strings.list
	i=$(( $i + 1 ))
done
