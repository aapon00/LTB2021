import pandas as pd
import numpy as np

FOLDER="experiments/size_validation/512_latencies.csv"

df = pd.read_csv(FOLDER, na_filter= False)

# devices = df.groupby('deviceNum')
# print(devices)
df['IMT'] =  np.where(df['Message ID']== 0, np.nan, df['Send Time']- df['Send Time'].shift(1))


df['Receive Time'] = pd.to_datetime(df['Receive Time'], unit='ms')
df['Send Time'] = pd.to_datetime(df['Send Time'], unit='ms')
df['Latency'] = (df['Receive Time'] - df['Send Time']).dt.total_seconds() * 1000

df.drop(df[df['Message ID'] < 30].index, inplace=True)

print(FOLDER)
print(df['Latency'].describe())
