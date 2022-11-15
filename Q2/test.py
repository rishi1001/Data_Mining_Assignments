import sys

import numpy as np
import pandas as pd
from torch_geometric.utils import dense_to_sparse

from model import *
from dataset import TimeSeries

### model-parameters
hidden_layers=16
lr=0.01
weight_decay=5e-4
normalize=False

device = torch.device(f'cuda' if torch.cuda.is_available() else 'cpu')
print("Testing on", device)
   
def read_graph(dataset_adj):
    df = pd.read_csv(dataset_adj,index_col=0)
    cols=df.columns
    df.columns=[i for i in range(len(cols))]
    df=df.reset_index(drop=True)

    t=torch.tensor(df.values)
    edge_index , edge_weight = dense_to_sparse(t)
    num_nodes = len(cols)
    edge_index = edge_index.to(device)
    edge_weight = edge_weight.to(device)
    return edge_index, edge_weight

if __name__ == "__main__":
    p = int(sys.argv[1])
    f = int(sys.argv[2])
    dataset_X = sys.argv[3]
    dataset_adj = sys.argv[4]
    output_path = sys.argv[5]
    model_path = sys.argv[6]

    model = None
    model = TemporalGNN(node_features=1, periods=f).to(device)     # to device remains

    model.load_state_dict(torch.load(model_path))
    model.eval()

    test_dataset = np.load(dataset_X)
    edge_index, edge_weight = read_graph(dataset_adj)

    results = []

    with torch.no_grad():
        for x in test_dataset: # dataset returns one less probably
            y_pred = model(x, edge_index, edge_weight).cpu()
            # since we are predicting y-x
            y_pred += x[-1]
            results.append(y_pred)
    
    np.save(output_path, np.array(results))



