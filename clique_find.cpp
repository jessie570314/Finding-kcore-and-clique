
  
#include <vector>
#include <csignal>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <list>
#include <time.h>
#define VERT_NUM  82168

using namespace std;
vector<int> cur_best_clique;
vector<int> coreness;
vector<vector<int> > graph(VERT_NUM);
vector<vector<int> > graphd(VERT_NUM);
vector<vector<int> > D(VERT_NUM);
vector<int> dv;
vector<int> dv1;
vector<int> hlist;
vector<int> kco(VERT_NUM);
vector<int> L(VERT_NUM);
vector<int> color;

vector<int> my_set_intersection(vector<int> &a , vector<int> &b){
    vector<int> c(a.size() + b.size());
    vector<int>::iterator it = set_intersection(a.begin(),a.end(),b.begin(),b.end(),c.begin());
    c.resize(it - c.begin());
    return c;
}

void easy_clique_finder(vector<vector<int> > &graph , vector<int> &cur_candidate , vector<int> &cur_clique){
    if(cur_best_clique.size()==1500)return;
    for(int i = 0 ; i < cur_candidate.size() ; i ++){
        int v = cur_candidate[i];
        vector<int> new_candidate = my_set_intersection(cur_candidate , graph[v]);
        cur_clique.push_back(v);
        
        if( cur_clique.size() > cur_best_clique.size() ){
            cur_best_clique = cur_clique;   // store the largest clique you can find
        }
        if(cur_best_clique.size()==1500)return;
        easy_clique_finder( graph , new_candidate , cur_clique );
        cur_clique.pop_back();
    }
    return ;
}

void clique_finder(vector<int> &cur_candidate,vector<int> &cur_clique, vector<int> &colorused, int s){
    if(cur_best_clique.size()==1500)return;
    for(int i = 0 ; i < cur_candidate.size() ; i++){
        int v = cur_candidate[i];
        colorused[color[v]]=1;
        vector<int> new_candidate = my_set_intersection(cur_candidate , graphd[v]);
        vector<int> new_can;
        for(int j=0;j<new_candidate.size();j++){
            if(colorused[color[new_candidate[j]]]!=1 && new_candidate[j]>s){
                new_can.push_back(new_candidate[j]);
            }
        }
        cur_clique.push_back(v);
        if( cur_clique.size() > cur_best_clique.size() ){
            cur_best_clique = cur_clique;// store the largest clique you can find
        }
        if(cur_best_clique.size()==1500)return;
        clique_finder( new_can,  cur_clique ,colorused,cur_candidate[i]);
        colorused[color[v]]=0;
        cur_clique.pop_back();
    }
    return ;
}

void coloring(vector<vector<int> > &graph ,vector<int > &list){
    color.assign(82168,-1);
    color[list[0]] = 0;
    bool availablecolor[VERT_NUM];
    for(int i=0;i<VERT_NUM;i++){
        availablecolor[i]=false;
    }
    int j=list[1];
    int a = 1;
    while(j<VERT_NUM){
        for(int i=0;i<graphd[j].size();i++){
            if(color[graphd[j][i]] != -1){
                availablecolor[color[graphd[j][i]]] = true;
            }
        }
        int z;
        for ( z = 0; z < VERT_NUM; z++){
            if (availablecolor[z] == false)break;
        }
        color[j] = z;
        for(int i=0;i<graphd[j].size();i++){
            if(color[graphd[j][i]] != -1){
                availablecolor[color[graphd[j][i]]] = false;
            }
        }
        if(a<list.size()-1)j = list[++a];
        else j = VERT_NUM+1;
    }
    
}

void k_core(vector<vector<int> > &graph ){
    for(int i=0;i<VERT_NUM;i++){
        D[dv[i]].push_back(i);
        L[i]=0;
        kco[i]=0;
    }
    
    for(int i = 0;i<VERT_NUM;i++){//degree
        if(D[i].size()==0)continue;
        for(int j=0;j<D[i].size();j++){//vertax this degree
            if(dv[D[i][j]]!=i)continue;
            if(L[D[i][j]]==1)continue;
            for(int z = 0;z < graph[D[i][j]].size();z++){
                if(L[graph[D[i][j]][z]] == 1 || dv[graph[D[i][j]][z]]==i)continue;
                dv[graph[D[i][j]][z]]-=1;
                D[dv[graph[D[i][j]][z]]].push_back(graph[D[i][j]][z]);
            }
            L[D[i][j]]=1;
            kco[D[i][j]]=i;
        }
    }
    return ;
}

void signalHandler( int signum ) {
    cout<<"Get signal"<<signum<<endl;
    fstream out;
    out.open("clique.txt" , ios::out);
    sort(cur_best_clique.begin() , cur_best_clique.end());
    for(int v : cur_best_clique){
        out<<v<<endl;
    }
    exit(signum);
}


int main(int argc , char *argv[]){
    signal(SIGINT, signalHandler);
    freopen(argv[1] , "r" , stdin);
    int K = stoi(argv[2]);
    vector<int> candidates, cur_clique;
    
    int A , B;
    dv.assign(82168,0);
    while(cin >> A >> B){
        if(A == B)continue;
        graph[A].push_back(B);
        graph[B].push_back(A);
        dv[A]++;
        dv[B]++;
    }
    for(int i = 0 ; i < VERT_NUM ; i++){
        sort(graph[i].begin(), graph[i].end());
    }
    k_core(graph);
    fstream out;
    out.open("kcore.txt" , ios::out);
    for(int i=0;i<VERT_NUM;i++){
        if(kco[i]>=K){
            out<<i<<" "<<kco[i]<<endl;
        }
        if(kco[i]>=1499){
            hlist.push_back(i);
        }
    }
    
    for(int i=0;i<hlist.size();i++){
        candidates.push_back(hlist[i]);
        for(int j=0;j<graph[hlist[i]].size();j++){
            if(kco[graph[hlist[i]][j]]>=1499){
                graphd[hlist[i]].push_back(graph[hlist[i]][j]);
            }
        }
    }
   
    coloring(graphd ,hlist);
    vector<int>colorused;
    colorused.assign(VERT_NUM, 0);
    //easy_clique_finder(graphd , candidates , cur_clique);
    clique_finder(candidates, cur_clique, colorused,-1);
    fstream out1;
    out1.open("clique.txt" , ios::out);
    sort(cur_best_clique.begin() , cur_best_clique.end());
    for(int v : cur_best_clique){
        out1<<v<<endl;
    }
    
    
}

