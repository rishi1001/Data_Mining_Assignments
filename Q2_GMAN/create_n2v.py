import networkx as nx
from node2vec import Node2Vec
import pandas as pd

graph_file = '../a3_datasets/conv_d1_adj_mx.csv'         # experiment with conv vs not conv
dimensions_req = 64
SE_file = '../a3_datasets/conv_d1_SE.txt'


df = pd.read_csv(graph_file,index_col=0)
cols=df.columns
df.columns=[i for i in range(len(cols))]
df=df.reset_index(drop=True)

# Create a graph
graph = nx.from_pandas_adjacency(df)

n2v = Node2Vec(graph, dimensions=dimensions_req, walk_length=30, num_walks=200, workers=4)  # Use temp_folder for big graphs

# Embed nodes
model = n2v.fit(window=10, min_count=1, batch_words=4)

# Save embeddings for later use
model.wv.save_word2vec_format(SE_file)