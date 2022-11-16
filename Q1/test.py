import sys

import pandas as pd

from model import *
from dataset import TimeSeries
from torch_geometric.utils import dense_to_sparse

def get_dataset(graph_file,csv_file):
    df = pd.read_csv(graph_file,index_col=0)
    cols=df.columns
    df.columns=[i for i in range(len(cols))]
    df=df.reset_index(drop=True)
    
    t=torch.tensor(df.values)
    edge_index , edge_weight = dense_to_sparse(t)
    edge_weight=edge_weight
    num_nodes = len(cols)
    edge_index=edge_index.to(device)
    edge_weight=edge_weight.to(device)
    
    if normalize:
        edge_weight=edge_weight/torch.max(edge_weight)
        edge_weight=edge_weight.double()
    #mapping ={i:cols[i] for i in range(len(cols))}                    
    mapping=cols
    
    df = pd.read_csv(csv_file)
    df=df.drop(['Unnamed: 0'], axis=1)
    dataset=torch.tensor(df.values).type(torch.DoubleTensor)
    return dataset.to(device),edge_index.to(device),edge_weight.to(device)

### model-parameters
hidden_layers=32
lr=0.001
weight_decay=5e-4
normalize=False

device = torch.device(f'cuda' if torch.cuda.is_available() else 'cpu')
print("Testing on", device)

if __name__ == "__main__":
    dataset_X = sys.argv[1]
    dataset_adj = sys.argv[2]
    output_path = sys.argv[3]
    model_path = sys.argv[4]
    model_name = sys.argv[5]
    print(sys.argv)

    model = None
    if model_name=="gcn":
        model = GCN(hidden_channels=hidden_layers).to(device)
    else:
        model = GAT(hidden_channels=hidden_layers).to(device)
    model=model.double()
    model.load_state_dict(torch.load(model_path))
    model.eval()

    dataset,edge_index,edge_weight = get_dataset(dataset_adj,dataset_X)
    results = pd.read_csv(dataset_X)
    timestamps = pd.to_datetime(results[results.columns[0]])
    results = results.drop(results.columns[0], axis=1)
    timestamps = timestamps + (timestamps[1] - timestamps[0])
    results.index = timestamps
    results.index.name = None

    with torch.no_grad():
        for i in range(len(dataset)): # dataset returns one less probably
            x=dataset[i]
            x=torch.reshape(x,(x.shape[0],1))
            y_pred = model(x, edge_index, edge_weight).cpu()
            # since we are predicting y-x
            y_pred += x.cpu()
            y_pred = y_pred.ravel().tolist()
            # print(y_pred.shape)
            results.iloc[i] = y_pred
    
    results.to_csv(output_path)



