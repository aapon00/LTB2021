#!/bin/bash

if [ $# -lt 3 ]; then #Not enough arguments
	echo "USAGE: ./run_multiple_sensors <parameters_file> <connection_strings_file> <log_folder> [optional:generate_data]"
	exit 1
elif [ $# -eq 3 ]; then #Generate_data not specified, using default
	REPO=$(cd "$(dirname "$(realpath "${BASH_SOURCE[0]}")")" && git rev-parse --show-toplevel)
	GENERATE_DATA=${REPO}/C_Synthetic_Data_Generator/cmake/generate_data
elif [ $# -eq 4 ]; then #Generate_data specified
	GENERATE_DATA=$4
else #Excess arguements
	echo "USAGE: ./run_multiple_sensors <parameters_file> <connection_strings_file> <logs> [optional:generate_data]"
	exit 1
fi

LOG_FOLDER=$3

# make sure there are two extra cmd line args
num_devices=0
i=0

echo paramater_array: $1
echo connection_strings: $2
echo logs: $LOG_FOLDER
echo generate_data: $GENERATE_DATA

# mapfile reads lines of a file into an array where each line in the file is an element in the array
mapfile -t parameters_array <$1
mapfile -t connection_strings <$2

# first half of parameters.list: how many devices per set of parameters
# second half of parameters.list: the sets of parameters (each set defines a device)
half_size=$(( ${#parameters_array[@]} / 2 ))

# determine how many connection strings (and therefore devices) are stored on this machine
num_available_devices=${#connection_strings[@]}
echo $num_available_devices
# determine how many devices the experiment wants to use
# --- sum the number of devices specified in the first half of parameters.list
while [ $i -lt $half_size ]
do
	num_devices=$(( $num_devices + ${parameters_array[i]} ))
	i=$(( $i + 1 ))
done

i=0

# print an error if we do not have enough devices for the experiment
if [ $num_available_devices -lt $num_devices ]
then
	echo "ERROR: more devices requested than avaialable (asked for ${num_devices}, but only have ${num_available_devices} available)" >&2
# otherwise, loop through devices and run the experiment (i.e. generate data and send to IotHub)
# --- each iteration creates one new device per set of parameters, instead of all the devices with one set of parameters, then all the devices with another set, etc.
else
	count=0
	while [ $count -lt $num_devices ]
	do
		j=0
		while [ $j -lt $half_size ]
		do
			# decrement number of devices remaining per set of parameters until it is 0
			if [ ${parameters_array[$j]} -gt 0 ]
			then
				param_index=$(( $j + $half_size ))

				$GENERATE_DATA ${connection_strings[count]} ${parameters_array[$param_index]} --filename "$LOG_FOLDER"/log$i &
				count=$(( $count + 1 ))
				parameters_array[$j]=$(( ${parameters_array[$j]} - 1 ))
				i=$(( $i + 1 ))
			fi
			j=$(( $j + 1 ))
		done
	done
	echo "All processes spawned at `date`. Now waiting"
	wait
	echo "All procceses finished at `date`"
fi

exit 0
