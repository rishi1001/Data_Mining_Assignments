import networkx as nx
import pandas as pd
import numpy as np

csv_file = './a3_datasets/d2_X.csv'
df = pd.read_csv(csv_file)
df=df.drop(['Unnamed: 0'], axis=1)

print(df.head())

# convert df to numpy
a= df.to_numpy()
print(a.shape)

# col0 = a[:,0]
# print(col0.shape)

# timestamp +1
# shift a by 1 row
b = np.roll(a, -1, axis=0)
print(a[1])
print(b[0])

# subtract a and b
c = abs(b-a)
c = c[:-1]
print(c.shape)

# get traspose of c
c = c.T

# print plot of c
import matplotlib.pyplot as plt
plt.plot(c)
plt.savefig('d2_stats.png')

# print stats
import sys
with open ('d2_stats2.txt', 'w') as f:
    sys.stdout = f
    print("Mean",np.mean(c))
    print("Std",np.std(c))
    print("Min",np.min(c))
    print("Max",np.max(c))

sys.stdout = sys.__stdout__
print('done')
