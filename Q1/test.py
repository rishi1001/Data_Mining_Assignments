import sys

import pandas as pd

from model import *
from dataset import TimeSeries

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

    model = None
    if model_name=="gcn":
        model = GCN(hidden_channels=hidden_layers).to(device)
    else:
        model = GAT(hidden_channels=hidden_layers).to(device)

    model.load_state_dict(torch.load(model_path))
    model.eval()

    dataset=TimeSeries(dataset_X,dataset_adj,normalize)

    results = pd.read_csv(dataset_X).drop(['Unnamed: 0'], axis=1)

    with torch.no_grad():
        for i in range(len(dataset)+1): # dataset returns one less probably
            data = dataset[i]
            y_pred = model(data.x, dataset.edge_index, dataset.edge_weight).cpu()
            # since we are predicting y-x
            y_pred += data.x
            results.iloc[i] = y_pred
    
    results.to_csv(output_path)



