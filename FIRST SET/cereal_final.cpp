#include <iostream>
#include <cstdlib>
#include <algorithm>
using namespace std;

void merge(int arr[], int left, int m, int right)
{
	int i, j, k;
	int n1 = m - left + 1;
	int n2 = right - m;

	// Create temp arrays
	int L[n1], R[n2];

	// Copy data to temp arrays L[] and R[]
	for (i = 0; i < n1; i++)
		L[i] = arr[left + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];

	// Merge the temp arrays back into arr[l..r
	i = 0;
	j = 0;
	k = left;
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	// Copy the remaining elements of L[],
	// if there are any
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	// Copy the remaining elements of R[],
	// if there are any
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
}
void mergeSort(int arr[], int left, int right)
{
	if (left < right) {
		int m = left + (right - left) / 2;

		// Sort first and second halves
		mergeSort(arr, left, m);
		mergeSort(arr, m + 1, right);

		merge(arr, left, m, right);
	}
}

struct mid{
    int value;
    int position;
};
mid search_mid(int arr[], int l, int r)
{
    mid mymid;
    if(l>r){
        mymid.value=mymid.position=-1;
        return mymid;
    }
    int m = l + (r - l) / 2;
    mymid.value=arr[m];
    mymid.position=m;
    return mymid;
}

int max_res=0;

void Max_Med(int a[], int sorted[], int N, int K,int res, int left, int right){
    //Αφού δω όλες τις κ-αδες πρέπει να είναι >= απ'όλα τα median των διαφόρων κ-αδων 
    //Και να έχω βρει μία τουλ. κ-αδα στην οποία ο l μου να είναι όντως ο median

    //Αν καταστρατηγείται η πρώτη συνθήκη δουλεύει η αναδρομή μεγαλώνοντας το l
    //Αν καταστρατηγείται η δεύτερη συνθήκη τότε πρέπει να μικρύνουμε το l
    int median=(K+1)/2;
    if(K>N) {
        max_res=max(res,max_res);
        return;
    }
    
    mid max_mid = search_mid(sorted, left, right);
    
    if(max_mid.value==-1){
        max_res=max(res,max_res);
        return;
    }
    int count_less=0;
    int count_more=0;
    int count_equal=0;
    int temp_res=-1;
    
    
    //  prvth k-ada
    for(int i=0; i<K;i++){
        if(a[i]>max_mid.value)count_more++;
        else if(a[i]<max_mid.value)count_less++;
        else count_equal++;
    }
    
    if(count_more>=(K-median+1)){
        //το l που τσεκαρω τωρα < median τρεχουσας κ-αδας 
        //αρα θελω μεγαλυτερο l-->παμε δεξια
        Max_Med(a,sorted,N,K,res,max_mid.position+1,right);
        return;
    }
    else if(count_less<median && (count_less+count_equal)>=median){
        //ειναι διαμεσος της τρεχουσας κ-αδας
        temp_res=max_mid.value;
    }

   
    //πρέπει να τσεκάρω αν στην θέση π πάω να αγνοήσω(να κάνω override με το νέο στοιχείο)υπήρχε το l μου
    //αν ναι πρέπει να ανανεώσω τον counter των εμφανισεων του l στην κ-αδα
    for(int i=K; i<N; i++){
    
        //σε κάθε νέα κ-αδα τσεκάρουμε ένα νέο στοιχείο(δεν χρειάζεται να δούμε ξανα κατι ό,τι εχόυμε ξαναδει)
        //αν το καινουργιο στοιχείο ειναι μεγαλύτερο του l τοτε έχουμε +1 μεγαλύτερο στοιχείο
        //και -1 μικρότερο
        if(a[i]>max_mid.value)count_more++;
        else if(a[i]<max_mid.value)count_less++;
        else if(a[i]==max_mid.value)count_equal++;

        //κοιτάμε το στοιχείο που πλέον δεν ανήκει στην κ-αδα και φτιάχνουμε τους counters
        if(a[i-K]>max_mid.value) count_more--;
        else if(a[i-K]<max_mid.value)count_less--;
        else if(a[i-K]==max_mid.value)count_equal--;

        //ΣΥΝΘΗΚΕΣ
        if(count_more>=(K-median+1)){
            //το l που τσεκαρω τωρα < median τρεχουσας κ-αδας 
            //αρα θελω μεγαλυτερο l-->παμε δεξια
            Max_Med(a,sorted,N,K,res,max_mid.position+1,right);
            return;
        }
        else if(count_less<median && (count_less+count_equal)>=median){
            //ειναι διαμεσος της τρεχουσας κ-αδας
            temp_res=max_mid.value;
        }
    }

    if(temp_res!=-1){
        if(temp_res>res){
            res=temp_res;
        }
    }
    else{//το l μου δεν ειναι διαμεσος πουθενά αλλα είναι μεγαλύτερο απ'όλες τις διαμέσους των κ-αδων 
    //οποτε πρέπει να μειώσουμε το l-->παμε αριστερα
        Max_Med(a,sorted,N,K,res,left,max_mid.position-1);
        return;
    }
    //τελειωσα με τις κ-αδες ενος l και δοκιμαζω το μεγαλυτερο του
    Max_Med(a,sorted,N,K,res,max_mid.position+1,right);
    max_res=max(res,max_res);
    return;
 
}
int main(){
    int N,K;
    cin>>N>>K;

    int a[N],sorted[N];
    
    for (int i=0;i<N;i++){
        cin>>a[i];
        sorted[i]=a[i];
    }

    mergeSort(sorted, 0, N-1);

    for(int k=K; k<=N; k++){
        Max_Med(a,sorted,N,k,0,0,N-1);
        
    }
    cout<<max_res<<endl;
    return 0;
    
}