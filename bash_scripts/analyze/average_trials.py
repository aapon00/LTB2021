from os import path
from glob import glob
import pandas as pd
import sys
import numpy as np
from math import ceil

if (len(sys.argv) != 3):
    print("Usage: python average_trials.py <log path> <output>")
    exit(1)

log_folder = sys.argv[1]
output_file = sys.argv[2]

trials = glob(path.join(log_folder, "trial*", "all.csv"))

print('Found {} trials'.format(len(trials)))


dataframes = []
for trial in trials:
    df = pd.read_csv(trial, index_col=['Message ID','deviceNum'])
    df['Status'] = df['Status'].astype(int)
    df['Receive Time'] = pd.to_datetime(df['Receive Time'], unit='ms')
    df['Send Time'] = pd.to_datetime(df['Send Time'], unit='ms')
    df['Latency'] = (df['Receive Time'] - df['Send Time']
                     ).dt.total_seconds() * 1000
    # NOTE possible to do some sort of check on actual IMT
    dataframes.append(df)

# Confrim all dataframes are the same size
dataframe_sizes = {}
numberMessages = len(dataframes[0])
for frame in dataframes:
    dataframe_sizes[len(frame)] = 0
if(len(dataframe_sizes) != 1):
    print("FATAL: Dataframes are uneven")
    exit(1)


average_data_frame = dataframes[0][['Latency', 'Status']].copy()
average_data_frame['Latency'] = 0.0
average_data_frame['Status'] = 0


# print(dataframes[0]['Message ID'])
print(average_data_frame)
# average_data_frame[]

for index,row in average_data_frame.iterrows():
    avgLatency = 0
    status = 1
    for frame in dataframes:
        
        avgLatency += ( frame.at[index, 'Latency'] / len(dataframes))
        # print(str(index) + ": " + str(frame.at[index, 'Latency']), end =" ") 
        status = status & frame.at[index, 'Status'] 
    average_data_frame.at[index, 'Latency'] = avgLatency
    average_data_frame.at[index, 'Status'] = int(status)
    # exit(1)


average_data_frame.to_csv(output_file)
