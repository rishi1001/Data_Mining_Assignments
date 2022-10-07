#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <sstream>
#include<tuple>
using namespace std;
// TODO also need to add node labels
struct Graph{
    vector<int> nodes;
    vector<tuple<int,int,int>> edges;
    int g_id;
};

/**
 * t # id
 * v <> <>
 * ..
 * e <> <> <>
 * ..
*/

/**
 * @brief assumses that graph at beging 
 * 
 * @param g: graph 
 * @param file : ifstream to take input
 * TODO  for ZERO SIZED GRAPH  
 *  
 */
void read_graph(Graph &g, ifstream &file){
    string line;
    int len;
    int prev_index=-1,index,label,a,b;
    //cout<<"hehe\n";
    while(!file.eof()){
        len=file.tellg();
        getline(file,line);
        if(line=="") {
            // file.seekg(len ,std::ios_base::beg);
            break;
        }
        stringstream ss(line);
        char c;
        ss>>c;
        if(c=='v'){   // v index label
            ss>>index;
            ss>>label;
            assert(index==prev_index+1);   // assuming in sorted order
            prev_index=index;
            g.nodes.push_back(label);
            //cout<<label<<endl;
        }
        else if(c=='e'){  // e a b label
            ss>>a;
            ss>>b;
            ss>>label;
            g.edges.push_back(make_tuple(a,b,label));
            //cout<<a<<" "<<b<<endl;
        }
        else{  // graph os complete restoring ifstream
            file.seekg(len ,std::ios_base::beg);
            break;
        }
    }
}
void write_graph(Graph &g, ofstream &file){
    string line="";
    for(int i=0;i<g.nodes.size();i++){
        line="v "+to_string(i)+' '+to_string(g.nodes[i])+'\n';
        file<<line;
    }
    for(int i=0;i<g.edges.size();i++){
        line="e "+to_string(get<0>(g.edges[i]))+' '+to_string(get<1>(g.edges[i]))+' '+to_string(get<2>(g.edges[i]))+'\n';
        file<<line;
    }
}


bool read_int(int &x, ifstream &file){  // t # index * support
    string line;
    bool found=false;
    while(!file.eof()){
        getline(file,line);
        if(line.size()>0){
            found=true;
            break;
        }
    }
    if(!found) return false;
    stringstream ss(line);
    char c;
    ss>>c;
    //cout<<line<<" "<<c<<" "<<int(c)<<" "<<int('t')<<endl;
    // cout<<"here0\n";
    // cout<<line<<endl;
    //cout<<"here\n";
    //cout<<c<<" "<<int(c)<<endl;
    //cout<<"here1\n";
    //cout<<int('t')<<endl;
    //cout<<"here2\n";
    assert(c=='t');
    //cout<<"here3\n";

    ss>>c; 
    assert(c=='#');
    ss>>x;
    ss>>c;
    assert(c=='*');
    ss>>x;
    return true;
}

// Do we need this
void write_int(int x, ofstream &file);

void read_vector(vector<int> &v, ifstream &file){
    string line;
    getline(file,line);
    // cout<<"Here: "<<line<<"\n";
    if(line=="") return;
    stringstream ss(line);
    char c;
    ss>>c;
    assert(c=='x');
    int num;
    while(ss>>num){
        v.push_back(num);
        //cout<<num<<" ";
    }
    //cout<<endl;
}
void write_vector(vector<int> &v, ofstream &file){
    string line="";
    line+='x';
    for(int x:v){
        line+=' '+to_string(x);
    }
    line+='\n';
    file<<line;
}