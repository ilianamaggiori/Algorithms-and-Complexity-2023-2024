#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Καθώς σταδιακά οι λεωφόροι κλείνουν, δηλαδή "φεύγουν" ακμές, κάποια στιγμή ο γράφος παύει να είναι συνεκτικός. 
//Αφού η μεταφορά απ την μία πόλη στην άλλη γίνεται ακαριαία αυτό σημαίνει ότι δεν μας νοιάζει η διαδρομή αλλά μόνο η συνεκτικότητα του γράφου, δηλ. να είναι προσβάσιμες όλες οι πόλεις.
// Αναζητώ εγώ λοιπόν το ελάχιστο πλήθος ακμών ώστε ο γράφος να είναι συνεκτικός, με το μέγιστο δυνατό συνολικό βάρος. 
// Απ' αυτές θα απειρίσω τις Κ μικρότερου βάρους ακμές.
// Συνεπώς ψάχνουμε να βρούμε το Maximum spanning tree(αφού μας νοιάζουν οι ακμές με τα μεγάλα βάρη) και στην συνέχεια κάνουμε οδικό έργο στις Κ μικρότερες ακμές.

struct Edge {
    int vertex1;
    int vertex2;
    long int weight;
};


bool compareByWeight(const Edge& a, const Edge& b) {
    return a.weight > b.weight; //θελουμε maximum spanning tree
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


//εφαρμόζουμε τον αλγόριθμο του Kruskal και 
vector<Edge> maxSpanningTree(vector<Edge>& edges,vector<int>& parent,int N) 
{ 
    // Sort all edges 
    sort(edges.begin(), edges.end(),compareByWeight); 
    
    for (int i = 0; i<N; i++){
        parent[i] = i+1;
    }

    int par_u, par_v;
    int M=edges.size();
    vector<Edge> maxSpanningTree;
    for(int i=0; i<M;i++){
        par_u= find(parent,edges[i].vertex1);
        par_v=find(parent,edges[i].vertex2);
        if (par_u != par_v) {
            maxSpanningTree.push_back(edges[i]);
            unite(parent,par_u,par_v);
        }
    }
    return maxSpanningTree;
} 



int main(){
    int N,M,K;
    cin>>N>>M>>K;
    Edge edge;
    vector<Edge> edges;

    for(int i=0;i<M;i++){
        cin>>edge.vertex1>>edge.vertex2>>edge.weight;
        edges.push_back(edge);
    }
    
    vector<int> parent(N);
    vector<Edge> resmaxSpanningTree = maxSpanningTree(edges,parent,N);
    
//    cout << "Edges in the maximum spanning tree:\n";
//     for (const Edge& edge : resmaxSpanningTree) {
//         cout << edge.vertex1 << " - " << edge.vertex2 << " : " << edge.weight << endl;
//     }

    int size_sp_tree= resmaxSpanningTree.size(); //μέγεθος τελικού γράφου

    if(K>=size_sp_tree){
        cout<<"infinity"<<endl;
    }
    else{
        //προσεχουμε οτι ειναι σορταρισμενα απ το μεγαλυτερο στο μικροτερο ομως εμεις θελουμε να "φτιαξουμε" τις Κ μικροτερες ακμες
        cout<<resmaxSpanningTree[size_sp_tree-K-1].weight<<endl;
    }
    return 0;
}
