#include "subgraph.hpp"
using namespace std;
int main(){
	Options opt;
	vector<int> nodes_q = {0,1};
	vector<tuple<int,int,int>> edges_q = {
		{0,1,0}
	};
	
	vector<int> nodes_g = {0,1,1};
	vector<tuple<int,int,int>> edges_g = {
		{0,1,0},
		{0,2,0}
	};

	check_subgraph(opt, nodes_q, edges_q, nodes_g, edges_g);
}