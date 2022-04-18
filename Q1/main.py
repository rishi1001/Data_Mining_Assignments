print("hehehe")
import torch
import os
# from dataset import data_point, graph
from model import GCN
import pandas as pd
import numpy as np

G=graph("../a3_datasets/d2_adj_mx.csv")
print(G.edge_list)
print(G.edge_weight)


def read_data():
  dataset=[]
  path='../a3_datasets/d2_X.csv'

  df = pd.read_csv(path)
  df=df.drop(['Unnamed: 0'], axis=1)
  for i in range(len(df)-1):
    # print("dddddddd")
    # print(i)
    d=df.loc[i:i+1,:]
    d=d.reset_index(drop=True)
    dataset.append(data_point(d,G.mapping))

  return dataset


# TODO masking for train and test while training & loss function
# TODO add code for validation
def convert(l,mapping):
  m = {mapping[i]:i for i in range(len(mapping))}
  return [m[str(i)] for i in l]  


dataset = read_data()
splits = np.load("../a3_datasets/d2_graph_splits.npz") 
train_node_ids = convert(splits["train_node_ids"],G.mapping) 
val_node_ids = convert(splits["val_node_ids"],G.mapping) 
test_node_ids = convert(splits["test_node_ids"],G.mapping)

# TODO generate train,val,test set


model = GCN(hidden_channels=16)
optimizer = torch.optim.Adam(model.parameters(), lr=0.0001)
criterion = torch.nn.MSELoss()          # TODO check this

best_loss = -1
bestmodel = None

def train(epoch):
    model.train()

    running_loss = 0.0
    # batch wise training
    for data in dataset:
        optimizer.zero_grad()  # Clear gradients.
        print(data.features)
    #   print(data.features.shape)
        out = model(data.features, G.edge_index, G.edge_weight).reshape(-1)   
    #   print(out.shape)
        loss = criterion(out, data.y[train_node_ids])
    #   make_dot(out).render("graph.dot", format="png")
    #   print(out)
        print("dataaa y")
        print(data.y)
        print("out", out)
        loss.backward()
        optimizer.step()
        print(loss)
        running_loss += loss.item()

    print('epoch %d training loss: %.3f' % (epoch + 1, running_loss / (len(dataset))))

def test(test_set,test=False):         # test=True for test set
    model.eval()
    global best_loss
    global bestmodel
    running_loss = 0.0
    with torch.no_grad():
        for data in test_set:
            out = model(data.features, G.edge_index, G.edge_weight).reshape(-1)
            if test:
                loss = criterion(out, data.y[data.test_node_ids])
            else:
                loss = criterion(out, data.y[data.val_node_ids])
            loss = criterion(out, data.y)
            running_loss += loss.item()
            print(loss)
            print("dataaa y")
            print(data.y)
            print("out", out)
            print("test loss", loss)
            print("test acc", loss)
    
    if test==False and (best_loss==-1 or running_loss < best_loss):
        best_loss=running_loss
        # Saving our trained model
        torch.save(model.state_dict(), './models/bestval.pth')
        bestmodel = model

if __name__ == '__main__':
    print('Start Training')
    os.makedirs('./models', exist_ok=True)

    num_epochs = 100
    for epoch in range(num_epochs):  # loop over the dataset multiple times
        print('epoch ', epoch + 1)
        train(epoch)
        test(val_node_ids)      # on validation set      

    print('performing test')
    print('Finished Training')

    # Saving our trained model
    torch.save(model.state_dict(), './models/lastmodel.pth')