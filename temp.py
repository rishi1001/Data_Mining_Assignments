import torch

temp = torch.tensor([1,2,3,4,5])


mask = [0,1,2,4]

print(temp[mask])