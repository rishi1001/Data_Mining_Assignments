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


def evaluate_metric(model, X,Y, G):
    model.eval()
    with torch.no_grad():
        mae, mape, mse = [], [], []
        y_pred = model(X, G.edge_index, G.edge_weight).view(len(X), -1)
        print(y_pred)
        print(Y)
        print(y_pred.shape,Y.shape)
        d = np.abs(Y - y_pred)
        mae += d.tolist()
        mape += (d / Y).tolist()
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