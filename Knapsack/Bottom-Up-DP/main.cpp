#include <vector>
#include <iostream>
using namespace std;

int knapsack(int W, vector<int> &val, vector<int> &wt) {

    int n = wt.size();//toplam esya sayisi

    //dp tablosunu olusturur
    //satirlar: degerlendirilen esya sayisi
    //sutunlar: canta kapasitesi
    vector<vector<int>> dp(n + 1, vector<int>(W + 1));


    //d tablosunu kucuk problemlerin sonuclarindan badlayarak bottom-up seklinde doldurur
    for (int i = 0; i <= n; i++) {

        for (int j = 0; j <= W; j++) {
            //hic esya yoksa veya cantanin kapasitesi 0 ise elde edilebilecek maksimum deger 0
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else {
                int pick = 0;//esyayi alma durumunun sonucu

                //esyanin agirligi mevcut kapasiteye sigiyorsa esyayi alir
                if (wt[i - 1] <= j)
                    pick = val[i - 1] + dp[i - 1][j - wt[i - 1]];

                //esyayi almama durumunun sonucu
                int notPick = dp[i - 1][j];

                dp[i][j] = max(pick, notPick);//esyayi alma ve almama durumlarindan max olani secer
            }
        }
    }
    //elde edilen tum max degerleri dondurur
    return dp[n][W];
}

int main() {
    vector<int> val = {1, 2, 3};//degerler
    vector<int> wt = {4, 5, 1};//agirlilar
    int W = 4;//max kapasite
    cout << knapsack(W, val, wt) << endl;
    return 0;
}