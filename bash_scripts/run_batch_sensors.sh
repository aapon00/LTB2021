#!/bin/bash

#run batch sensors 

# for testing
echo $@ >> $1/local_repo/`hostname`.out
RESULTS=$2
echo $RESULTS is results on `hostname`
#echo Dollar3 is $3
#IFS= read -r -a devices <<< "$3"
LOG_FOLDER=$1/local_repo
cd $LOG_FOLDER

#IFS= read -r  -a devices <<< ./batch
mapfile -t devices < ./batch
NUM_DEVICES=${#devices[@]}
echo num_devices is $NUM_DEVICES


#Set up spack
. $HOME/spack/share/spack/setup-env.sh
module load gcc/8.2.0
spack load azureiot@LTS-05_2020_Ref01 arch=x86_64


#cmd="./generate_data ${array[0]} ${array[1]} ${array[2]} ${array[3]} ${array[4]} ${array[5]} ${array[6]} ${array[7]} ${array[8]} --filename ${LOG_FOLDER}/log${array[9]}"

#array[0] -> conn
#array[1] -> number

#Sleep for a second...helps spread out the load
sleep $3
((HALF=$NUM_DEVICES/2))

for ((j=0;j<NUM_DEVICES;j++)); do
	echo "batch: for loop j: " $j 
	IFS=' ' read -r -a array <<< "${devices[$j]}"
	#array[2..len(array)-1] -> everything else
	LENGTH=${#array[@]}

	PARAM=${array[0]}
	PARAM="$PARAM --filename ${LOG_FOLDER}/log${array[1]}"

	for ((i=2;i<LENGTH;i++)); do
		PARAM="$PARAM ${array[$i]}"
	done

	echo $PARAM
	cmd="./generate_data $PARAM"
	echo On node `hostname`
	echo $cmd > cmd_${array[1]}.txt
	$cmd  &
	if [[ j -eq $HALF ]]; then
		echo "at half point, sleeping"
		sleep 1
	fi

done

echo "All processes spawned at `date`. Now waiting"
wait
echo "All procceses finished at `date`"
