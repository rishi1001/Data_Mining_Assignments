import torch
import os
from dataset import TimeSeries
from model import GCN
import pandas as pd
import numpy as np
from utils import *
from torch_geometric.loader import DataLoader

def convert(l,mapping):
  m = {mapping[i]:i for i in range(len(mapping))}
  return [m[str(i)] for i in l]  

def get_train_node_ids(train_node_ids, batch_size):
    train_ids = []
    for i in range(batch_size):
        for x in train_node_ids:
            train_ids.append(x+i*dataset.num_nodes)
    return train_ids

### model-parameters
batch_size=2
hidden_layers=16
lr=0.01
weight_decay=5e-4
num_epochs=1
normalize=True

dataset=TimeSeries("../a3_datasets/d2_small_X.csv","../a3_datasets/d2_adj_mx.csv",normalize)
dataloader = DataLoader(dataset, batch_size=batch_size,shuffle=True, num_workers=0)

splits = np.load("../a3_datasets/d2_graph_splits.npz") 
train_node_ids = convert(splits["train_node_ids"],dataset.mapping) 
# print("here",len(train_node_ids))
val_node_ids = convert(splits["val_node_ids"],dataset.mapping) 
test_node_ids = convert(splits["test_node_ids"],dataset.mapping)



model = GCN(hidden_channels=hidden_layers)
model=model.double()
optimizer = torch.optim.Adam(model.parameters(), lr=lr, weight_decay=weight_decay)
criterion = torch.nn.MSELoss(reduction='sum')

best_loss = -1
bestmodel = None

def train(epoch):
    model.train()

    running_loss = 0.0
    # batch wise training
    for i,data in enumerate(dataloader):
        optimizer.zero_grad()  # Clear gradients.
        #print(data.features)
        out = model(data.x, data.edge_index,data.edge_weight)  
        tt= get_train_node_ids(train_node_ids,data.y.shape[0]//dataset.num_nodes)
        loss = criterion(out[tt], data.y[tt])
        # print(loss)


        loss.backward()
        optimizer.step()
        running_loss += loss.item()
        # if i==100:
        #     break

    print('epoch %d training loss: %.3f' % (epoch + 1, running_loss / len(train_node_ids)))

def test(test=False):         # test=True for test set
    model.eval()
    global best_loss
    global bestmodel
    running_loss = 0.0
    with torch.no_grad():
        out0 = []
        X0 = []
        Y0 = []
        for data in dataloader:
            out = model(data.x, data.edge_index, data.edge_weight)
            if test:        # TODO here also change the node ids dependening on size of y and num_nodes
                loss = criterion(out[test_node_ids], data.y[test_node_ids])
            else:
                loss = criterion(out[val_node_ids], data.y[val_node_ids])
            running_loss += loss.item()
        if test:
            print('epoch %d Test loss: %.3f' % (epoch + 1, running_loss / len(test_node_ids)))
        else:
            print('epoch %d Test loss: %.3f' % (epoch + 1, running_loss / len(val_node_ids)))
        
        if test:
            plot_pred(X0,Y0,out0)
        
    
    if test==False and (best_loss==-1 or running_loss < best_loss):
        best_loss=running_loss
        # Saving our trained model
        torch.save(model.state_dict(), './models/bestval.pth')
        bestmodel = model

if __name__ == '__main__':
    print('Start Training')
    os.makedirs('./models', exist_ok=True)

    # TODO we can use scalar to fit transform the data, also pass that in evaluate metric

    for epoch in range(num_epochs): 
        train(epoch)
        test()      # on validation set    

    print('performing test')
    test(test=True)
    print('Finished Training')

    MAE, MAPE, RMSE, MAE2 = evaluate_metric(bestmodel, dataset)
    print("MAE: ", MAE, MAE2)

    # TODO use the plotting function from utils.py

    # Saving our trained model
    torch.save(model.state_dict(), './models/lastmodel.pth')
