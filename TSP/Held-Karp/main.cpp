#include <bits/stdc++.h>
using namespace std;

pair<long long, vector<int>> heldKarp(const vector<vector<long long>>& dist) {
    int n = dist.size();
    int N = 1 << n;//olusturulabilecek alt kümelerin sayisi
    const long long INF = LLONG_MAX / 4;

    // dp[mask][j] =
    //0 numarali sehirden baslayip mask icerisindeki sehirleri ziyaret ederek
    //j sehrinde bitmenin minimum maliyeti
    vector<vector<long long>> dp(N, vector<long long>(n, INF));

    //parent[mask][j] = (mask, j) durumu icin en iyi onceki sehri tutar
    vector<vector<int>> parent(N, vector<int>(n, -1));

    //baslangic durumu:yalnizca 0 numarali sehir ziyaret edildi ve maliyet 0
    dp[1][0] = 0;

    //tablo doldurma
    for (int mask = 1; mask < N; ++mask) {

        //turun icinde 0 numarali sehir her zaman bulunmalidir
        if ((mask & 1) == 0) continue;

        for (int j = 1; j < n; ++j) {

            //eger j sehri mevcut alt kumede yoksa gec
            if ((mask & (1 << j)) == 0) continue;

            //j sehri ziyaret edilmeden onceki alt kume
            int prevMask = mask ^ (1 << j);

            for (int k = 0; k < n; ++k) {

                //eger k sehri onceki alt kumede bulunuyorsa
                if (prevMask & (1 << k)) {

                    //k sehrine kadar olan maliyet ile
                    //k sehrinden j şehrine gitme maliyetini topla
                    long long cost = dp[prevMask][k] + dist[k][j];

                    //daha dusuk maliyet bulunduysa tabloyu guncelle
                    if (cost < dp[mask][j]) {
                        dp[mask][j] = cost;
                        parent[mask][j] = k;
                    }
                }
            }
        }
    }

    //tum sehirler ziyaret edildikten sonra baslangic sehri olan 0a geri donerek turu tamamla
    int fullMask = N - 1;
    long long minCost = INF;
    int lastCity = -1;

    for (int j = 1; j < n; ++j) {
        long long cost = dp[fullMask][j] + dist[j][0];

        if (cost < minCost) {
            minCost = cost;
            lastCity = j;
        }
    }

    //en iyi rotayi geriye dogru takip ederek olustur
    vector<int> tour;
    int mask = fullMask;
    int cur = lastCity;

    while (cur != -1) {
        tour.push_back(cur);

        int p = parent[mask][cur];

        //mevcut sehri alt kumeden cikar
        mask ^= (1 << cur);

        //onceki sehre gec
        cur = p;
    }

    reverse(tour.begin(), tour.end());
    tour.push_back(0); //baslangic sehrine geri doner

    return {minCost, tour};
}

int main() {
    vector<vector<long long>> dist = {
        { 0,  2,  9, 10},
        { 1,  0,  6,  4},
        {15,  7,  0,  8},
        { 6,  3, 12,  0}
    };

    auto [cost, tour] = heldKarp(dist);

    cout << "Minimum tur maliyeti: " << cost << endl;
    cout << "Tur: ";

    for (int v : tour) {
        cout << v << " ";
    }

    cout << "\n";

    return 0;
}


