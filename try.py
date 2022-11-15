import torch.nn.functional as F
import torch

p = 12
q = 12
z = p+q

TE = F.one_hot(torch.arange(0, z), z).unsqueeze(0).float()
print(TE.shape)