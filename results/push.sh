#!/bin/bash

for dir in $(ls) 
do
echo $dir
git add $dir
git commit -m "adding results: "$dir
git push origin main
RES=$?
if [[ ! ( $RES -eq 0 ) ]]
	then 
		exit 1
	fi
done
