#include "util.hpp"
#include "vf3lib/subgraph.hpp"
#include "vf3lib/include/Options.hpp"
#include <assert.h>
#include <iostream>
#include <chrono>

using namespace std::chrono;

int Dataset_size;
string dataset_size_file="totGraphs.txt";
string input_dataset_filename="formatted.txt";
string Feature_file_name="Features.txt";
string Inverted_file_name="index.txt";

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
        string dummy_line;
        getline(features,dummy_line);
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
vector<int> find_supergraphs_from_subset(vector<int> &subset, Graph &q, vector<Graph> &graphs){
    vector<int> final_graphs;
    for(int ind:subset){
        if(is_subgraph(q,graphs[ind])){
            final_graphs.push_back(ind);
        }
    }
    return final_graphs;
    // TODO output final_graphs in the desired format
}


void write_to_file(vector<int>& v,ofstream &file){
    string t="";
    for(int i=0;i<v.size()-1;i++){
        t=to_string(v[i])+'\t';
        file<<t;
    }
    t=to_string(v[v.size()-1])+'\n';
    file<<t;
}
int main(int argc, char** argv){
    vector<Graph> graphs;
    vector<Graph> feature_graphs;
    vector<vector<int>> index;
    
    cout<<"Reading Index\n";
    read_index(Inverted_file_name,Feature_file_name,input_dataset_filename,graphs,feature_graphs,index);
    cout<<"Reading Index done\n";
    Dataset_size=graphs.size();
    string query_file_name,output_file_name;
    query_file_name=argv[1];
    output_file_name=argv[2];
    // cout<<"Enter query file name: ";
    // cin>>query_file_name;
    auto start = high_resolution_clock::now();
    
    ifstream queries(query_file_name);
    int i=0;
    ofstream out(output_file_name);
    while(!queries.eof()){
        cout<<"Find Indices for query "<<i<<endl;
        i++;
        Graph query;
        read_graph(query,queries);
        vector<int> ind=query_index(query,feature_graphs,index,Dataset_size);
        ind = find_supergraphs_from_subset(ind,query,graphs);
        write_to_file(ind,out);
    }
    queries.close();
    out.close();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Total Time taken: "<< duration.count() << " seconds" << endl;

}
