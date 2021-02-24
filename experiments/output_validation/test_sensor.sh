#!/bin/bash

#Set up location variables
REPO=/root/DigitalPlant #EDIT ME
GENERATE_DATA=${REPO}/C_Synthetic_Data_Generator/cmake/generate_data
if [[ ! -f $GENERATE_DATA ]]; then
	echo "Could not find generate_data at $GENERATE_DATA"
	exit 1
fi


#Grab connection string from args, otherwise use default
CONN_STRING=$1
if [ ! $# -eq 1 ]; then #Not enough arguments
  echo "USAGE: ./run_multiple_sensors <connection_string>"
  echo "using default" 
  CONN_STRING="HostName=dicelab.azure-devices.net;DeviceId=device1;SharedAccessKey=YVKBlwaqrTkfqYIRfgS/RuHo+F5B6jyy0moJ9dR9Nxk="
fi

#Debugging print outs
echo connection_strings: $CONN_STRING
#echo logs: $LOG_FOLDER
echo generate_data: $GENERATE_DATA

#TCPDUMP SETUP, starndard enthernet port listing on 8883, the azure IoT hub port. 
tcpdump -i eth0 port 8883 > tcpdump.raw.pareto.3.71 &
PID=$!

#RUN EXPERIMENT

#For pareto uncomment this
$GENERATE_DATA $CONN_STRING --distribution pareto --shape 2 --intermessage 71 --msgcount 1000 --type Size512 --filename large_pareto

#For constant uncomment this
#$GENERATE_DATA $CONN_STRING --distribution constant --intermessage 100 --time 30 --type Size512 --filename 512

#END TCPDUMP
kill $PID
wait

