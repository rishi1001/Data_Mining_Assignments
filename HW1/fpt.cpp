#include <cstdio>
#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <climits>
using namespace std;

int x=98;           // this is hard coded, take command line input
int tot_transactions=0;          // tot transactions (update it)

struct Node{
    int val;
    Node *parent;
    int count;          // this count stores the total count which end at this particular node
};

// Function to get Leaves of the tree
void getLeaves(vector<Node*> &leaves, Node* curr, map<Node*,vector<Node*>>& adj){
    if (curr==nullptr){
        return;
    }

    if(adj[curr].size()==0){
        leaves.push_back(curr);
        return;
    }

    for(Node* child: adj[curr]){
        getLeaves(leaves,child,adj);
    }
}

bool isFrequent(int v){
    if(v*100>=x*tot_transactions){
        return true;
    }
    return false;
}

// Maximum Number of elements 
int MAXN = INT_MIN;
void generateItemsets(int curr, vector<Node*> &a ,map<Node*,int> &count, vector<int> &freq, vector<vector<int>> &ans){            // call with curr=MAXN and all leaf nodes in a
    if(curr<0 || a.empty()) return;
    // ans.push_back(freq);
    // cout<<"A at "<<(curr)<<"\n";
    // for(Node* x:a){
    //     cout<<"{"<<x->val<<" "<<x->count<<" "<<count[x]<<"} ";
    // }
    // cout<<endl;
    for(int i=curr;i>=0;i--){
        freq.push_back(i);
        vector<Node*> up;
        map<Node*,int> up_count;                                  
        int n=a.size();
        int tot=0;
        for(int j=0;j<n;j++){
            if(a[j]->val==i){
                up_count[a[j]->parent]+=count[a[j]]+a[j]->count;            
                a[j]=a[j]->parent;          
            }
        }
        

        for(auto x:up_count) {
            if (x.first->val !=-1) up.push_back(x.first);
            tot+=x.second;
        }

        if(isFrequent(tot)){
            //cout<<"Where are youu??"<<"\n";
            //cout<<"freq.size("<<freq.size()<<"\n";
            
            ans.push_back(freq);
            // if (curr==MAXN){
            //     cout<<"fffff\n";
            //     for(int xx:freq) cout<<xx<<" ";
            //     cout<<endl;
            //     cout<<"CALLING for "<<i-1<<endl;
            //     for(Node* x:up){
            //         cout<<"{"<<x->val<<" "<<x->count<<" "<<up_count[x]<<"} ";
            //     }
            //     cout<<endl;

            // }
            generateItemsets(i-1,up,up_count,freq,ans);
        }
        freq.pop_back();
    }
}

// vector<Node> adj[MAXN];

vector<vector<int>> fpt(string datasetName){
    vector<vector<int>> ans;
    ifstream inFile;
    inFile.open(datasetName);
    tot_transactions=0;
    
    // Tree construction
    Node* root = new Node;
    root->parent = NULL;
    root-> count =0;
    root-> val =-1;

    Node* temp_root = new Node;
    map<Node*,vector<Node*>> adj; // adj[N] = {vector of children of N}
    bool found;


    while(!inFile.eof()){
        string s;
        getline(inFile,s);
        if(s.empty()) break;
        stringstream ss(s);
        int num;
        temp_root = root;
        while(ss>>num){
            MAXN=max(num,MAXN);

            // Checking if node is in current Node's childs
            found=false;
            for (Node* child: adj[temp_root]){
                if (child->val==num){
                    found = true;
                    temp_root = child;
                    break;
                }
            }

            if (!found){
                // Adding node to the tree
                Node* newNode = new Node;
                newNode->val = num;
                newNode->count =0;
                newNode->parent = temp_root;

                adj[newNode] = {};
                adj[temp_root].push_back(newNode);

                temp_root = newNode;
            }

            // updating count of the particular node
            

        }
        temp_root->count +=1;
        tot_transactions++;
    }
    inFile.close();

    // dfs on tree and call fun()
    // PRINTING TREE
    // queue<Node*> q;
    // q.push(root);
    // int level=0;
    // while(!q.empty()){
    //     cout<<"LEVEL "<<level<<endl;
    //     int ll=q.size();
    //     for(int i=0;i<ll;i++){
    //         Node* tt = q.front();
    //         q.pop();
    //         cout<<"{"<<tt->val<<" "<<tt->count<<"} ";
    //         for(auto x:adj[tt]){
    //             q.push(x);
    //         }
    //     }
    //     level++;
    //     cout<<endl;
    // }


    vector<Node*> leaves;
    getLeaves(leaves,root,adj);
    // for(Node* x:leaves){
    //     cout<<"{"<<x->val<<" "<<x->count<<"} ";
    // }
    // cout<<endl;
    // Generating Itemsets
    map<Node*,int> count; 
    vector<int> freq;
    //cout<<MAXN<<"\n";
    generateItemsets(MAXN,leaves,count,freq,ans);
    return ans;


}

void writeOutput (string outputFileName, vector<vector<int>>& ans){
    // Need to convert the first string because of output
    // 121 comes before 8 in answer
    ofstream fout;
    fout.open(outputFileName);
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
        s+='\n';
        //fout<<s;
        sorted_ans.push_back(s);
    }
    sort(sorted_ans.begin(),sorted_ans.end());
    for(auto i: sorted_ans){
        fout<<i;
    }
    fout.close();
} 


int main(int argc, char **argv)
{

    string datasetName = argv[1];
    x= stoi(argv[2]);
    string outputFileName = argv[3];

    vector<vector<int>> ans=fpt(datasetName);
    writeOutput(outputFileName,ans);
    
    return 0;
}