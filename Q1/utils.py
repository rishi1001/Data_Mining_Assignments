import torch
import numpy as np
import matplotlib.pyplot as plt

# def evaluate_metric_scaler(model, data_iter, scaler):
#     model.eval()
#     with torch.no_grad():
#         mae, mape, mse = [], [], []
#         for x, y in data_iter:
#             y = scaler.inverse_transform(y.cpu().numpy()).reshape(-1)
#             y_pred = scaler.inverse_transform(
#                 model(x).view(len(x), -1).cpu().numpy()
#             ).reshape(-1)
#             d = np.abs(y - y_pred)
#             mae += d.tolist()
#             mape += (d / y).tolist()
#             mse += (d**2).tolist()
#         MAE = np.array(mae).mean()
#         MAPE = np.array(mape).mean()
#         RMSE = np.sqrt(np.array(mse).mean())
#         return MAE, MAPE, RMSE

def evaluate_metric(model, dataset,mask,diff=False):
    model.eval()
    with torch.no_grad():
        mae, mape, mse, mae2 = [], [], [], []
        for i in range(len(dataset)):
            data = dataset[i]
            y = data.y.cpu()
            y_pred = model(data.x, dataset.edge_index, dataset.edge_weight).cpu()
            d = np.abs(y[mask] - y_pred[mask])
            d2 = np.abs(data.x[mask].cpu() - y[mask])
            if diff:
                d2 = np.abs(y[mask])
            mae += d.tolist()
            mae2 += d2.tolist()
            mape += (d / y[mask]).tolist()
            mse += (d**2).tolist()
        MAE = np.array(mae).mean()
        MAE2 = np.array(mae2).mean()
        MAPE = np.array(mape).mean()
        RMSE = np.sqrt(np.array(mse).mean())
        return MAE, MAPE, RMSE, MAE2

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

def plot_pred(X,Y,out):
    hori = np.arange(0, len(out))
    plt.plot(hori, out, 'r', linewidth=2.0)
    plt.plot(hori, X, 'b', linewidth=2.0)
    plt.plot(hori, Y, 'g', linewidth=2.0)
    plt.show()