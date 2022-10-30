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
    directory='./dataset2'
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

best_loss = -1
bestmodel = None

def train(epoch):
    model.train()

    running_loss = 0.0
    # batch wise training
    for data in enumerate(train_set):
        optimizer.zero_grad()  # Clear gradients.
        print(data.features)
    #   print(data.features.shape)
        out = model(data.features, data.edge_index, data.edge_weight).reshape(-1)   
    #   print(out.shape)
        loss = criterion(out, data.y[data.train_mask])
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
            out = model(data.features, data.edge_index, data.edge_weight).reshape(-1)
            if test:
                loss = criterion(out, data.y[data.test_mask])
            else:
                loss = criterion(out, data.y[data.val_mask])
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
        test(val_set)      # on validation set      

    print('performing test')
    test(test_set, test=True)       # on test set

    print('Finished Training')

    # Saving our trained model
    torch.save(model.state_dict(), './models/lastmodel.pth')