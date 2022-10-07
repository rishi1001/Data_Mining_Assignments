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
// string dataset_size_file="dummy_totGraphs.txt";
// string input_dataset_filename="dummy.txt";
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
        if(g.nodes.empty()) break;          // due to empty line
        // string dummy_line;
        // getline(features,dummy_line);
        feature_graphs.push_back(g);
    }
    // cout<<"Tot Featuers : "<<feature_graphs.size()<<"\n";
    ifstream inverted_index(inverted_index_file_name);
    while(!inverted_index.eof()){
        vector<int> v;
        read_vector(v,inverted_index);
        if(v.empty()) break;
        index.push_back(v);
    }
    // cout<<"Tot Indexes : "<<index.size()<<"\n";
    ifstream graph_index(graph_index_file_name);
    while(!graph_index.eof()){
        string dummy_line;              
        getline(graph_index,dummy_line);                // for the first line of t # 0
        if(dummy_line=="") break;                // done reading
        stringstream ss(dummy_line);
        char c;
        ss>>c;
        ss>>c;
        int graph_id;
        ss>>graph_id;
        Graph g;
        read_graph(g,graph_index);
        g.g_id=graph_id;
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
 * Checks if q is subgraph of g
 */
bool is_subgraph(Graph &q, Graph &g){
    // What to pass for 
    Options opt;
    opt.undirected = true;
    return check_subgraph(opt,q.nodes,q.edges,g.nodes,g.edges);
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
    // cout<<"Subset Size :"<<subset.size()<<"\n";
    // cout<<"Index Size :"<<index.size()<<"\n";
    for(int i=0;i<index.size();i++){
        // cout<<"here : "<<i<<"\n";
        if(is_subgraph(feature_graphs[i],q)){
            subset = intersection(subset,index[i]);
            // cout<<"subgraph found: "<<i<<"\n";
        }
    }
    return subset;
}

/**
 * Finds final results and outputs them
 */
vector<int> find_supergraphs_from_subset(vector<int> &subset, Graph &q, vector<Graph> &graphs){
    vector<int> final_graphs;
    for(int ind:subset){
        if(is_subgraph(q,graphs[ind])){
            // cout<<"here : "<< ind <<"\n";
            final_graphs.push_back(graphs[ind].g_id);            // adding the actual id
        }
    }
    return final_graphs;
    // TODO output final_graphs in the desired format
}


void write_to_file(vector<int>& v,ofstream &file){
    // cout<<"ans size: "<<v.size()<<"\n";
    string t="";
    for(int i=0;i<v.size();i++){            // v can be empty
        t=to_string(v[i])+'\t';
        file<<t;
    }
    t='\n';
    file<<t;
}
int main(int argc, char** argv){
    vector<Graph> graphs;
    vector<Graph> feature_graphs;
    vector<vector<int>> index;
    
    cout<<"Reading Index\n";
    read_index(Inverted_file_name,Feature_file_name,input_dataset_filename,graphs,feature_graphs,index);
    cout<<"Reading Index done\n";

    map<string,int> hash;
    string hashFileName = "hashing.txt";
    ifstream hashFile(hashFileName);
    // read hashes string:int from file
    while(!hashFile.eof()){
        string s;
        int i;
        hashFile>>s>>i;
        hash[s]=i;
    }
    Dataset_size=graphs.size();
    string query_file_name,output_file_name;
    output_file_name="output_2019CS10382.txt";
    cout<<"Enter query file name: ";
    cin>>query_file_name;
    auto start = high_resolution_clock::now();

    ifstream queries(query_file_name);
    int i=0;
    ofstream out(output_file_name);
    string line;
    while(!queries.eof()){
        getline(queries,line);         // dummy line for graph id(does query id matter?)
        if(line=="") break;
        cout<<"Find Indices for query "<<i<<"\n";
        i++;
        Graph query;
        getline(queries,line);
        stringstream ss(line);
        int totNodes;
        ss>>totNodes;
        for(int i=0;i<totNodes;i++){
            getline(queries,line);
            int nodeLabel = hash[line];
            query.nodes.push_back(nodeLabel);
        }
        getline(queries,line);
        stringstream ss1(line);
        int totEdges;
        ss1>>totEdges;
        for(int i=0;i<totEdges;i++){
            getline(queries,line);
            stringstream ss2(line);
            int u,v,label;
            ss2>>u>>v>>label;
            query.edges.push_back(make_tuple(u,v,label));
        }
        getline(queries,line);      // empty line
        cout<<"Done Reading\n";
        vector<int> ind = query_index(query,feature_graphs,index,Dataset_size);        
        cout<<"Got ind: " << ind.size() <<"\n";
        ind = find_supergraphs_from_subset(ind,query,graphs);
        cout<<"About to Write\n" << ind.size() << "\n";
        write_to_file(ind,out);
    }
    queries.close();
    out.close();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Total Time taken: "<< duration.count() << " milli seconds" << endl;

}
