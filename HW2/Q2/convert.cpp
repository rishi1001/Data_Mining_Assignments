#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <sstream>
#include <map>
using namespace std;

int main(int argc, char** argv){
    string fileName = argv[1];
    ifstream file(fileName);
    ofstream out("formatted.txt");
    ofstream outHash("hashing.txt");
    ofstream totGraphs("totGraphs.txt");
    string line;
    map<string,int> m;
    int hash=1;
    string toWrite="";
    int tot=0;
    while(!file.eof()){
        getline(file,line);
        if(line=="") break;
        tot+=1;
        // cout<<line<<"\n";           // get id from string
        toWrite = "t # "+line.substr(1,line.size()-1)+"\n";
        out<<toWrite;
        getline(file,line);
        stringstream ss(line);
        int totNodes;
        ss>>totNodes;
        for(int i=0;i<totNodes;i++){
            getline(file,line);
            // cout<<line<<"\n";
            if(m[line]==0){
                m[line]=hash;
                hash++;
            }
            toWrite = "v "+to_string(i)+" "+to_string(m[line])+"\n";
            out<<toWrite;
        }
        getline(file,line);
        stringstream ss1(line);
        int totEdges;
        ss1>>totEdges;
        for(int i=0;i<totEdges;i++){
            getline(file,line);
            // cout<<line<<"\n";
            toWrite = "e "+line+"\n";
            out<<toWrite;
        }
        getline(file,line);
        // cout<<"Done 1 graph\n";
    }
    for(auto it=m.begin();it!=m.end();it++){
        outHash<<it->first<<" "<<it->second<<"\n";
    }
    totGraphs<<tot;
    file.close();
    out.close();
    outHash.close();
    return 0;
}