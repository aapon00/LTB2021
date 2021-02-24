#!/bin/bash

DATE=$(echo $(date +"%B_%d_%H_%M_%S")_$HOSTNAME  | cut -d . -f 1)_4units
REPO=/zfs/dicelab/DigitalPlant/
TEMPLATE=$REPO/pbs/multi_node_experiment.template
PREFIX=multi_node
RESULTS_PARENT=$REPO/results/$DATE

NUM_BOXES=(1 2 4)
CPUS_PER_BOX=40

mkdir $RESULTS_PARENT


if [[ ! -f $TEMPLATE ]]; then
	echo "Template at $TEMPLATE not found"
	exit 2
fi

for num in "${NUM_BOXES[@]}"; do
	echo $num
	dest="$PREFIX"_"$num".pbs
	cp $TEMPLATE $dest

	#change num_boxes
	regex="s/%NUM_BOXES%/"$num"/g"
	perl -pi -e $regex $dest

	#Change results destination
	escaped_results_parent=$(echo $RESULTS_PARENT |sed --expression='s/\//\\\//g') #escape file path /zfs/ --> \/zfs\/
	regex="s/%RESULTS%/$escaped_results_parent/g"
	echo $regex
	perl -pi -e $regex $dest

done

for para in "${NUM_BOXES[@]}"; do
	#(( devices = $para * $CPUS_PER_BOX ))
	devices=4000
	printf "$devices\n\
--distribution constant --intermessage 200 --time 5400 --type Size2048\n"\
	>> ./parameter"$para".list
done




exit 0
