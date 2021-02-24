#!/usr/bin/env python3

import re
import os
import csv
from datetime import datetime
import time

def parse_args():
	import argparse

	parser = argparse.ArgumentParser(description="Parse TCPdump text output of IoTHub session into message/reply times and sizes.")
	parser.add_argument('infile', help="Text output of TCPdump (non-verbose).")
	parser.add_argument('--out_path', help="Target directory for output file(s).", default='.')
	parser.add_argument('--iothub_port', help="Remote port of IoTHub", default="8883")

	return parser.parse_args()

def main():
	args = parse_args()

	# 15:51:48.285017 IP 10.0.1.201.57682 > 52.177.196.50.8883: Flags [P.], seq 1480597953:1480599030, ack 2051355523, win 273, options [nop,nop,TS val 19243846 ecr 1689126032], length 1077
	# 15:51:48.298519 IP 52.177.196.50.8883 > 10.0.1.201.57682: Flags [P.], seq 1:70, ack 1077, win 1029, options [nop,nop,TS val 1689127033 ecr 19243846], length 69
	# 15:51:48.298542 IP 10.0.1.201.57682 > 52.177.196.50.8883: Flags [.], ack 70, win 273, options [nop,nop,TS val 19243849 ecr 1689127033], length 0
	# 15:51:49.285089 IP 10.0.1.201.57682 > 52.177.196.50.8883: Flags [P.], seq 1077:2138, ack 70, win 273, options [nop,nop,TS val 19244096 ecr 1689127033], length 1061
	# 15:51:49.297389 IP 52.177.196.50.8883 > 10.0.1.201.57682: Flags [P.], seq 70:139, ack 2138, win 1025, options [nop,nop,TS val 1689128030 ecr 19244096], length 69
	# 15:51:49.297404 IP 10.0.1.201.57682 > 52.177.196.50.8883: Flags [.], ack 139, win 273, options [nop,nop,TS val 19244099 ecr 1689128030], length 0

	# read each line and parse into a timestamp, send port, recv port, sequence numbers, and length
	expr = re.compile('(?P<ts>[\d:.]+) IP [^ ]*\.(?P<sender>\d+) > [^ ]*\.(?P<recver>\d+): Flags \[[^\]]*\], seq (?P<seq_from>\d+):(?P<seq_to>\d+).*, length (?P<len>\d+)')

	with open(args.infile, 'r') as f:
		packets = [m.groupdict() for m in expr.finditer(f.read())]

	# group into connections (in case of multiple sensors)
	connections = {}
	for p in packets:
		# fix up types
		p['ts_s'] = sum(float(x) * 60 ** i for i,x in enumerate(reversed(p['ts'].split(":"))))
		p['seq_from'] = int(p['seq_from'])
		p['seq_to'] = int(p['seq_to'])
		p['len'] = int(p['len'])

		# label the connection by the port that is not the iothub_port
		conn_id = p['sender'] if p['recver'] == args.iothub_port else p['recver']
		connections.setdefault(conn_id, {'packets':[]})['packets'].append(p)

	for conn in connections.values():

		# fix initial sequence number for each connection, TCPdump shows the actual seq in the first packet and then each following packet shows relative sequence numbers
		conn['packets'][0]['seq_from'] = 0
		conn['packets'][0]['seq_to'] = conn['packets'][0]['len']

		# TODO: group multi-segment packets

		# split into outgoing messages and responses, sort by sequence number, then zip them together
		messages = [p for p in conn['packets'] if p['recver'] == args.iothub_port]
		replies  = [p for p in conn['packets'] if p['sender'] == args.iothub_port]
		zipped = list(zip(
			sorted(messages, key=lambda x: x['seq_from']),
			sorted(replies,  key=lambda x: x['seq_from'])
		))

		conn['messages'] = []

		# group into dicts for convenience
		for i in range(len(zipped)):
			m = zipped[i]

			conn['messages'].append({
				'msg_ts':      m[0]['ts'],
				'ack_ts':      m[1]['ts'],
				'relative_ts': m[0]['ts_s'] - zipped[0][0]['ts_s'],
				'gap':         m[0]['ts_s'] - zipped[i-1][0]['ts_s'] if i > 0 else 0.0,
				'latency':     m[1]['ts_s'] - m[0]['ts_s'],
				'len':         m[0]['len']
			})

	# print results
	if not os.path.exists(args.out_path):
		os.makedirs(args.out_path) # creates intermediate if they don't exist

	base = os.path.splitext(os.path.basename(args.infile))[0]

	fieldnames = ['msg_ts', 'ack_ts', 'relative_ts', 'gap', 'latency', 'len']

	for port, conn in connections.items():
		outfile = os.path.join(args.out_path, '%s.%s.csv' % (base, port))

		with open(outfile, 'w', newline='') as f:
			writer = csv.DictWriter(f, fieldnames=fieldnames)
			writer.writeheader()

			for message in conn['messages']:
				writer.writerow(message)


if __name__ == '__main__':
	main()
