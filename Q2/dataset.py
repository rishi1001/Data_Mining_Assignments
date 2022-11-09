import networkx as nx
import scipy.sparse as sp
import numpy as np
import torch
import pandas as pd
import torch
from torch_geometric.utils import dense_to_sparse
# from torch.utils.data import Dataset
from torch_geometric.data import Data
from torch_geometric.data import Dataset
       
class TimeSeries(Dataset):
    def __init__(self,csv_file,graph_file,num_timesteps_in: int = 12, num_timesteps_out: int = 12) -> None:
        super().__init__()
        df = pd.read_csv(graph_file,index_col=0)
        cols=df.columns
        df.columns=[i for i in range(len(cols))]
        df=df.reset_index(drop=True)
        
        t=torch.tensor(df.values)
        self.edge_index , self.edge_weight = dense_to_sparse(t)
        self.edge_weight=self.edge_weight
        self.num_nodes = len(cols)

        #self.mapping ={i:cols[i] for i in range(len(cols))}                    
        self.mapping=cols
        
        df = pd.read_csv(csv_file)
        df=df.drop(['Unnamed: 0'], axis=1)
        # TODO normalise featuers
        self.X = df.to_numpy()
        self._generate_task(num_timesteps_in, num_timesteps_out)
        self.dataset = list(zip(self.features, self.targets))

        # i = np.arange(df.shape[0]-1)
        # i_1 = i+1
        # pairs = np.array([i,i_1]).T.ravel()
        # self.dataset = df.loc[pairs,cols].to_numpy().reshape(df.shape[0]-1, 2, len(cols),1)


    def _generate_task(self, num_timesteps_in: int = 12, num_timesteps_out: int = 12):
        indices = [
            (i, i + (num_timesteps_in + num_timesteps_out))
            for i in range(self.X.shape[2] - (num_timesteps_in + num_timesteps_out) + 1)
        ]

        # Generate observations
        features, target = [], []
        for i, j in indices:
            features.append((self.X[:, :, i : i + num_timesteps_in]).numpy())
            target.append((self.X[:, 0, i + num_timesteps_in : j]).numpy())

        self.features = features
        self.targets = target

    def __len__(self):
        return len(self.dataset)
    
    def __getitem__(self, idx):
        # return {'x':torch.tensor(self.dataset[idx][0]).type(torch.DoubleTensor),'y': torch.tensor(self.dataset[idx][1]).type(torch.DoubleTensor),'edge_weight':self.edge_weight,'edge_index':self.edge_index} 
        x=torch.tensor(self.dataset[idx][0]).type(torch.DoubleTensor)
        y=torch.tensor(self.dataset[idx][1]).type(torch.DoubleTensor)
        d= Data(x=x,edge_index=self.edge_index,edge_attr=self.edge_weight,y=y)
        return d


import os
import numpy as np
import torch
from torch_geometric.utils import dense_to_sparse
from ..signal import StaticGraphTemporalSignalBatch


class TimeSeries2(object):          # TODO in argument - will it be object or Dataset?

    def __init__(self,csv_file,graph_file ):
        # super(TimeSeries2, self).__init__()
        A = np.load(graph_file)
        X = np.load(csv_file).transpose(
            (1, 2, 0)
        )
        X = X.astype(np.float32)

        # Normalise as in DCRNN paper (via Z-Score Method)
        means = np.mean(X, axis=(0, 2))
        X = X - means.reshape(1, -1, 1)
        stds = np.std(X, axis=(0, 2))
        X = X / stds.reshape(1, -1, 1)

        self.A = torch.from_numpy(A)
        self.X = torch.from_numpy(X)

        

    def _get_edges_and_weights(self):
        edge_indices, values = dense_to_sparse(self.A)
        edge_indices = edge_indices.numpy()
        values = values.numpy()
        self.edges = edge_indices
        self.edge_weights = values

    def _generate_task(self, num_timesteps_in: int = 12, num_timesteps_out: int = 12):
        indices = [
            (i, i + (num_timesteps_in + num_timesteps_out))
            for i in range(self.X.shape[2] - (num_timesteps_in + num_timesteps_out) + 1)
        ]

        # Generate observations
        features, target = [], []
        for i, j in indices:
            features.append((self.X[:, :, i : i + num_timesteps_in]).numpy())
            target.append((self.X[:, 0, i + num_timesteps_in : j]).numpy())

        self.features = features
        self.targets = target

    def get_dataset(
        self, num_timesteps_in: int = 12, num_timesteps_out: int = 12) -> StaticGraphTemporalSignalBatch:
        self._get_edges_and_weights()
        self._generate_task(num_timesteps_in, num_timesteps_out)
        dataset = StaticGraphTemporalSignalBatch(self.edges, self.edge_weights, self.features, self.targets, 32)    # 32 is the batch size

        return dataset

    # TODO: Implement __getitem__ and __len__ methods
    # def __len__(self):
    #     return len(self.dataset)
    
    # def __getitem__(self, idx):
    #     # return {'x':torch.tensor(self.dataset[idx][0]).type(torch.DoubleTensor),'y': torch.tensor(self.dataset[idx][1]).type(torch.DoubleTensor),'edge_weight':self.edge_weight,'edge_index':self.edge_index} 
    #     x=torch.tensor(self.dataset[idx][0]).type(torch.DoubleTensor)
    #     y=torch.tensor(self.dataset[idx][1]).type(torch.DoubleTensor)
    #     d= Data(x=x,edge_index=self.edge_index,edge_attr=self.edge_weight,y=y)
    #     return d