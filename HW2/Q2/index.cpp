#include "util.hpp"
#include <assert.h>
#include<map>

// Returns the support for k size subgraph  
int get_support(int k,int initial_support){
    // currently just returing constant suport for each subgraph
    return initial_support; 
}

// return if the subgraph g with indices is discrimnative or not.
bool check_discriminative(Graph &g,vector<int>& indices, map<int,vector<int>>& m, float gamma){
    int g_size=g.edges.size();
    vector<int> parent=m[g_size-1];  
    float discriminating_factor= (parent.size()*(1.0))/indices.size();
    return discriminating_factor>gamma; // true is discriminating_factor is greater than gamma
}   

// returns true if we should include subgraph g in the feature vector
bool good_feature(Graph &g, int support ,vector<int>& indices,map<int,vector<int>>& m, float gamma, int initial_support ){
    int required_support=get_support(g.edges.size(),initial_support);
    return  (support> required_support) && (check_discriminative(g,indices,m,gamma));
}
// we can do this in script
void run_gspan(string dataset_file_name, string out_file_name, float support);

/**
 * Read output of gspan and contruct index
 */
// Also need total number of graphs in the dataset
int Dataset_size;
int initial_support;
string outfile; // file storing output of GSpan
void construct_index(string feat_file_name, string inverted_index_file_name, string feature_index_file_name, string graph_index_file_name, float gamma){
    //TODO 
    map<int,vector<int>> m; // map to store inverted index of the last seen subgraph of size k
    vector<Graph> features; // Graph which are discriminative and frequent
    vector<vector<int>> inverted_indice; // inverted_indice[i] stores index of graph 
                                         // in datset in which features[i] is present 
    
    // size 0 is contained in each graph
    m[0]={};
    for(int i=0;i<Dataset_size;i++) m[0].push_back(i);
    
    ifstream gSpan_output(outfile);
    while(true){
        // Add code for breaking if stream is empty
        
        /* format 
            t #index <support>
            graph
            x <inverted index>
        */
        int support; // Add code to first read support from output
        
        Graph potent_feature;
        read_graph(potent_feature,gSpan_output);

        vector<int> inv_index;  // TODO take care 'x' in the output
        read_vector(inv_index,gSpan_output);

        if(good_feature(potent_feature,support,inv_index,m,gamma,initial_support)){
            features.push_back(potent_feature);
            inverted_indice.push_back(inv_index);
        }

        // Updating m
        m[potent_feature.edges.size()]=inv_index;
    }
    gSpan_output.close();



    // dump features to files
    ofstream features_file(feat_file_name);
    for(Graph g:features){
        write_graph(g,features_file);
    }
    features_file.close();

    ofstream inverted_indices_file(inverted_index_file_name);
    for(vector<int> ind:inverted_indice){
        write_vector(ind,inverted_indices_file);
    }
    inverted_indices_file.close();


}

int main(){
    return 0;
}