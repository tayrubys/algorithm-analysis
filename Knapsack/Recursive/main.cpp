#include <iostream>
using namespace std;

int ksRecur(int weights[], int values[], int n, int capacity){
    //base case:
    if (n == 0 || capacity == 0)
        return 0;

    //esyayi alma
    int exclude = ksRecur(weights, values, n - 1, capacity);
    
    //esya cantaya sigiyorsa al
    int include = 0;
    if (weights[n - 1] <= capacity)
        include = values[n - 1] + ksRecur(weights, values, n - 1, capacity - weights[n - 1]); 
   
   //iki durumdan maksimum olanı dondur
   return max(include, exclude);             
}

int main() {
    int weights[] = {1, 2, 3, 6, 7, 4};
    cout << "Weights: ";
    for (int i = 0; i < 6; i++)
        cout << weights[i] << " ";

    int values[] = {10, 20, 25, 40, 60, 70};
    cout << "\nValues: ";
    for (int i = 0; i < 6; i++)
        cout << values[i] << " ";

    int capacity = 7;
    cout << "\nMax Weight Capacity: " << capacity << endl;

    int n = 6;
    cout << "Maximum value: "
         << ksRecur(weights, values, n, capacity)
         << endl;

    return 0;
}
