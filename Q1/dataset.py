import networkx as nx
import scipy.sparse as sp
import numpy as np
import torch
import pandas as pd
import torch
from torch_geometric.utils import dense_to_sparse
from torch.utils.data import Dataset
class graph():

    def __init__ (self,path):
        self.edge_index=None
        self.edge_weight=None
        self.mapping=None    ## index to node_id
        self.read_graph(path)

    def read_graph(self,path):
        df = pd.read_csv(path,index_col=0)
        cols=df.columns
        df.columns=[i for i in range(len(cols))]
        df=df.reset_index(drop=True)
        
        t=torch.tensor(df.values)
        self.edge_index , self.edge_weight = dense_to_sparse(t)
        self.normalise()
        #self.mapping ={i:cols[i] for i in range(len(cols))}                    
        self.mapping=cols

    def normalise(self):
        self.edge_weight=self.edge_weight/torch.max(self.edge_weight)
        self.edge_weight=self.edge_weight.double()

class data_point():

    def __init__(self,data,mapping):
        self.features=None
        self.y=None
        self.num_features=1
        self.generate_features(data,mapping)
        self.generate_y(data,mapping)
    
    
    def generate_features(self,data,mapping):
        self.num_features=1
        # TODO check do wee nedd [3] or [3,1]
        l=[[data.iloc[0][mapping[i]]] for i in range(len(mapping))]
        self.features=torch.tensor(l).double()

    def generate_y(self,data,mapping):
        # TODO check do wee nedd [3] or [3,1]
        l=[[data.iloc[1][mapping[i]]] for i in range(len(mapping))]
        self.y=torch.tensor(l).double()
        
        
    
class TimeSeries(Dataset):
    def __init__(self,csv_file,mapping) -> None:
        super().__init__()
        df = pd.read_csv(csv_file)
        df=df.drop(['Unnamed: 0'], axis=1)
        i = np.arange(df.shape[0]-1)
        i_1 = i+1
        pairs = np.array([i,i_1]).T.ravel()
        self.dataset = df.loc[pairs,mapping].to_numpy().reshape(df.shape[0]-1, 2, len(mapping),1)

    def __len__(self):
        return len(self.dataset)
    
    def __getitem__(self, idx):
        return {'x':torch.tensor(self.dataset[idx][0]).type(torch.DoubleTensor),'y': torch.tensor(self.dataset[idx][1]).type(torch.DoubleTensor)} 
    
