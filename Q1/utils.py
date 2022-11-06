import torch
import numpy as np
import matplotlib.pyplot as plt

def evaluate_metric_scaler(model, data_iter, scaler):
    model.eval()
    with torch.no_grad():
        mae, mape, mse = [], [], []
        for x, y in data_iter:
            y = scaler.inverse_transform(y.cpu().numpy()).reshape(-1)
            y_pred = scaler.inverse_transform(
                model(x).view(len(x), -1).cpu().numpy()
            ).reshape(-1)
            d = np.abs(y - y_pred)
            mae += d.tolist()
            mape += (d / y).tolist()
            mse += (d**2).tolist()
        MAE = np.array(mae).mean()
        MAPE = np.array(mape).mean()
        RMSE = np.sqrt(np.array(mse).mean())
        return MAE, MAPE, RMSE


def evaluate_metric(model, dataset, G):
    model.eval()
    with torch.no_grad():
        mae, mape, mse = [], [], []
        for data in dataset:
            y = data['y']
            y_pred = model(data['x'], G.edge_index, G.edge_weight)
            d = np.abs(y - y_pred)
            mae += d.tolist()
            mape += (d / y).tolist()
            mse += (d**2).tolist()
        MAE = np.array(mae).mean()
        MAPE = np.array(mape).mean()
        RMSE = np.sqrt(np.array(mse).mean())
        return MAE, MAPE, RMSE

def plot(n, future, y, y_pred):
    plt.figure(figsize=(12,6))
    plt.title(f"Step {i+1}")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)

    plt.plot(np.arange(n), y, 'r', linewidth=2.0)
    plt.plot(np.arange(n, n+future), y_pred, 'r:', linewidth=2.0)

def plot_y(dataset):
    # print(dataset.shape)
    y = []
    for i in range(len(dataset)):
        print(i)
        y.append(dataset[i]['x'][0].item())
    print(y)
    plt.plot(y)
    plt.show()

def plot_graph(G):
    # use networkx
    import networkx as nx
    import matplotlib.pyplot as plt
    # load networkx graph
    edges = G.edge_index.t().numpy()
    print(G.edge_index.shape)
    G = nx.from_edgelist(edges)
    nx.draw(G)
    plt.show()