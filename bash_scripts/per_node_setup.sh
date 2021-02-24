#!/bin/bash

######################################################
#$TMPDIR is used to access local_scratch -> /local_scratch/pbs.$PBS_JOBID/
#if [ ! -z "$PARAMETER_LOC" ]; then PARAMETER_LOC=/zfs/dicelab/DigitalPlant/bash_scripts/parameters.list; fi
#REPO=$(cd "$(dirname "$(realpath "${BASH_SOURCE[0]}")")" && git rev-parse --show-toplevel)
echo "RUNNING PER NODE SETUP"

#Declare constants

REPO=/zfs/dicelab/DigitalPlant
echo $REPO is repo

#the results parent directory
TMPDIR=$1
RESULTS=$2
BATCH_NAME=$3

GENERATE_DATA_LOC=$REPO/C_Synthetic_Data_Generator/cmake/generate_data
PARAMETER_LOC=$REPO/bash_scripts/parameters.list
CONN_LOC=$REPO/connection_strings/connection_strings_10000.list
BATCH_LOC=$BATCH_NAME

#Create folder in /local_scratch, timestamped and with the node name
#FOLDER=$(echo $(date +"%B_%d_%H_%M_%S")_$HOSTNAME  | cut -d . -f 1)
FOLDER="local_repo"

#Create final results folder
#mkdir $RESULTS/$FOLDER/
#if [[ ! -d $RESULTS/$FOLDER ]]; then
#	echo "Failed to create final results folder at $RESULTS/$FOLDER"
#	exit 1
#fi

#Create temp logging folder
mkdir $TMPDIR/$FOLDER
if [[ ! -d $TMPDIR/$FOLDER ]]; then
	echo "Failed to create log folder"
	exit 1
fi
#CHANGE DIR to logging folder
cd $TMPDIR/$FOLDER
mkdir logs

#Copy files to /local_scratch
cp $PARAMETER_LOC ./parameters
cp $GENERATE_DATA_LOC ./generate_data
cp $CONN_LOC ./conn.list
cp $BATCH_LOC ./batch
ls -l .
