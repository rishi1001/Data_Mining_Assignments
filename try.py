import numpy as np

splits = np.load("a3_datasets/d1_graph_splits.npz") 
train_node_ids = splits["train_node_ids"] 
val_node_ids = splits["val_node_ids"] 
test_node_ids = splits["test_node_ids"]

print(train_node_ids.shape)
print(val_node_ids.shape)
print(test_node_ids.shape)