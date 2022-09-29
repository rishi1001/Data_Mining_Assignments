#include "util.hpp"
#include "vf3lib/subgraph.hpp"
#include "vf3lib/include/Options.hpp"
#include <assert.h>

/**
 * @brief Reading from the inverted index and graph file. And inserting into the desired vectors.
 * 
 * @param inverted_index_file_name 
 * @param feature_index_file_name 
 * @param graph_index_file_name 
 * @param graphs 
 * @param feature_graphs 
 * @param index 
 */
void read_index(string inverted_index_file_name, string feature_index_file_name, string graph_index_file_name, vector<Graph> &graphs, vector<Graph> &feature_graphs, vector<vector<int>> &index){
    ifstream features(feature_index_file_name);
    while(!features.eof()){
        Graph g;
        read_graph(g,features);
        feature_graphs.push_back(g);
    }
    ifstream inverted_index(inverted_index_file_name);
    while(!inverted_index.eof()){
        vector<int> v;
        read_vector(v,inverted_index);
        index.push_back(v);
    }
    ifstream graph_index(graph_index_file_name);
    while(!graph_index.eof()){
        string dummy_line;              
        getline(graph_index,dummy_line);                // for the first line of t # 0
        if(dummy_line=="") break;                // done reading
        Graph g;
        read_graph(g,graph_index);
        graphs.push_back(g);
    }
}

/**
 * Get feature vector for graph
 */
// vector<int> get_feature_vector(Graph &q, vector<Graph> &feature_graphs){
//     vector<int> feature_vector;
//     for(Graph g:feature_graphs){
//         if (is_subgraph(g,q)){
//             feature_vector.push_back(1);
//         }
//         else feature_vector.push_back(0);
//     }
//     return feature_vector;
// }

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

vector<int> query_index(Graph &q,vector<Graph> &feature_graphs, vector<vector<int>> &index,int dataset_size){
    vector<int> subset;
//     TODO: Start form last index with one. Take the dataset when all are zeros
    for(int i=0;i<dataset_size;i++){
        subset.push_back(i);
    }
    for(int i=0;i<index.size();i++){
        if(is_subgraph(feature_graphs[i],q)){
            subset = intersection(subset,index[i]);
        }
    }
    return subset;
}

/**
 * Checks if q is subgraph of g
 */
bool is_subgraph(Graph &q, Graph &g){
    // What to pass for 
    Options opt;
    return check_subgraph(opt,q.nodes,q.edges,g.nodes,g.edges);
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
