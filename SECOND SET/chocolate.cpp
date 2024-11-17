//we will use dp
// C(i,0,t) = max profit using first i prices having made at most t transactions and not having chocolate on day i 
// C(i,1,t) = max profit using first i prices having made at most t transactions and having chocolate on day i 
//
//              /  C(i-1,1,t-1) + price[i] -->we sold chocolate the same morning (so yesterday we had chocolate to sell)
//  C(i,0,t)=max
//              \   C(i-1,0,t) -->we didn't do anything on day i so the situation is the same as the previous day
//
//
//              /  C(i-1,0,t) - price[i] -->we buyed chocolate the same morning(so yesterday we didn't have chocolate)
//             /                            (one transaction is completed when we buy+sell so on day i-1 we had t transaction and on day i we also have t)
//  C(i,1,t)=max
//              \  
//               \ C(i-1,1,t) -->we didn't do anything on day i so the situation is the same as the previous day

#include <iostream>
#include <vector>
using namespace std;


int maxProfit(int N, int K, vector<int>& prices) {
    vector<vector<vector<int> > > C(N, vector<vector<int> >(2, vector<int>(K + 1)));

    //initialization
    for (int i = 0; i < N; i++) {
        C[i][0][0] = 0;
    }
    for (int t = 0; t <= K; t++) C[0][1][t] = -prices[0]; 
    
    int option1=0;
    for (int t = 0; t <= K; ++t) { //we consider K=0 as an extra sol
        for (int i = 1; i < N; ++i) {
            //for C[i][0][t]
            if(t>0) option1=C[i - 1][1][t - 1]+prices[i];
            else option1=0;

            C[i][0][t] = max(option1, C[i - 1][0][t]);

            //for C[i][1][t]
            C[i][1][t] = max(C[i - 1][0][t] - prices[i], C[i - 1][1][t]);
        }
    }
    
    // for (int t = 0; t <= K; ++t) { 
    //         for (int i = 0; i < N; ++i) {
    //             cout<<"C["<<i<<"][0]["<<t<<"]= "<<C[i][0][t]<<endl;
    //             cout<<"C["<<i<<"][1]["<<t<<"]= "<<C[i][1][t]<<endl;
    //         }
    //     }
    // for (int t = 0; t <= K; t++) {
    //     cout<<"C[0][1]["<<t<<"]= "<<C[0][1][t]<<endl;
    // }

    //we find solution for every t in C[n-1][0][t] 
    int max_profit = 0;
    for (int t = 0; t <= K; ++t) {
        max_profit = max(max_profit, C[N - 1][0][t]);
        // cout<<"for t = "<<t<<"max profit ="<<max_profit<<endl;
    }
    return max_profit;
};

int main() {
    int N,K;
    cin>>N>>K;
    vector<int> prices;
    int price=0;
    for(int i=0;i <N;i++){
        cin>>price;
        prices.push_back(price);
    }
    int result = maxProfit(N, K, prices);

    cout << result << endl;

    return 0;
}

