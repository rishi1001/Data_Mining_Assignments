#include <cstdio>
#include <csignal>
#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include<ctime>

using namespace std;

int x=98;           // this is hard coded, take command line input
vector<vector<int>> ans; // global, can save on interrupt
bool saved=false;
string outputFileName;
time_t initialTime;

bool binary_search(vector<int> &x, vector<vector<int>> &f){
    int n = f.size() - 1;
    int k = 0;
    while(n>0){
        // std::cout << k << " " << n << std::endl;
        if(k+n < f.size() && f[k + n] <= x){
            k+=n;
            if(f[k]==x){
                break;
            }
        }
        else{
            n/=2;   
        }
    }
    return k<f.size() && f[k]==x;
}

//Function to check if small(sorted) is present in big(sorted)
bool check(vector<int>&small, vector<int>&big){ 
    int i=0,j=0;
    int s=small.size(), b=big.size();

    while(i<s && j<b){
        if(small[i]==big[j]){
            i++;
            j++;
        }
        else if(small[i]<big[j]){
            return false;
        }
        else {
            j++;
        }
    }
    
    if (i==s) return true;
    return false;
} 

void writeOutput (string outputFileName, vector<vector<int>>& ans){
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


void checkTime(){
    if (saved) return;
    time_t curr;
    time(&curr);
    //cout<<"IN Checktime, "<<curr-initialTime<<endl;
    if ((curr-initialTime)> 3300){
        saved=true;
        writeOutput(outputFileName,ans);
    }

}
vector<vector<int>> candidate_gen(vector<vector<int>> &f){
   
    vector<vector<int>> c;
    int n=f.size();
    for(int i=0;i<n;i++){
        // 
        checkTime();
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

                // TODO : DOUBT here compare last element to be pushed m

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
                    if(!binary_search(sub,f)){
                        pos=false;
                        break;
                    }
                }
                if(pos){
                    c.push_back(v);
                }
            }
            else{
                break;
            }
        }
    }

    return c;

}

void apriori(string datasetName){
    map<int,int> c1;
    ifstream inFile;
    inFile.open(datasetName);
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
        // DOUBT
        if(val.second*100>=x*n){
            f.push_back({val.first});
        }
    }
    for(int i=0;i<f.size();i++) ans.push_back(f[i]);
    while(!f.empty()){
        vector<vector<int>> c=candidate_gen(f);
        f.clear();
        int tot_c = c.size();
        vector<int> count(tot_c,0);         // Frequency of each candidate
        inFile.open(datasetName);
        // Getting frequency of each candidate
        while(!inFile.eof()){
            string s;
            getline(inFile,s);
            if(s.empty()) break;
            // cout<<"here1\n";
            stringstream ss(s);
            int num;

            // Optimized code
            vector<int> transaction;
            while(ss>>num){
                transaction.push_back(num);
            }
            sort(transaction.begin(),transaction.end());
            
            for(int i=0;i<tot_c;i++){
                if(check(c[i],transaction)) count[i]++;
            }

        }
        for(int i=0;i<tot_c;i++){
            // cout<<val.first<<", "<<val.second<<"\n";
            if(count[i]*100>=x*n){
                f.push_back(c[i]);
            }
        }
        // for(auto i: f){
        //     for(auto j:i) cout<<j<<" ";
        //     cout<<"\n";
        // }
        ans.reserve(ans.size() + f.size());
        ans.insert(ans.end(),f.begin(),f.end());
        inFile.close();
        // break;
    }
}



int main(int argc, char **argv)
{
    // associates standard input with input.txt 
    time(&initialTime);
    string datasetName = argv[1];
    x= stoi(argv[2]);
    outputFileName = argv[3];
    apriori(datasetName);
    writeOutput(outputFileName,ans);
    
    return 0;
}