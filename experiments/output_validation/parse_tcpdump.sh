#!/bin/bash

# NOTE: expecting normal (not verbose) TCPdump input such as from 'tcpdump -i eth0 port 8883'
infile=$1

echo "msg_ts,msg_size,ack_ts,ack_size"

# remove blank lines, data between IP and the last comma, then split records on sending a TCP ack to the receiver's ack
cat $infile \
 | sed '/^$/d' 
 | sed 's/IP.*,//'              \
 | awk                          \
     -v RS='length 0\n'           \
     -v OFS=','                   \
     '{print $1, $3, $4, $6}'
