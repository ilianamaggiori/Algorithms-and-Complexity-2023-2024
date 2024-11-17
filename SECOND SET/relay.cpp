// we will use again dp

//             / p(i) + s(i)d(i) , if i deliver myself the letters to the root       \
// cost(i)=min/                                                                   ----\
//            \                                                                   ----/
//             \                                                                     /
//              \ p(i) +min( s(i)(d(i)-d(j)) + cost(j)) ,if i use a neighbor during the route
//                     0<=j<i
//                    where j is ancestor


//             / p(i) + s(i)d(i)
// cost(i)=min/
//             \
//              \ p(i) + s(i)d(i) + min(cost(j) - d(j)s(i))
//                                   j


#include <iostream>
#include <vector>
using namespace std;

struct Node {
    int p, s;
};

//we have to evaluate the distance of every node from root as e are given only the distance between the nodes
void get_distance_from_root(vector<int>& distance_from_root, vector<vector<int> >& adj, int parent, int total_nodes) {
    // Traverse neighbors
    for (int i = 0; i < total_nodes; i++) {
        if (adj[parent][i] != 0) { //
            if (distance_from_root[i] == -1) { //check if we already evaluated the distance of a node
                distance_from_root[i] = adj[parent][i] + distance_from_root[parent];
                get_distance_from_root(distance_from_root, adj, i, total_nodes);
            }
        }
    }
}

void MinCost(vector<int>& distance_from_root, vector<vector<int> >& adj,vector<int> &Cost,vector<int>& Ancestors, int current, int total_nodes,  vector<Node>& nodes){
    
    for(auto j:Ancestors){
        Cost[current]=min(Cost[current], (nodes[current].p + nodes[current].s * distance_from_root[current] + Cost[j] - distance_from_root[j]* nodes[current].s));
    }
    //for every node we look at(doing a sort of DFS) we put it in a "stack" and after we evaluated the min cost for it we pop it from the stack
    //in this way every time we evaluate the min cost for a Vi the others of the stack are surely its ancestors 
    Ancestors.push_back(current);
    // for(auto k:Ancestors){
    //     cout<<"Ancestors: "<<k<<" ";
    // }
    // cout<<endl;
    for(int i=0; i<total_nodes; i++){
        //call min cost for every neighbor
        if(adj[current][i] != 0){
            if(distance_from_root[i]>distance_from_root[current]){ //we avoid infinite loops because without this check we would look all the time at the two nodes of an edge
                MinCost(distance_from_root,adj,Cost,Ancestors,i,total_nodes,nodes);
            }
        }
    }
    Ancestors.pop_back();
}


int main() {
    int total_nodes;
    cin >> total_nodes;

    vector<Node> nodes(total_nodes);
    vector<vector<int> > adj(total_nodes, vector<int>(total_nodes, 0));


    // Input edges
    for (int i = 0; i < total_nodes - 1; i++) {
        int node1, node2, distance;
        cin >> node1 >> node2 >> distance;
        adj[node1-1][node2-1] = distance;
        adj[node2-1][node1-1] = distance;
    }

    nodes[0].p = 0;
    nodes[0].s = 0;

    // Input nodes
    for (int i = 1; i < total_nodes; i++) {
        cin >> nodes[i].p >> nodes[i].s;
    }

    vector<int> distance_from_root(total_nodes, -1);
    distance_from_root[0]=0;
    get_distance_from_root(distance_from_root, adj, 0, total_nodes);

    // // Output distances
    // for (int i = 0; i < total_nodes; i++) {
    //     cout << "Distance of node " << i + 1 << " from root is: " << distance_from_root[i] << endl;
    // }

    //the ancestors of the specific node we are looking
    vector<int> Ancestors;
    vector<int> Cost(total_nodes, INT32_MAX);
    Cost[0]=0;
    MinCost(distance_from_root,adj,Cost,Ancestors,0,total_nodes,nodes);

    cout<<Cost[1];
    for(auto k= Cost.begin()+2 ; k!=Cost.end(); k++){
        cout<<" "<<*k;
    }
    cout<<endl;
    return 0;
}