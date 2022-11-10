import torch
from torch.nn import Linear
import torch.nn.functional as F
from torch_geometric.nn import GCNConv


class GCN(torch.nn.Module):
    def __init__(self, hidden_channels):
        super().__init__()
        torch.manual_seed(1234567)
        self.batch_norm0 = torch.nn.BatchNorm1d(1)
        self.conv1 = GCNConv(1, hidden_channels)            # input features = 1
        self.batch_norm1 = torch.nn.BatchNorm1d(hidden_channels)
        self.conv2 = GCNConv(hidden_channels, 2*hidden_channels)
        # mlp
        self.lin1 = Linear(2*hidden_channels, hidden_channels)
        self.lin2 = Linear(hidden_channels, 1)

    def forward(self, x, edge_index,edge_weight):       # TODO add batchnorm
        # print(x.shape)
        # print(x)
        x = self.batch_norm0(x)
        # print(x)
        x = self.conv1(x, edge_index,edge_weight)
        x = self.batch_norm1(x)
        x = x.relu()
        # x = F.dropout(x, p=0.5, training=self.training)
        x = self.conv2(x, edge_index,edge_weight)           # node embeddings

        x = x.relu()
        #mlp
        x = self.lin1(x)
        x = x.relu()
        # x_1 = F.dropout(x_1, p=0.5, training=self.training)
        x = self.lin2(x)
        return x


class GAT(torch.nn.Module):
    def __init__(self, hidden_channels,heads=8):
        super().__init__()
        torch.manual_seed(1234567)
        self.batch_norm0 = torch.nn.BatchNorm1d(1)
        self.conv1 = GAT(1, hidden_channels,heads=heads)            # input features = 1
        self.batch_norm1 = torch.nn.BatchNorm1d(hidden_channels*heads)
        self.conv2 = GAT(hidden_channels*heads, 2*hidden_channels,heads=1)
        # mlp
        self.lin1 = Linear(2*hidden_channels, hidden_channels)
        self.lin2 = Linear(hidden_channels, 1)

    def forward(self, x, edge_index,edge_weight):       # TODO add batchnorm
        # print(x.shape)
        # print(x)
        x = self.batch_norm0(x)
        # print(x)
        x = self.conv1(x, edge_index,edge_weight)
        x = self.batch_norm1(x)
        x = x.relu()
        # x = F.dropout(x, p=0.5, training=self.training)
        x = self.conv2(x, edge_index,edge_weight)           # node embeddings

        x = x.relu()
        #mlp
        x = self.lin1(x)
        x = x.relu()
        # x_1 = F.dropout(x_1, p=0.5, training=self.training)
        x = self.lin2(x)
        return x
