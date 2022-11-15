import torch.nn.functional as F
import torch
import numpy as np

p = 12
q = 12
z = p+q

TE = F.one_hot(torch.arange(0, z), z).unsqueeze(0).float()
print(TE.shape)
# SE_file = './a3_datasets/conv_d1_SE.txt'
# # spatial embedding 
# f = open(SE_file, mode = 'r')
# lines = f.readlines()
# temp = lines[0].split(' ')
# N, dims = int(temp[0]), int(temp[1])
# SE = np.zeros(shape = (N, dims), dtype = np.float32)
# for line in lines[1 :]:
#     temp = line.split(' ')
#     index = int(temp[0])
#     SE[index] = temp[1 :]

# print(SE.shape)
# print(SE)
n = 2
a = torch.tensor([[1,1],[0,0.0],[0,0]])
a+=a/n
print(a)
print(torch.mean(a))
