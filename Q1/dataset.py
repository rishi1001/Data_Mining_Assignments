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
    def __init__(self,csv_file,graph_file,normalize) -> None:
        super().__init__()
        df = pd.read_csv(graph_file,index_col=0)
        cols=df.columns
        df.columns=[i for i in range(len(cols))]
        df=df.reset_index(drop=True)
        
        t=torch.tensor(df.values)
        self.edge_index , self.edge_weight = dense_to_sparse(t)
        self.edge_weight=self.edge_weight
        self.num_nodes = len(cols)
        
        if normalize:
            self.edge_weight=self.edge_weight/torch.max(self.edge_weight)
            self.edge_weight=self.edge_weight.double()
        #self.mapping ={i:cols[i] for i in range(len(cols))}                    
        self.mapping=cols
        
        df = pd.read_csv(csv_file)
        df=df.drop(['Unnamed: 0'], axis=1)
        i = np.arange(df.shape[0]-1)
        i_1 = i+1
        pairs = np.array([i,i_1]).T.ravel()
        self.dataset = df.loc[pairs,cols].to_numpy().reshape(df.shape[0]-1, 2, len(cols),1)

    def __len__(self):
        return len(self.dataset)
    
    def __getitem__(self, idx):
        # return {'x':torch.tensor(self.dataset[idx][0]).type(torch.DoubleTensor),'y': torch.tensor(self.dataset[idx][1]).type(torch.DoubleTensor),'edge_weight':self.edge_weight,'edge_index':self.edge_index} 
        x=torch.tensor(self.dataset[idx][0]).type(torch.DoubleTensor)
        y=torch.tensor(self.dataset[idx][1]).type(torch.DoubleTensor)
        d= Data(x=x,edge_index=self.edge_index,edge_attr=self.edge_weight,y=y)
        return d