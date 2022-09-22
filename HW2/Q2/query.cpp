#include "util.hpp"

void read_index(string inverted_index_file_name, string feature_index_file_name, string graph_index_file_name, vector<Graph> &graphs, vector<Graph> &feature_graphs, vector<vector<int>> &index);

/**
 * Get feature vector for graph
 */
vector<int> get_feature_vector(Graph &q, vector<Graph> &feature_graphs);

/**
 * Find subset of graphs which are relevant, using the query feature vector and the index 
 */
vector<int> query_index(vector<int> &query_f, vector<vector<int>> &index);

/**
 * Checks if q is subgraph of g
 */
bool is_subgraph(Graph &q, Graph &g);

/**
 * Finds final results and outputs them
 */
void find_supergraphs_from_subset(vector<int> &subset, Graph &q, vector<Graph> &graphs);

int main(){
    return 0;
}