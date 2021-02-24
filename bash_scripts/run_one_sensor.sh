#!/bin/bash

# for testing
echo $@ >> $1/local_repo/`hostname`.out
RESULTS=$2
echo $RESULTS is results on `hostname`
IFS=' ' read -r -a array <<< "$3"
LOG_FOLDER=$1/local_repo
cd $LOG_FOLDER

#Set up spack
. $HOME/spack/share/spack/setup-env.sh
module load gcc/8.2.0
spack load azureiot@LTS-05_2020_Ref01 arch=x86_64
echo $PATH

#cmd="./generate_data ${array[0]} ${array[1]} ${array[2]} ${array[3]} ${array[4]} ${array[5]} ${array[6]} ${array[7]} ${array[8]} --filename ${LOG_FOLDER}/log${array[9]}"

#array[0] -> conn
#array[1] -> number

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
$cmd

#Move files to results, and then clean up
#rm parameters generate_data conn.list
#mv * $RESULTS/$FOLDER
#Catch anything left
#rm -rf $LOG_FOLDER
