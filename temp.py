import networkx as nx
import pandas as pd

df = pd.read_csv('./a3_datasets/d2_adj_mx.csv',index_col=0)
cols=df.columns
# df.columns=[i for i in range(len(cols))]
print(df.head)
print(df.shape)
G = nx.from_pandas_adjacency(df)