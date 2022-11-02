import pandas as pd
import numpy as np
import scipy.sparse as sp

df = pd.read_hdf('./stgcn_wave/data/metr-la.h5')
print(df.head())

df = pd.read_csv('./a3_datasets/d1_X.csv')
df=df.drop(['Unnamed: 0'], axis=1)
df = df.iloc[:,1:]
print(df.head())

# def get_adjacency_matrix(distance_df, sensor_ids, normalized_k=0.1):
#     """
#     :param distance_df: data frame with three columns: [from, to, distance].
#     :param sensor_ids: list of sensor ids.
#     :param normalized_k: entries that become lower than normalized_k after normalization are set to zero for sparsity.
#     :return: adjacency matrix
#     """
#     num_sensors = len(sensor_ids)
#     dist_mx = np.zeros((num_sensors, num_sensors), dtype=np.float32)
#     dist_mx[:] = np.inf
#     # Builds sensor id to index map.
#     sensor_id_to_ind = {}
#     for i, sensor_id in enumerate(sensor_ids):
#         sensor_id_to_ind[sensor_id] = i
#     # Fills cells in the matrix with distances.
#     for row in distance_df.values:
#         if row[0] not in sensor_id_to_ind or row[1] not in sensor_id_to_ind:
#             continue
#         dist_mx[sensor_id_to_ind[row[0]], sensor_id_to_ind[row[1]]] = row[2]

#     # Calculates the standard deviation as theta.
#     distances = dist_mx[~np.isinf(dist_mx)].flatten()
#     std = distances.std()
#     adj_mx = np.exp(-np.square(dist_mx / std))
#     # Make the adjacent matrix symmetric by taking the max.
#     # adj_mx = np.maximum.reduce([adj_mx, adj_mx.T])

#     # Sets entries that lower than a threshold, i.e., k, to zero for sparsity.
#     adj_mx[adj_mx < normalized_k] = 0
#     return adj_mx

# with open('./stgcn_wave/data/sensor_graph/graph_sensor_locations.csv') as f:
#     sensor_ids = f.read().strip().split(",")

# distance_df = pd.read_csv('./stgcn_wave/data/sensor_graph/distances_la_2012.csv', dtype={"from": "str", "to": "str"})

# adj_mx = get_adjacency_matrix(distance_df, sensor_ids)
# sp_mx = sp.coo_matrix(adj_mx)
# print(sp_mx)
# print(type(adj_mx))
# print(adj_mx.shape)

# df = pd.read_csv('./a3_datasets/d1_adj_mx.csv')
# print(df.head())

# adj_mx = np.loadtxt('./a3_datasets/d1_adj_mx.csv', delimiter=',', dtype=str)
# print(adj_mx)

df = pd.read_csv('./a3_datasets/d1_adj_mx.csv',index_col=0)
cols=df.columns
df.columns=[i for i in range(len(cols))]
df=df.reset_index(drop=True)
print(df.shape)

# sp_mx = sp.coo_matrix(df)
# print(sp_mx)