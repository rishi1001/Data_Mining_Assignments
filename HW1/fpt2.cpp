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
vector<vector<int>> ans;   // GLOBAL VECTOR FOR ALL TRANSACTIONS
struct Node{
    int val;
    Node *parent;
    Node *next;
    int count;          // this count stores the total count which end at this particular node
};

struct fptree
{
    Node *root = nullptr;
    map<int,Node*> headerTable;
};


bool isFrequent(int v){
    if(v*100>=x*tot_transactions){
        return true;
    }
    return false;
}

// ALL COMBINATION OF SIGLE PATH TREE PUSH IN ans
void mine(struct fptree* tree, vector<int>& prefix){
    vector<int> elements;
    for(pair<int,Node*> ele: tree->headerTable){     
        elements.push_back(ele.first);
    }
    int n=elements.size();
    int tot_n = (1<<n);
    for(int i=0;i<tot_n;i++){
        vector<int> curr=prefix;            // TODO check if EFFICENT or not
        for(int j=0;j<n;j++){
            if(i&(1<<j)){
                curr.push_back(elements[j]);
            }
        }
        ans.push_back(curr);
    }
    return;
}

// Constructs conditional FPtree on currentFptree of value
struct fptree* generate(struct fptree* currentFptree,int value){
    return nullptr;
}

void fpGrowth(struct fptree* tree, vector<int>& prefix){
    
    //TODO: Check single path 
    bool single = false;
    if (single){
       mine(tree,prefix);
       return;
    }
    

    

}


map<int,int> getFrequentElements(string datasetName){
    ifstream inFile;
    inFile.open(datasetName);
    tot_transactions=0;

    set<int> uniqueElements;
    map<int,int> frequencies;
    while(!inFile.eof()){
        string s;
        getline(inFile,s);
        if(s.empty()) break;
        stringstream ss(s);
        int num;
        while(ss>>num){
            uniqueElements.insert(num);
            frequencies[num]+=1;
        }
        tot_transactions++;
    }
    inFile.close();
    vector<pair<int,int>> temp;  // Temporary array to get vector of freqeuent elements in Sorted order  
    for(int x:uniqueElements){
        if(!isFrequent(frequencies[x])){
            frequencies.erase(x);
        }
        else{
            //cout<<x<<' '<<frequencies[x]<<endl;
            temp.push_back({-1*frequencies[x],x});
        }
    }
    sort(temp.begin(),temp.end());
    for(pair<int,int> x:temp){
        frequentElements.push_back(x.second);
    }
    MAXN = frequentElements.size();
    return frequencies;
}

vector<vector<int>> fpt(string datasetName){
    vector<vector<int>> ans;
    ifstream inFile;
    inFile.open(datasetName);
    
    // Tree construction
    Node* root = new Node;
    root->parent = NULL;
    root-> count =0;
    root-> val =-1;

    Node* temp_root = new Node;
    map<Node*,vector<Node*>> adj; // adj[N] = {vector of children of N}
    bool found;

    map<int,int> frequencies =getFrequentElements(datasetName);
    
    while(!inFile.eof()){
        string s;
        getline(inFile,s);
        if(s.empty()) break;
        stringstream ss(s);
        int num;
        temp_root = root;
        // reading Transactions
        vector<pair<int,int>> transaction;
        while(ss>>num){
            if (frequencies.find(num)!=frequencies.end()){
                transaction.push_back({-1*frequencies[num],num});
            }
        }
        sort(transaction.begin(),transaction.end());
        
        // Adding Transaction to the FPTREE
        for(pair<int,int> x:transaction){
           
            // Checking if node is in current Node's childs
            found=false;
            for (Node* child: adj[temp_root]){
                if (child->val==x.second){
                    found = true;
                    temp_root = child;
                    break;
                }
            }

            if (!found){
                // Adding node to the tree
                Node* newNode = new Node;
                newNode->val = x.second;
                newNode->count =0;
                newNode->parent = temp_root;

                adj[newNode] = {};
                adj[temp_root].push_back(newNode);

                temp_root = newNode;
            }

            // updating count of the particular node
            

        }
        temp_root->count +=1;
    }
    inFile.close();

    // cout<<MAXN<<' '<<tot_transactions<<endl;
    // cout<<"FREQUENT ELEMENTS: ";
    // for(auto x:frequentElements){
    //     cout<<x<<" ";
    // }
    // cout<<endl;

    // cout<<"TREE\n";
    // //dfs on tree and call fun()
    // //PRINTING TREE
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
    generateItemsets(MAXN-1,leaves,count,freq,ans);
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