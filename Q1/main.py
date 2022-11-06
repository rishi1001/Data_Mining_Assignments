import torch
import os
from dataset import  graph,TimeSeries
from model import GCN
import pandas as pd
import numpy as np
from utils import *
from torch.utils.data import DataLoader


G=graph("../a3_datasets/d1_adj_mx.csv")
print(G.edge_index)
print(G.edge_weight.shape)

# TODO masking for train and test while training & loss function
# TODO add code for validation
def convert(l,mapping):
  m = {mapping[i]:i for i in range(len(mapping))}
  return [m[str(i)] for i in l]  


dataset=TimeSeries("../a3_datasets/d1_X.csv",G.mapping)

splits = np.load("../a3_datasets/d1_graph_splits.npz") 
train_node_ids = convert(splits["train_node_ids"],G.mapping) 
val_node_ids = convert(splits["val_node_ids"],G.mapping) 
test_node_ids = convert(splits["test_node_ids"],G.mapping)
# train_node_ids = [0,1,2,3,4]
# val_node_ids = [0,1,2,3,4] 
# test_node_ids = [0,1,2,3,4]
# # TODO ask if we can use featuers of test nodes also while training?


dataloader = DataLoader(dataset, batch_size=4,shuffle=True, num_workers=0)

model = GCN(hidden_channels=16)
model=model.double()
optimizer = torch.optim.Adam(model.parameters(), lr=0.01, weight_decay=5e-4)
criterion = torch.nn.MSELoss()

best_loss = -1
bestmodel = None

def train(epoch):
    model.train()

    running_loss = 0.0
    # batch wise training
    for i,data in enumerate(dataset):
        optimizer.zero_grad()  # Clear gradients.
        #print(data.features)
        out = model(data['x'], G.edge_index, G.edge_weight)  
        loss = criterion(out[train_node_ids], data['y'][train_node_ids])/len(train_node_ids)

        loss.backward()
        optimizer.step()
        running_loss += loss.item()

    print('epoch %d training loss: %.3f' % (epoch + 1, running_loss / (len(dataset))))

def test(test=False):         # test=True for test set
    model.eval()
    global best_loss
    global bestmodel
    running_loss = 0.0
    with torch.no_grad():
        for data in dataset:
            out = model(data['x'], G.edge_index, G.edge_weight)
            if test:
                loss = criterion(out[test_node_ids], data['y'][test_node_ids])/len(test_node_ids)
            else:
                loss = criterion(out[val_node_ids], data['y'][val_node_ids])/len(val_node_ids)
            running_loss += loss.item()
        print('epoch %d Test loss: %.3f' % (epoch + 1, running_loss / (len(dataset))))
        
    
    if test==False and (best_loss==-1 or running_loss < best_loss):
        best_loss=running_loss
        # Saving our trained model
        torch.save(model.state_dict(), './models/bestval.pth')
        bestmodel = model

if __name__ == '__main__':
    print('Start Training')
    os.makedirs('./models', exist_ok=True)

    # TODO we can use scalar to fit transform the data, also pass that in evaluate metric


    num_epochs = 100
    for epoch in range(num_epochs):  # loop over the dataset multiple times
        # print('epoch ', epoch + 1)
        train(epoch)
        test()      # on validation set    

    print('performing test')
    test(test=True)
    print('Finished Training')

    MAE, MAPE, RMSE = evaluate_metric(bestmodel, dataset, G)
    print("MAE: ", MAE)

    # TODO use the plotting function from utils.py

    # Saving our trained model
    torch.save(model.state_dict(), './models/lastmodel.pth')
