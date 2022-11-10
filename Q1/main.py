import torch
import os
from dataset import TimeSeries
from model import *
import pandas as pd
import numpy as np
from utils import *
from torch_geometric.loader import DataLoader

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
print(device)
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
hidden_layers=32
lr=0.001
weight_decay=5e-4
num_epochs=100
normalize=False

graph_name="d2"      ###  can be d1,d2,temp
model_path=f"./models/{graph_name}"
os.makedirs(model_path,exist_ok=True)

dataset=TimeSeries("../a3_datasets/d2_small_X.csv","../a3_datasets/d2_adj_mx.csv",normalize)
dataloader = DataLoader(dataset, batch_size=batch_size,shuffle=True, num_workers=0)

splits = np.load("../a3_datasets/d2_graph_splits.npz") 
train_node_ids = convert(splits["train_node_ids"],dataset.mapping) 
# print("here",len(train_node_ids))
val_node_ids = convert(splits["val_node_ids"],dataset.mapping) 
test_node_ids = convert(splits["test_node_ids"],dataset.mapping)



# model = GCN(hidden_channels=hidden_layers).to(device)
model = GAT(hidden_channels=hidden_layers).to(device)
model=model.double()
optimizer = torch.optim.Adam(model.parameters(), lr=lr, weight_decay=weight_decay)
criterion = torch.nn.MSELoss(reduction='sum')

best_loss = -1


train_loss=[]
val_loss=[]
train_mae=[]
val_mae=[]

def train(epoch,plot=False):
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

    print('epoch %d training loss: %.3f' % (epoch + 1, running_loss / (len(dataset)*len(train_node_ids))))
    if (plot):
        train_loss.append(running_loss /(len(dataset)*len(train_node_ids)))
        MAE, MAPE, RMSE, MAE2 = evaluate_metric(model, dataset,train_node_ids)
        train_mae.append(MAE)

def test(test=False,plot=False):         # test=True for test set
    model.eval()
    global best_loss
    global bestmodel
    running_loss = 0.0
    with torch.no_grad():
        out0 = []
        y0 = []
        for i,data in enumerate(dataloader):
            out = model(data.x, data.edge_index, data.edge_weight)
            if (i%10==0 and plot):
                out0.append(out[val_node_ids[0]].item())
                y0.append(data.y[val_node_ids[0]].item())

            if test:        
                tt= get_train_node_ids(test_node_ids,data.y.shape[0]//dataset.num_nodes)
                loss = criterion(out[tt], data.y[tt])
            else:
                tt= get_train_node_ids(val_node_ids,data.y.shape[0]//dataset.num_nodes)
                loss = criterion(out[tt], data.y[tt])
            running_loss += loss.item()
        if test:
            print('epoch %d Test loss: %.3f' % (epoch + 1, running_loss / (len(dataset)*len(test_node_ids))))
        else:
            print('epoch %d Test loss: %.3f' % (epoch + 1, running_loss / (len(dataset)*len(val_node_ids))))
        
        if (epoch%100==0 and not test and plot):
            x=[i for i in range(len(out0))]
            plt.plot(x,out0,label="Model prediction")
            plt.plot(x,y0,label="Actual")    
            plt.draw()
            plt.legend()
            plt.savefig(f"plot_losses/{graph_name}/{epoch}.png")
            plt.clf()
        
        if (not test and plot):
            val_loss.append(running_loss /(len(dataset)*len(val_node_ids)))
            MAE, MAPE, RMSE, MAE2 = evaluate_metric(model, dataset,val_node_ids)
            val_mae.append(MAE)

    if test==False and (best_loss==-1 or running_loss < best_loss):
        best_loss=running_loss
        # Saving our trained model
        torch.save(model.state_dict(), './models/bestval.pth')


if __name__ == '__main__':
    print('Start Training')
    os.makedirs('./models', exist_ok=True)

    # TODO we can use scalar to fit transform the data, also pass that in evaluate metric
    plot=True
    for epoch in range(num_epochs): 
        train(epoch,plot)
        test(plot=plot)      # on validation set    

    print('performing test')
    test(test=True)
    print('Finished Training')

    print("For Training:  ")
    MAE, MAPE, RMSE, MAE2 = evaluate_metric(model, dataset,train_node_ids)
    print("MAE: ", MAE, MAE2)

    model.load_state_dict(torch.load('./models/bestval.pth'))
    print("For Validation:  ")
    MAE, MAPE, RMSE, MAE2 = evaluate_metric(model, dataset,val_node_ids)
    print("MAE: ", MAE, MAE2)
    
    
    print("For Testing:  ")
    MAE, MAPE, RMSE, MAE2 = evaluate_metric(model, dataset,test_node_ids)
    print("MAE: ", MAE, MAE2)

    if (plot):
        epochs=[i for i in range(len(train_loss))]
        # print(epochs)
        plt.plot(epochs,train_loss,label="Train_loss")
        plt.plot(epochs,val_loss,label="Val_loss")    

        plt.xlabel("Epochs")
        plt.ylabel("Loss")
        plt.legend()
        plt.draw()
        plt.savefig(f"plot_losses/{graph_name}/loss.png")
        plt.clf()    

        plt.plot(epochs,train_mae,label="Train_mae")
        plt.plot(epochs,val_mae,label="Val_mae")    

        plt.xlabel("Epochs")
        plt.ylabel("MAE")
        plt.legend()
        plt.draw()
        plt.savefig(f"plot_losses/{graph_name}/MAE.png")
        plt.clf()    

    # Saving our trained model
    torch.save(model.state_dict(), './models/lastmodel.pth')
