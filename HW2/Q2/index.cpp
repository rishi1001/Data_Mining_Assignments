#include "util.hpp"
#include <assert.h>
#include <iostream>
#include <map>
#include <chrono>

using namespace std::chrono;

// Some parameters
int Dataset_size;
float initial_support;
float gamma;
string gSpan_output= "formatted.txt.fp";
// string gSpan_output= "dummy.txt.fp";
string dataset_size_file="totGraphs.txt";
// string dataset_size_file="dummy_totGraphs.txt";
string Feature_file_name="Features.txt";
string Inverted_file_name="index.txt";


// Returns the support for k size subgraph  
float get_support(int k,float initial_support){
    // currently just returing constant suport for each subgraph
    return initial_support; 
}

// return if the subgraph g with indices is discrimnative or not.
bool check_discriminative(Graph &g,vector<int>& indices, map<int,vector<int>>& m, float gamma){
    int g_size=g.edges.size();
    vector<int> parent=m[g_size-1];  
    // cout<<"here: "<<g_size<<":"<<m[g_size-1].size()<<"\n";
    float discriminating_factor= (parent.size()*(1.0))/indices.size();
    return discriminating_factor>gamma; // true is discriminating_factor is greater than gamma
}   

// returns true if we should include subgraph g in the feature vector
bool good_feature(Graph &g, int support ,vector<int>& indices,map<int,vector<int>>& m, float gamma, float initial_support ){
    float required_support=get_support(g.edges.size(),initial_support);
    return  (support> required_support) && (check_discriminative(g,indices,m,gamma));
}

/**
 * 
 * 
 * Read output of gspan and contruct index
 */
void construct_index(string feat_file_name, string inverted_index_file_name, string graph_index_file_name, float gamma,float initial_support){
    //TODO 
    map<int,vector<int>> m; // map to store inverted index of the last seen subgraph of size k
    vector<Graph> features; // Graph which are discriminative and frequent
    vector<vector<int>> inverted_indice; // inverted_indice[i] stores index of graph 
                                         // in datset in which features[i] is present 
    
    // size 0 is contained in each graph
    m[-1]={};
    for(int i=0;i<Dataset_size;i++) m[-1].push_back(i);
    
    ifstream gSpan_output(graph_index_file_name);
    while(true){
        // Add code for breaking if stream is empty
        if (gSpan_output.eof()) break;
        /* format 
            t #index <support>
            graph
            x <inverted index>
        */
        int support; // Add code to first read support from output
        bool empty=read_int(support,gSpan_output);
        if(!empty) break;
        
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
    string emptyLine = "\n";
    for(Graph g:features){
        write_graph(g,features_file);
        features_file<<emptyLine;
    }
    features_file.close();

    ofstream inverted_indices_file(inverted_index_file_name);
    for(vector<int> ind:inverted_indice){
        write_vector(ind,inverted_indices_file);
    }
    inverted_indices_file.close();


}

int main(int argc, char** argv){
    cout<<"Creating Index\n";
    auto start = high_resolution_clock::now();
    
    // reading Dataset size
    ifstream temp(dataset_size_file);
    temp>>Dataset_size;
    temp.close();

    initial_support=stof(argv[1]);
    gamma=stof(argv[2]);

    construct_index(Feature_file_name,Inverted_file_name,gSpan_output,gamma,initial_support);


    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Total Time taken: "<< duration.count() << " seconds" << endl;
    return 0;
}