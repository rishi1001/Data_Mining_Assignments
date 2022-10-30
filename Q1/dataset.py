import networkx as nx
import scipy.sparse as sp
import numpy as np
import torch
from utils import *


class data_point():
    def __init__(self,folder):
        self.folder=folder
        self.num_features=0
        self.features=None
        self.y=None
        self.edge_index=None
        self.edge_weight =None
        self.graph=None
        self.train_mask=None
        self.val_mask=None
        self.test_mask=None
        self.generate_edge_index()
        self.generate_features()
        self.generate_y()

    def generate_masks(self):
        # TODO : generate masks using input files
        pass
    
    
    def generate_edge_index(self):
        
        self.graph=nx.read_edgelist(self.folder+'/graph_100_1.txt')
        mapping= {i:int(i) for i in self.graph.nodes()}
        self.graph=nx.relabel_nodes(self.graph,mapping)
        self.edge_index = torch.tensor(list(self.graph.edges())).t()
        # TODO : generate edge weight
        pass

    
    def generate_features(self):
        self.num_features=1
        # TODO : generate features using input files
        pass

    def generate_y(self):
        # TODO : generate y using input files
        pass
        
        
    