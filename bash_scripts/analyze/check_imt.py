#python3 
#DICELAB ENV

import pandas as pd
import numpy as np
from scipy import stats
import sys
from math import ceil

if (len(sys.argv) != 2):
    print("Usage: python check_imt.py <file path>")
    exit(1)

FILE = sys.argv[1]

#Creating Dataframe
df = pd.read_csv(FILE, na_filter= False)
# devices = df.groupby('deviceNum')
# print(devices)
df = df.sort_values(['deviceNum','Message ID'])

df['IMT'] =  np.where(df['Message ID']== 0, np.nan, df['Send Time']- df['Send Time'].shift(1))
#df['Receive Time'] = pd.to_datetime(df['Receive Time'], unit='ms')

#df['Send Time'] = pd.to_datetime(df['Send Time'], unit='ms')
#df['Latency'] = (df['Receive Time'] - df['Send Time']).dt.total_seconds() * 1000


if (len(df[df['IMT'] < 0]) != 0):
	print("ERROR: IMT < 0")
	print(df[df['IMT'] < 0])

PERCENT_TO_DROP = .05

num_messages = df.groupby('deviceNum')['deviceNum'].size()[0]
messages_to_drop = int(ceil(num_messages*PERCENT_TO_DROP))

print("Original dataframe contained {} messages, with each device sending {}\nDropping {}%, or {} messages from each side".format(len(df), num_messages, PERCENT_TO_DROP,messages_to_drop))
df.drop(df[df['Message ID'] >= (num_messages - messages_to_drop)].index, inplace=True)
df.drop(df[df['Message ID'] < messages_to_drop].index, inplace=True) #0 1 2 3 4


print(df['IMT'].describe())

