import torch
from torch.nn import Linear
import torch.nn.functional as F
from torch_geometric.nn import GCNConv
from datasets import data_point
import os
from torchviz import make_dot
from .model import GCN
from .dataset import data_point

def read_data():
    dataset=[]
    directory='../dataset2'
    for filename in os.listdir(directory): 
        dataset.append(data_point(directory+'/'+filename))
    return dataset


# TODO masking for train and test while training & loss function
# TODO add code for validation

dataset = read_data()
dataset_size=len(dataset)
train_set = dataset[0:int(0.9*dataset_size)]
test_set= dataset[int(0.9*dataset_size):]
# TODO generate train,val,test set


model = GCN(hidden_channels=16)
optimizer = torch.optim.Adam(model.parameters(), lr=0.0001)
criterion = torch.nn.MSELoss()          # TODO check this

def train():
      model.train()
      # batch wise training
      for data in dataset:
          optimizer.zero_grad()  # Clear gradients.
          print(data.features)
        #   print(data.features.shape)
          out = model(data.features, data.edge_index, data.edge_weight).reshape(-1)   
        #   print(out.shape)
          loss = criterion(out, data.y)
        #   make_dot(out).render("graph.dot", format="png")
        #   print(out)
          print("dataaa y")
          print(data.y)
          print("out", out)
          loss.backward()
          optimizer.step()
          print(loss)
      return loss

def test():
      model.eval()
      out = model(data.x, data.edge_index)
      pred = out.argmax(dim=1)  # Use the class with highest probability.
      test_correct = pred[data.test_mask] == data.y[data.test_mask]  # Check against ground-truth labels.
      test_acc = int(test_correct.sum()) / int(data.test_mask.sum())  # Derive ratio of correct predictions.
      return test_acc


for epoch in range(30):
    loss = train()
    # print(f'Epoch: {epoch:03d}, Loss: {loss:.4f}')