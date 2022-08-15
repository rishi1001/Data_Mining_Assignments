#include <cstdio>
#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

int x=1;

vector<vector<int>> candidate_gen(vector<vector<int>> &f){
    // for(auto i: f){
    //     for(auto j:i) cout<<j<<" ";
    //     cout<<"\n";
    // }
    vector<vector<int>> c;
    int n=f.size();
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            int n1=f[i].size();
            bool intersect=true;
            for(int k=0;k<n1-1;k++){
                if(f[i][k]!=f[j][k]){
                    intersect=false;
                    break;
                }
            }
            if(intersect){
                vector<int> v;
                for(int k=0;k<n1;k++){
                    v.push_back(f[i][k]);
                }
                v.push_back(f[j][n1-1]);
                int vn=v.size();
                bool pos=true;
                for(int z=0;z<vn;z++){
                    vector<int> sub;
                    for(int z1=0;z1<vn;z1++){
                        if(z1==z) continue;
                        sub.push_back(v[z1]);
                    }
                    bool found=false;
                    for(int i1=0;i1<n;i1++){            // optimise this
                        if(sub==f[i1]){
                            found=true;
                            break;
                        }
                    }
                    if(!found){
                        pos=false;
                        break;
                    }
                }
                if(pos){
                    c.push_back(v);
                }
            }
        }
    }

    return c;

}

vector<vector<int>> apriori(){
    vector<vector<int>> ans;
    map<int,int> c1;
    ifstream inFile;
    inFile.open("my_test.dat");
    int n=0;
    while(!inFile.eof()){
        string s;
        getline(inFile,s);
        if(s.empty()) break;
        stringstream ss(s);
        int num;
        while(ss>>num){
            c1[num]++;
        }
        n++;
    }
    inFile.close();
    vector<vector<int>> f;
    for(pair<int,int> val: c1){
        if(val.second*100>=x*n){
            f.push_back({val.first});
        }
    }
    for(int i=0;i<f.size();i++) ans.push_back(f[i]);
    int iter=0;
    while(!f.empty()){
        vector<vector<int>> c=candidate_gen(f);
        f.clear();
        
        if(iter==2) break;
        iter++;
        int tot_c = c.size();
        map<int,int> count;
        inFile.open("my_test.dat");
        while(!inFile.eof()){
            string s;
            getline(inFile,s);
            if(s.empty()) break;
            // cout<<"here1\n";
            stringstream ss(s);
            int num;
            set<int> transaction;
            while(ss>>num){
                transaction.insert(num);
            }
            // for(auto i: transaction) cout<<i<<" ";
            // cout<<"transaction done\n";
            for(int i=0;i<tot_c;i++){
                bool present=true;
                for(int j: c[i]){
                    if(transaction.find(j)==transaction.end()){
                        present=false;
                        break;
                    }
                }
                if(present) count[i]++;
            }
        }
        for(pair<int,int> val: count){
            // cout<<val.first<<", "<<val.second<<"\n";
            if(val.second*100>=x*n){
                f.push_back(c[val.first]);
            }
        }
        // for(auto i: f){
        //     for(auto j:i) cout<<j<<" ";
        //     cout<<"\n";
        // }
        for(int i=0;i<f.size();i++) ans.push_back(f[i]);
        inFile.close();
        // break;
    }

    return ans;


}

int main()
{
    // associates standard input with input.txt 
    // ifstream inFile;
    // inFile.open("my_test.dat");
    // while(!inFile.eof()){
    //     string x;
    //     getline(inFile,x);
    //     stringstream ss(x);
    //     int num;
    //     vector<int> v;
    //     while(ss>>num){
    //         v.push_back(num);
    //     }
    //     // inFile>>x;
    //     // if(x=='\n') {
    //     //     cout<<"\n";
    //     // }
    //     cout<<v.size()<<": ";
    //     for(auto i:v) cout<<i<<",";
    //     cout<<"\n";
    // }
    // int x;
    // // reads the input.txt file and stores in string x
    // // getline(cin, x);
    // inFile>>x;
    // // prints string x in output.txt file
    // cout << x << "\n";

    // inFile>>x;
    // // getline(cin, x);
    // // prints string x in output.txt file
    // cout << x; 

    vector<vector<int>> ans=apriori();
    set<vector<int>> sorted_ans;
    for(auto i: ans){
        sorted_ans.insert(i);
    }
    for(auto i:sorted_ans){
        for(auto j:i) cout<<j<<" ";
        cout<<"\n";
    }
    
    return 0;
}