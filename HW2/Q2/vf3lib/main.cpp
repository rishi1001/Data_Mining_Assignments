#include "subgraph.hpp"
using namespace std;

void test_1(){
	Options opt;
	vector<int> nodes_q = {0,1};
	vector<tuple<int,int,int>> edges_q = {
		{0,1,0}
	};
	
	vector<int> nodes_g = {0,1,2};
	vector<tuple<int,int,int>> edges_g = {
		{0,1,0},
		{0,2,1}
	};

	if(check_subgraph(opt, nodes_q, edges_q, nodes_g, edges_g)){
		std::cout << "subgraph" << std::endl;
	}
	else{
		std::cout << "not a subgraph" << std::endl;
	}
}

void test_2(){
	Options opt;
	vector<int> nodes_q = {0,1};
	vector<tuple<int,int,int>> edges_q = {
		{0,1,0}
	};
	
	vector<int> nodes_g = {0,1,2};
	vector<tuple<int,int,int>> edges_g = {
		{0,1,1},
		{0,2,0}
	};

	if(check_subgraph(opt, nodes_q, edges_q, nodes_g, edges_g)){
		std::cout << "subgraph" << std::endl;
	}
	else{
		std::cout << "not a subgraph" << std::endl;
	}
}

void test_3(){
	Options opt;
	vector<int> nodes_q = {0,1};
	vector<tuple<int,int,int>> edges_q = {
		{0,1,0}
	};
	
	vector<int> nodes_g = {3,1,2};
	vector<tuple<int,int,int>> edges_g = {
		{0,1,0},
		{0,2,1}
	};

	if(check_subgraph(opt, nodes_q, edges_q, nodes_g, edges_g)){
		std::cout << "subgraph" << std::endl;
	}
	else{
		std::cout << "not a subgraph" << std::endl;
	}
}


void test_4(){
	Options opt;
	vector<int> nodes_q = {0,1};
	vector<tuple<int,int,int>> edges_q = {
		{0,1,0}
	};
	
	vector<int> nodes_g = {0,0,2,3,1,1};
	vector<tuple<int,int,int>> edges_g = {
		{0,1,0},
		{0,2,0},
		{2,3,0},
		{0,4,0},
		{4,5,0}
	};

	opt.undirected = true;

	if(check_subgraph(opt, nodes_q, edges_q, nodes_g, edges_g)){
		std::cout << "subgraph" << std::endl;
	}
	else{
		std::cout << "not a subgraph" << std::endl;
	}
}

int main(){
	test_1();
	test_2();
	test_3();
	test_4();
}