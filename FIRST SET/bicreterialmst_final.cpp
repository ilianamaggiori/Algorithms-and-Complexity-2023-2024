#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int gcd(int a, int b)
{
    // Find Minimum of a and b
    int result = min(a, b);
    while (result > 0) {
        if (a % result == 0 && b % result == 0) {
            break;
        }
        result--;
    }
 
    // Return gcd of a and b
    return result;
}

float bs_c(float left, float right){
    if(left>right){
        return -1;
    }
    return (right+left)/2;
}


struct Edge {
    int vertex1;
    int vertex2;
    float profit;
    float weight;
    float pcw;
};

struct Kruskal_res{
    int value; // -1 or 1
    int sp;
    int sw;
};

bool compareByP_CW(const Edge& a, const Edge& b) {
    return a.pcw > b.pcw;
}


// Find the root of the set to which 'i' belongs
//γρήγορη union find(κάθε φορά που βρίσκει τον αρχικό πατέρα αλλάζει και τον πίνακα parent
//για να μην χρειαστεί μετά να τα ξαναδιατρέξει όλα)
int find(vector<int>& parent,int i) {
    if (parent[i-1] == i) {
        // return parent[i-1];
        return i;
    }
    else{
        parent[i-1]=find(parent,parent[i-1]);
        return parent[i-1];
        // return find(parent,parent[i-1]);
    }
}


void unite(vector<int>& parent,int u, int v){
    parent[u-1]=parent[v-1];
}


Kruskal_res kruskal(vector<Edge>& edges,vector<int>& parent, float c) 
{ 
    // Sort all edges 
    sort(edges.begin(), edges.end(),compareByP_CW); 
    Kruskal_res res;
    int par_u, par_v;
    int ans_p=0;
    int ans_w=0;
    int M=edges.size();
    for(int i=0; i<M;i++){
        par_u= find(parent,edges[i].vertex1);
        par_v=find(parent,edges[i].vertex2);
        if (par_u != par_v) {
            unite(parent,par_u,par_v);
            ans_p+=edges[i].profit;
            ans_w+=edges[i].weight;
        }
    }
    
    if(ans_p-c*ans_w<0){
        res.value=-1;
        res.sp=-1;
        res.sw=-1;
    }
    else{
        res.value=1;
        res.sp=ans_p;
        res.sw=ans_w;
    }
    return res;
    //cout <<ans_p/gcd(ans_p,ans_w)<<" "<<ans_w/gcd(ans_p,ans_w) <<endl; 
} 

Kruskal_res result;
Kruskal_res previous_result;
Kruskal_res final_result;

void max_tree(vector<Edge>& edges,vector<int>& parent,int N,float l, float r,float previous_c){
    float c= bs_c(l,r);
    if(c==-1 || abs(c-previous_c)<=0.001){
        if(result.value==-1){
            final_result=previous_result;
        }
        else {
            final_result=result;
        }
        return;
    }

    int M=edges.size();
    for(int i=0;i<M;i++){
        edges[i].pcw= edges[i].profit - c*edges[i].weight;
    }
    for (int i = 0; i <N; i++){
        parent[i] = i+1;
    }
    if(result.value!=-1){
        previous_result=result;
    }
    
    result = kruskal(edges,parent,c);
    if(result.value==1){
        max_tree(edges,parent,N,c,r,c);
        return;
    }
    else{
        max_tree(edges,parent,N,l,c,c);
        return;
    }    
}
int main(){
    int N,M;
    cin>>N>>M;
    Edge edge;
    vector<Edge> edges;
    float c_min=0; // min(p/w)
    float c_max=0; // max(p/w)
    for(int i=0;i<M;i++){
        cin>>edge.vertex1>>edge.vertex2>>edge.profit>>edge.weight;
        if(edge.profit/edge.weight > c_max)c_max=edge.profit/edge.weight;
        if(i==0)c_min=edge.profit/edge.weight;
        else if(edge.profit/edge.weight < c_min)c_min=edge.profit/edge.weight;
        edges.push_back(edge);
    }
    
    vector<int> parent(N);

    max_tree(edges,parent,N,c_min,c_max,0);
    cout<<final_result.sp/gcd(final_result.sp,final_result.sw)<<" "<<final_result.sw/gcd(final_result.sp,final_result.sw)<<endl;
 
    return 0;
}
