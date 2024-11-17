#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <climits>
using namespace std;


//συνάρτηση για να βρίσκει τον κόμβο με την μικρότερη απόσταση από αυτούς που είναι ακόμα unprocessed
int minDistance(vector<int>& dist, bool sptSet[], int N) {
    int min = INT_MAX, min_index;
    for (int i = 0; i < N; i++)
        if (sptSet[i] == false && dist[i] <= min)
            min = dist[i], min_index = i;
    return min_index;
}


void dijkstra(vector<vector<int> >& graph, int src, vector<int>& dist, int N, int startingTime, vector<int>& president_parent, vector<int>& president_timestamps) {

    bool sptSet[N]; // sptSet[i] θα είναι true αν ο κόμβος i είναι included στο shortest path tree ή η ελάχιστη απόσταση του i από το source έχει οριστικοποιηθεί


    //Αρχικοποίηση όλων των κόμβων ως unvisited (η αρχικοποίηση των distances γίνεται στην main με το όρισμα του vector)
    for (int i = 0; i < N; i++){
        sptSet[i] = false;
    }
    //"Απόσταση" από το source
    //Σε αντίθεση με τον κανονικό Dijkstra, η απόσταση του source από το source δεν είναι πλέον 0 αλλά η χρονική στιγμή εκκίνησης Τ που μας δίνεται 
    dist[src-1] = startingTime;

    // Βρίσκουμε το shortest path για όλους τους κόμβους
    for (int i = 0; i< N-1; i++) {
        int u = minDistance(dist, sptSet, N);  //παίρνουμε τον κόμβο με την μικρότερη απόσταση από αυτούς που είναι ακόμα unprocessed
        
        sptSet[u] = true; // Κάνουμε mark τον κόμβο ως processed

        //παιρνουμε ολους τους γειτονες του u και κάνουμε update τα βάρη αναλόγως
        for (int v = 0; v < N; v++)
            if (!sptSet[v] && graph[u][v]){
                int weight_v = graph[u][v];
                //ελεγχουμε αν η ακμη με τον γειτονα (u,v) χρησιμοποιειται απ τον προεδρο, αν ναι θα πρεπει να προσεξουμε την αναμονη και να αλλαξουμε το weight
                //1Η ΠΕΡΙΠΤΩΣΗ: Η ακμή χρησιμοποιείται απ τον πρόεδρο με την ιδια φορα με εμας
                if(president_parent[v]==u){
                    //αν οταν μπω στον u ο προεδρος δεν εχει βγει απο τον v τοτε περιμενω
                    if(dist[u]>= president_timestamps[u] && dist[u] < president_timestamps[v]){
                        weight_v = weight_v + (president_timestamps[v]- dist[u]); //το παλιό βάρος/timestamp της ακμής αυξάνεται κατά την αναμονή που πρέπει να κάνουμε
                    }
                }
                //2Η ΠΕΡΙΠΤΩΣΗ: Η ακμή χρησιμοποιείται απ τον πρόεδρο με την αντίθετη φορά από εμάς
                if(president_parent[u]==v){
                    //αν οταν μπω στον u ο προεδρος δεν εχει βγει απο τον u τοτε περιμενω
                    if(dist[u]>= president_timestamps[v] && dist[u] < president_timestamps[u]){
                        weight_v = weight_v + (president_timestamps[u]- dist[u]);
                    }
                }
                //3Η ΠΕΡΙΠΤΩΣΗ: Η ακμή δεν χρησιμοποιείται από τον πρόεδρο οπότε η ανανέωση του βάρους της ακμής γίνεται όπως στον κλασικό Dijkstra

                //Αυξάνω το βάρος της ακμής κατάλληλα
                if(dist[u]+ weight_v < dist[v]){
                    dist[v] = dist[u] + weight_v;
                }
            }
    }
}
 


int main(){
    int N,M;
    cin>>N>>M;

    int A,B,T,K;
    cin>>A>>B>>T>>K;

    vector<int> president_path;
    int pres_node;

    for(int i=0; i<K; i++){
        cin>>pres_node;
        president_path.push_back(pres_node);
    }

    vector<vector<int> > Graph(N, vector<int>(N, 0)); 
    int vertex1,vertex2,weight;
    for(int i=0;i<M;i++){
        cin>>vertex1>>vertex2>>weight;
        Graph[vertex1-1][vertex2-1]=weight;
        Graph[vertex2-1][vertex1-1]=weight;
    }

    //κρατάμε τον πατέρα κάθε κόμβου που περνάει ο πρόεδρος για να βρίσκουμε την φορά κίνησής του
    vector<int> president_parent(N, -1);
    //αποθηκευουμε τα timestamps που θα βρίσκεται ο προεδρος σε καθε κόμβο της διαδρομής του
    vector<int> president_timestamps(N, -1);

    int timeStamp=0;
    for(int i=1; i<K; i++){
        president_parent[president_path[i]-1]= president_path[i-1]-1;
        timeStamp+=Graph[president_path[i]-1][president_path[i-1]-1];
        president_timestamps[president_path[i]-1]= timeStamp;
    }

    // for(int i=0; i<N; i++){
    //     cout<<"timestamp of "<< i+1 <<"is "<< president_timestamps[i]<<endl;
    // }
    // for (const auto& pair : president_timestamps) {
    //     cout << "(" << pair.first << ", " << pair.second << ")" << endl;
    // }

//    for(int i=0; i<N; i++){
//         cout<<"parent of "<< i+1 <<"is "<< president_parent[i]<<endl;
//     }
    //ο πίνακας distances θα είναι και η απόσταση(χρονικά) κάθε κόμβου από το source 
    //οπότε στην θέση B-1 θα περιέχει το ζητούμενο
    vector<int> distances(N, INT_MAX); 

    dijkstra(Graph, A, distances, N, T, president_parent, president_timestamps);
    int sol = distances[B-1];
    cout<<sol-T<<endl;
    //cout<<sol-A<<endl;
    // // Print shortest distances
    // cout << "Shortest distances from node " << A << ":\n";
    // for (int i = 0; i < N; ++i) {
    //     cout << "To node " << i+1 << ": ";
    //     cout << distances[i] << "\n";
    // }



    // for (const Edge& edge : edges) {
    //     cout << edge.vertex1 << " - " << edge.vertex2 << " : " << edge.weight << endl;
    // }
    // for (const int& pres_node : president_path) {
    //     cout << pres_node<< endl;
    // }
    return 0;
}