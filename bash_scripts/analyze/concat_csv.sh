#!/bin/bash

#USAGE: ./<script> <dir_with_csv_files>

#check argument
if [ $# -lt 1 ] || [ $# -ge 3 ]; then
	printf "./$0 <log file directory> <optional:output location>\n" >&2
	exit 1
fi

#if [ $# -eq 2 ]; then
#	OUTPUT=$2
#else
#	OUTPUT="../all.csv" #Changed for three node experiment (old: ../all.csv)
#fi


OUTPUT="../all.csv" #Changed for three node experiment (old: ../all.csv)

LOGDIR=$1


#check if provided dir is actually a dir
if [ ! -d $LOGDIR  ]; then
	printf "$1 is not a directory\n" >&2
	exit 1
fi

#Switch pwd to LOGDIR
cd $LOGDIR

if [ -f $OUTPUT ]; then
	echo "$OUTPUT already exists. Deleting" >&2
	rm  $OUTPUT
fi

#Grab a file to get headers
base_log=$(ls *.csv | head -n 1)
printf "Using $base_log as default\n" >&2


#Add more headers
HEADERS=$(head -n 1 $base_log) 
printf "$HEADERS,deviceNum\n">> $OUTPUT


for csv in *.csv
do
	echo "$csv" >&2
	deviceNum=$(echo $csv | sed -r 's/[^0-9]*//g')
#	printf "$csv -> $deviceNum\n" >&2
	sed -i '1d' $csv #FIXME 
	#sort -o $csv $csv
	while IFS="" read -r line || [ -n "$line" ]
        do
                #Ignore the headers, only copy the data
        	printf "$line,$deviceNum\n" >> $OUTPUT
        
        done < "$csv";
	sed -i '1s/^/Message ID,Send Time,Receive Time,Status\n/i' $csv
done


#clean up
cd ..
exit 0
