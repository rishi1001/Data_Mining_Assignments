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
#include <csignal>
using namespace std;

int x=98;           // this is hard coded, take command line input
int tot_transactions=0;          // tot transactions (update it)
vector<vector<int>> ans;   // GLOBAL VECTOR FOR ALL TRANSACTIONS
string outputFileName;
struct Node{
    int val;
    Node *parent = nullptr;
    Node *next = nullptr;
    int count;          // this count stores the total count which end at this particular node
};

struct fptree
{
    Node *root = nullptr;
    map<int,Node*> headerTable;
    bool single=true;
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
    for(int i=1;i<tot_n;i++){               // from 1 because not pushing empty set
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

// TODO: Check if tree is empty
// Returns True if tree is empty
bool isEmpty(struct fptree* tree){
    if(tree->headerTable.empty()) return true;
    return false;
}

// Constructs conditional FPtree on currentFptree of value
// TODO try const for currentFptree
struct fptree* generate(struct fptree* currentFptree,int value){
    
    // First we will generate frequencies of each element
    map<int,int> frequencies;
    Node *current = currentFptree->headerTable[value];
    Node *temp;
    while(current!=nullptr){
        int count = current->count;   // This will added for each ancestor
        temp=current->parent;         // Now go up to the root
        while(temp != currentFptree->root){
            frequencies[temp->val]+=count; 
            temp=temp->parent;
        }
        current= current->next;
    }


    struct fptree* conditionalTree = new fptree;
    // Root of the newFPtree
    Node*  root = new Node;
    root->val = -1;
    root->parent = nullptr;
    root->count = 0;
    root->next= nullptr;
    conditionalTree->root = root;
    conditionalTree->single=true;
    
    // Adjacency List 
    map<Node*,vector<Node*>> adj;
    map<int,Node*> lastOccurence;

    current = currentFptree->headerTable[value];
    while(current!=nullptr){
        int count = current->count;   // This will added for each ancestor
        temp=current->parent;         // Now go up to the root
        vector<pair<int,int>> transaction;
        while(temp != currentFptree->root){    
            if(isFrequent(frequencies[temp->val])){
                transaction.push_back({-1*frequencies[temp->val],temp->val});
            }
            temp=temp->parent;
        }
        current= current->next;

        // Now pushing transaction to newFPtree
        sort(transaction.begin(),transaction.end());
        Node* temp_root = root;
        bool found;
        for(pair<int,int> x:transaction){
            // Checking if node is in current Node's childs
            found=false;
            for (Node* child: adj[temp_root]){
                if (child->val==x.second){
                    found = true;
                    temp_root = child;
                    temp_root->count += count;
                    break;
                }
            }

            if (!found){
                // Adding node to the tree
                Node* newNode = new Node;
                newNode->val = x.second;
                newNode->count =count;
                newNode->parent = temp_root;
                newNode->next=nullptr;

                // Update Next pointers
                if (lastOccurence.find(x.second)!=lastOccurence.end()){
                    // Found
                    lastOccurence[x.second]->next=newNode;
                }else{
                    //First found so update header table
                    conditionalTree->headerTable[x.second]=newNode;
                }
                lastOccurence[x.second]=newNode;

                // Updating adjacency matrix
                adj[newNode] = {};
                adj[temp_root].push_back(newNode);
                if(adj[temp_root].size()>1) conditionalTree->single=false;
                temp_root = newNode;
            }


        }
    }
    return conditionalTree;

    


}

void fpGrowth(struct fptree* tree, vector<int>& prefix){
    
    //TODO: Check single path 
    if (tree->single){
       mine(tree,prefix);
       return;
    }
    
    for(pair<int,Node*> x: tree->headerTable){
        int num_times=0;                // sum of count of x.first in tree 
        struct Node* temp=x.second;     // temp to iterate in nodes having val x.first
        while(temp!=nullptr){
            num_times+=temp->count;
            temp=temp->next;
        }
        if (isFrequent(num_times)){     // if x.first is frequent
            prefix.push_back(x.first);
            struct fptree* conditionalTreeOnX = generate(tree,x.first);
            ans.push_back(prefix);

            if (!isEmpty(conditionalTreeOnX)){
                fpGrowth(conditionalTreeOnX,prefix);
            }
            prefix.pop_back();
        }   
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
    for(int x:uniqueElements){
        if(!isFrequent(frequencies[x])){
            frequencies.erase(x);
        }
    }
    return frequencies;
}

void fpt(string datasetName){
    vector<vector<int>> ans;
    ifstream inFile;
    inFile.open(datasetName);
    
    // Tree construction
    struct fptree* tree = new fptree;
    Node* root = new Node;
    root->parent = nullptr;
    root-> count =0;
    root-> val =-1;
    root->next = nullptr;
    tree->root=root;
    tree->single=true;

    Node* temp_root = new Node;
    map<Node*,vector<Node*>> adj; // adj[N] = {vector of children of N}
    map<int,Node*> lastOccurence;
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
                    temp_root->count+=1;
                    break;
                }
            }

            if (!found){
                // Adding node to the tree
                Node* newNode = new Node;
                newNode->val = x.second;
                newNode->count =1;
                newNode->parent = temp_root;

                adj[newNode] = {};
                adj[temp_root].push_back(newNode);
                if(adj[temp_root].size()>1) tree->single=false;

                temp_root = newNode;

                // Update Next pointers
                newNode->next=nullptr;
                if (lastOccurence.find(x.second)!=lastOccurence.end()){
                    // Found
                    lastOccurence[x.second]->next=newNode;
                }else{
                    //First found so update header table
                    tree->headerTable[x.second]=newNode;
                }
                lastOccurence[x.second]=newNode;
            }
        }
    }
    inFile.close();


    


    // CALLING FPGROWTH
    vector<int> prefix;
    fpGrowth(tree,prefix);


}


void writeOutput (string outputFileName){
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

void signalHandler( int signum ) {
    cout << "Interrupt signal (" << signum << ") received.\n";
    cout << "Please wait, saving file.\n";
    writeOutput(outputFileName);    
    exit(signum);  
}

void registerSignals(){
    signal(SIGINT, signalHandler);              // ctrl + c
    // signal(SIGTERM, signalHandler);
    // signal(SIGQUIT, signalHandler);
}

int main(int argc, char **argv)
{
    registerSignals();

    string datasetName = argv[1];
    x= stoi(argv[2]);
    outputFileName = argv[3];

    fpt(datasetName);
    writeOutput(outputFileName);
    
    return 0;
}