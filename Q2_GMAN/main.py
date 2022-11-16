import torch
import os
from dataset import TimeSeries
from model import GMAN
import pandas as pd
import numpy as np
from utils import *
from torch.utils.data import DataLoader
import sys
from tqdm import tqdm

def convert(l,mapping):
  m = {mapping[i]:i for i in range(len(mapping))}
  return [m[str(i)] for i in l]  

def get_train_node_ids(train_node_ids, batch_size):
    train_ids = []
    for i in range(batch_size):
        for x in train_node_ids:
            train_ids.append(x+i*dataset.num_nodes)
    # print(train_ids)
    return train_ids

## device setting
gpu=2
device = torch.device(f'cuda:{gpu}' if torch.cuda.is_available() else 'cpu')
print(device)
### model-parameters
hidden_layers=16
lr=0.01
weight_decay=5e-4
normalize=False     # just keep it False always

# dataset_X = "../a3_datasets/d2_small_X.csv"
# dataset_splits = "../a3_datasets/d2_graph_splits.npz"
# graph_name = "d2"

p = int(sys.argv[1])
f = int(sys.argv[2])
dataset_X = sys.argv[3]
dataset_adj=sys.argv[4]
SE_file = sys.argv[5]
dataset_splits = sys.argv[6]
graph_name = sys.argv[7]
num_epochs=int(sys.argv[8])
batch_size=int(sys.argv[9])
model_name = "A3TGCN"

model_path=f"./models/{model_name}/{num_epochs}/{graph_name}"
os.makedirs(model_path,exist_ok=True)
plot_path=f"./plot_losses/{model_name}/{num_epochs}/{graph_name}"
os.makedirs(plot_path,exist_ok=True)

dataset=TimeSeries(dataset_adj,dataset_X, num_timesteps_in=p, num_timesteps_out=f)
dataloader = DataLoader(dataset, batch_size=batch_size,shuffle=False, num_workers=0)

# spatial embedding 
file = open(SE_file, mode = 'r')
lines = file.readlines()
temp = lines[0].split(' ')
N, dims = int(temp[0]), int(temp[1])
SE = np.zeros(shape = (N, dims), dtype = np.float32)
for line in lines[1 :]:
    temp = line.split(' ')
    index = int(temp[0])
    SE[index] = temp[1 :]

SE=torch.from_numpy(SE)

splits = np.load(dataset_splits)
train_node_ids = convert(splits["train_node_ids"],dataset.mapping)
val_node_ids = convert(splits["val_node_ids"],dataset.mapping) 
test_node_ids = convert(splits["test_node_ids"],dataset.mapping)

print(f)
model=GMAN(L=5,K=8,d=8,num_his=p,num_pre=f,batch_size=batch_size,bn_decay=0.8,use_bias=False,mask=False)
model=model
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
    for x,y in dataloader:
        optimizer.zero_grad()  # Clear gradients.
        #print(data.features)
        out = model(x.float(),SE.float())  
        out = out.permute(2,0,1)
        y = y.permute(2,0,1)
        loss = criterion(out[train_node_ids].type(torch.DoubleTensor),y[train_node_ids].type(torch.DoubleTensor))
        # print(loss)


        loss.backward()
        optimizer.step()
        running_loss += loss.item()
        # if i==100:
        #     break
    
    print('epoch %d training loss: %.3f' % (epoch + 1, running_loss /(len(dataset)*len(train_node_ids)) ))
    if plot:
        train_loss.append(running_loss /(len(dataset)*len(train_node_ids)))
        # MAE, MAPE, RMSE, MAE2 = evaluate_metric(model, dataset,train_node_ids,diff=True)
        # train_mae.append(MAE)
    

def test(epoch,test=False,plot=False):         # test=True for test set
    model.eval()
    global best_loss
    global bestmodel
    running_loss = 0.0
    with torch.no_grad():
        out0 = []
        y0 = []
        for x,y in dataloader:
            out = model(x.float(),SE.float())  
            out = out.permute(2,0,1)
            y = y.permute(2,0,1)
            if test:
                loss = criterion(out[test_node_ids].type(torch.DoubleTensor),y[test_node_ids].type(torch.DoubleTensor))
            else:
                loss = criterion(out[val_node_ids].type(torch.DoubleTensor),y[val_node_ids].type(torch.DoubleTensor))

            running_loss += loss.item()
        ## TODO : waht factor to be multiplied
        if test:
            print('epoch %d Test loss: %.3f' % (epoch + 1, running_loss / (len(dataset)*len(test_node_ids))))
        else:
            print('epoch %d Test loss: %.3f' % (epoch + 1, running_loss / (len(dataset)*len(val_node_ids))))
        
        

        if (epoch%100==0 and not test and plot):
            x=[i for i in range(len(out0))]
            plt.plot(x,out0.cpu(),label="Model prediction")
            plt.plot(x,y0.cpu(),label="Actual")    
            plt.draw()
            plt.legend()
            plt.savefig(f"{plot_path}/{epoch}.png")
            plt.clf()
        if (not test):
            val_loss.append(running_loss /(len(dataset)*len(val_node_ids)))
            # MAE, MAPE, RMSE, MAE2 = evaluate_metric(model, dataset,val_node_ids,diff=True)
            # val_mae.append(MAE)
    
        
    
    if test==False and (best_loss==-1 or running_loss < best_loss):
        best_loss=running_loss
        # Saving our trained model
        torch.save(model.state_dict(), f'{model_path}/bestval.pth')

if __name__ == '__main__':
    print('Start Training')
    os.makedirs('./models', exist_ok=True)

    # TODO we can use scalar to fit transform the data, also pass that in evaluate metric
    plot=False
    for epoch in range(num_epochs): 
        train(epoch,plot=plot)
        test(epoch=epoch,plot=plot)      # on validation set    

    print('performing test')
    test(epoch=num_epochs,test=True)
    print('Finished Training')

    print("For Training:  ")
    MAE, MAPE, RMSE, MAE2 = evaluate_metric(model, dataset,train_node_ids, SE=SE, f=f, p=p,diff=True)
    print("MAE: ", MAE, MAE2)

    model.load_state_dict(torch.load(f'{model_path}/bestval.pth'))
    print("For Validation:  ")
    MAE, MAPE, RMSE, MAE2 = evaluate_metric(model, dataset,val_node_ids,SE=SE, f=f, p=p,diff=True)
    print("MAE: ", MAE, MAE2)
    
    
    print("For Testing:  ")
    MAE, MAPE, RMSE, MAE2 = evaluate_metric(model, dataset,test_node_ids,SE=SE, f=f, p=p,diff=True)
    print("MAE: ", MAE, MAE2)


    ## ploting
    if (plot):
        epochs=[i for i in range(len(train_loss))]
        # print(epochs)
        plt.plot(epochs,train_loss,label="Train_loss")
        plt.plot(epochs,val_loss,label="Val_loss")    

        plt.xlabel("Epochs")
        plt.ylabel("Loss")
        plt.legend()
        plt.draw()
        plt.savefig(f"{plot_path}/loss.png")
        plt.clf()    

        # plt.plot(epochs,train_mae,label="Train_mae")
        # plt.plot(epochs,val_mae,label="Val_mae")    

        # plt.xlabel("Epochs")
        # plt.ylabel("MAE")
        # plt.legend()
        # plt.draw()
        # plt.savefig(f"{plot_path}/MAE.png")
        # plt.clf()    


    
    torch.save(model.state_dict(), f'{model_path}/lastmodel.pth')
