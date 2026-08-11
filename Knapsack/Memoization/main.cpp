#include <iostream>
#include <vector>
using namespace std;

// Memoization kullanarak 0/1 Knapsack problemini çözer
int ksMemo(int weights[], int values[], int n, int capacity, vector<vector<int>> &dp) {

    //base case
    if (n == 0 || capacity == 0)
        return 0;

    //bu problem daha once cozulduyse sonucu dogrudan dp tablosundan al
    if (dp[n][capacity] != -1)
        return dp[n][capacity];

    //esyayi alma durumunu hesapla
    int exclude = ksMemo(weights, values, n - 1, capacity, dp);

    //esyayi alma durumunu hesapla
    int include = 0;
    if (weights[n - 1] <= capacity)
        include = values[n - 1] +ksMemo(weights, values, n - 1,capacity - weights[n - 1], dp);

    //esyayi alma ve almama durumlarindan maksimum olan sonucu dp tablosuna kaydet
    dp[n][capacity] = max(include, exclude);

    //hesaplanan sonucu dondur
    return dp[n][capacity];
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
    cout << "\nMax Weight Capacity: "<< capacity << endl;

    int n = 6;

    //memoization tablosunu olustur(baslangicta tum degerler:-1)
    //-1:bu alt problem henuz hesaplanmadi
    vector<vector<int>> dp(
        n + 1,
        vector<int>(capacity + 1, -1)
    );
    cout << "Maximum value: "<< ksMemo(weights, values, n, capacity, dp)<< endl;
    return 0;
}