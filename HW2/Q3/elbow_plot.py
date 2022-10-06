import sys
import numpy as np
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt

def get_2_diff(inertia, i):
    return ((inertia[i-1] - inertia[i]) / (inertia[i] - inertia[i+1] + 0.000001))

if __name__ == "__main__":
    data = np.genfromtxt(sys.argv[1])

    # print("Dataset: ", data.shape)
    output_name = sys.argv[2]

    # plt.scatter(data[:,0], data[:,1])
    # plt.savefig("data_" + output_name)
    # plt.clf()
    # fig = plt.figure(figsize=(4,4))
    # ax = fig.add_subplot(111,projection="3d")
    # ax.scatter(data[:,0], data[:,1],data[:,2])
    # plt.savefig("data_" + output_name)
    # plt.clf()

    cluster_sizes = np.arange(start=1,stop=16)
    inertia_vals = []
    for k in cluster_sizes:
        kmeans = KMeans(n_clusters=k,random_state=0).fit(data)
        inertia_vals.append(kmeans.inertia_)
    

    max_2_diff_index = 1
    for k in range(1,len(inertia_vals)-1):
        # print(get_2_diff(inertia_vals, k))
        if get_2_diff(inertia_vals, k) > get_2_diff(inertia_vals, max_2_diff_index):
            max_2_diff_index = k        
    plt.plot(cluster_sizes, inertia_vals, marker='o')
    plt.plot(cluster_sizes[max_2_diff_index], inertia_vals[max_2_diff_index], marker='o')
    plt.xticks(cluster_sizes)
    plt.ylim(ymin=0)
    plt.xlabel("Number of clusters")
    plt.ylabel("Variation")
    plt.title("Elbow plot")
    plt.savefig(output_name)
    plt.clf()
    
