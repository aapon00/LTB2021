#!/bin/bash









#check argument

if [ $# -ne 1 ]; then
        printf "./$0 <log file directory>\n" >&2
        exit 1
fi

echo $1 #prints the first command line argument

LOGDIR=$1

#OUTPUT="all.csv" #Changed for three node experiment (old: ../all.csv)

if [ ! -d $LOGDIR  ]; then
        printf "$1 is not a directory\n" >&2
        exit 1
fi

#Switch pwd to LOGDIR
cd $LOGDIR

#loop through tiers
for tier in *.list_run
do	
	cd $tier
	#echo $tier
	
	#loop through parameters
	for param in parameter*
	do
		cd $param
		#echo $param
		printf "\n\n`pwd`\n\n"
		#loop through trials
		for trial in trial*
		do
			#echo $trial

			echo "Working on $tier $param $trial"
			/zfs/dicelab/DigitalPlant/bash_scripts/analyze/concat_csv.sh $trial ./all.csv
			mv all.csv ./$trial/all.csv
		done
		cd ../
	done
	cd ../
done
	
#clean up
exit 0

