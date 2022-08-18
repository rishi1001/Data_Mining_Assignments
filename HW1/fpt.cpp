#include <cstdio>
#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

int x=98;           // this is hard coded, take command line input
int tot_transactions;          // tot transactions (update it)

struct Node{
    int val;
    Node *parent;
};

bool isFrequent(int v){
    if(v*100>=x*tot_transactions){
        return true;
    }
    return false;
}

int MAXN = 5;

vector<vector<int>> ans;
void fun(int curr, vector<Node*> &a, vector<int> &freq){            // call with curr=MAXN and all leaf nodes in a
    if(curr<0 || a.empty()) return;
    // ans.push_back(freq);
    for(int i=curr;i>=0;i--){
        freq.push_back(i);
        vector<Node*> up;
        int n=a.size();
        for(int j=0;j<n;j++){
            if(a[j]->val==i){
                up.push_back(a[j]->parent);
                a[j]=a[j]->parent;          // update this
            }
        }
        if(isFrequent(up.size())){
            ans.push_back(freq);
        }
        fun(i-1,up,freq);
        freq.pop_back();
    }
}

// vector<Node> adj[MAXN];

vector<vector<int>> fpt(){
    vector<vector<int>> ans;
    ifstream inFile;
    inFile.open("my_test.dat");
    tot_transactions=0;
    while(!inFile.eof()){
        string s;
        getline(inFile,s);
        if(s.empty()) break;
        stringstream ss(s);
        int num;
        while(ss>>num){
            c1[num]++;          // make treeee
        }
        tot_transactions++;
    }
    inFile.close();

    // dfs on tree and call fun()
    
    return ans;


}

int main()
{

    vector<vector<int>> ans=fpt();
    vector<string> sorted_ans;
    for(auto i: ans){
        string s="";
        vector<string> vs;
        for(auto j:i){
            vs.push_back(to_string(j));
        }
        sort(vs.begin(),vs.end());
        int vsn=vs.size();
        for(int j=0;j<vsn;j++){
            s+=vs[j];
            if(j!=vsn-1) s+=" ";
        }
        sorted_ans.push_back(s);
    }
    sort(sorted_ans.begin(),sorted_ans.end());
    for(auto i: sorted_ans){
        cout<<i<<"\n";
    }
    
    return 0;
}