#!/bin/bash

echo "RUNNING PER NODE CLEANUP"

#the results parent directory
TMPDIR=$1
echo $TMPDIR is TMPDIR
RESULTS=$2
echo $RESULTS is RESULTS
NODE=$3

cd $TMPDIR/local_repo
ls -l .

mkdir $RESULTS/$NODE


echo "Moving files"
#Move files to results, and then clean up
rm parameters generate_data conn.list
mv * $RESULTS/$NODE
#Catch anything left
rm -rf $LOG_FOLDER
