import sys
import numpy as np
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt

if __name__ == "__main__":
    data = np.genfromtxt(sys.argv[1])
    print("Dataset: ", data.shape)
    output_name = sys.argv[2]

    cluster_sizes = np.arange(start=1,stop=16)
    inertia_vals = []
    for k in cluster_sizes:
        kmeans = KMeans(n_clusters=k,random_state=0).fit(data)
        inertia_vals.append(kmeans.inertia_)

    plt.plot(cluster_sizes, inertia_vals, marker='o')
    plt.xticks(cluster_sizes)
    plt.ylim(ymin=0)
    plt.xlabel("Number of clusters")
    plt.ylabel("Variation")
    plt.title("Elbow plot")
    plt.savefig(output_name)
    plt.clf()
    
