#include "util.hpp"
#include <assert.h>

void read_index(string inverted_index_file_name, string feature_index_file_name, string graph_index_file_name, vector<Graph> &graphs, vector<Graph> &feature_graphs, vector<vector<int>> &index);

/**
 * Get feature vector for graph
 */
vector<int> get_feature_vector(Graph &q, vector<Graph> &feature_graphs){
    vector<int> feature_vector;
    for(Graph g:feature_graphs){
        if (is_subgraph(g,q)){
            feature_vector.push_back(1);
        }
        else feature_vector.push_back(0);
    }
    return feature_vector;
}

/*
returns interesct vector of set and superset
Assuming set and superset are sorted
*/
vector<int> intersection(vector<int>&  set, vector<int>& superset){
    vector<int> intersect;
    int i=0,j=0;
    while(i<set.size() && j<superset.size()){
        if(set[i]==superset[j]){
            intersect.push_back(set[i]);
            i++;
            j++;
        }
        else if(set[i]>superset[j]){
            j++;
        }
        else i++;
    }
    return intersect;
}

/**
 * Find subset of graphs which are relevant, using the query feature vector and the index 
 */

vector<int> query_index(vector<int> &query_f, vector<vector<int>> &index,int dataset_size){
    vector<int> subset;
    for(int i=0;i<dataset_size;i++){
        subset.push_back(i);
    }
    for(int i=0;i<index.size();i++){
        if(query_f[i]==1){
            subset=intersection(subset,index[i]);
        }
    }
    return subset;
}

/**
 * Checks if q is subgraph of g
 */
bool is_subgraph(Graph &q, Graph &g){

}

/**
 * Finds final results and outputs them
 */
void find_supergraphs_from_subset(vector<int> &subset, Graph &q, vector<Graph> &graphs){
    vector<int> final_graphs;
    for(int ind:subset){
        if(is_subgraph(q,graphs[ind])){
            final_graphs.push_back(ind);
        }
    }

    // TODO output final_graphs in the desired format
}

int main(){
    return 0;
}