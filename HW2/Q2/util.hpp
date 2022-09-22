#pragma once
#include <vector>
#include <fstream>
#include <string>
using namespace std;
// TODO also need to add node labels
struct Graph{
    vector<int> nodes;
    vector<tuple<int,int,int>> edges;
};

/**
 * t # id
 * v <> <>
 * ..
 * e <> <> <>
 * ..
*/
void read_graph(Graph &g, ifstream &file);
void write_graph(Graph &g, ofstream &file);


void read_int(int x, ifstream &file);
void write_int(int x, ofstream &file);

void read_vector(vector<int> &v, ifstream &file);
void write_vector(vector<int> &v, ofstream &file);