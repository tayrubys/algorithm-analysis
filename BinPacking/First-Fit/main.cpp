#include <bits/stdc++.h>
using namespace std;

//gerekli kutu sayısını bulur
int firstFit(const int weight[], int n, int capacity)
{
    //kutu kapasitesi ve nesne sayisi kontrol edilir
    if (capacity <= 0 || n <= 0) {
        cout << "kutu kapasitesi ve nesne sayisi pozitif olmalidir.\n";
        return -1;
    }

    int binCount = 0; //kullanilan kutu sayisi

    //worst case: her nesne icin ayr bir kutu gerekebilir o yuzden n
    vector<int> binRemaining(n); //her kutuda kalan kapasitesini tutar 

    //nesneleri verilen sirayla yerleştirir
    for (int i = 0; i < n; i++) {

        //nesne agirliginin gecerli olup olmadigini kontrol eder
        if (weight[i] <= 0) {
            cout << "nesne agirligi pozitif olmalidir"<< "gecersiz agirlik: " << weight[i] << '\n';
            return -1;
        }

        //nesne kutu kapasitesinden büyükse hicbir kutuya sigmaz
        if (weight[i] > capacity) {
            cout << "Hata: " << weight[i]<< " agirligindaki nesne, "<< capacity<< " kapasiteli kutuya sigmaz.\n";
            return -1;
        }

        int j;//nesnenin sigabilecegi ilk kutuyu arar
        for (j = 0; j < binCount; j++) {
            //kutunun kalan kapasitesi nesne icin yeterliyse nesne kutuya yerlestir ve kapasiteyi azalt
            if (binRemaining[j] >= weight[i]) {
                binRemaining[j] -= weight[i];
                break; //ff ilk uygun kutuyu kullandigi icin aramayi bitir
            }
        }

        //nesne mevcut kutuların hicbirine sıgmadıysa
        if (j == binCount) {
            binRemaining[binCount] = capacity - weight[i]; //yeni kutu acilir ve kalan kapasiteyi hesapla
            binCount++;
        }
    }
    return binCount; //kullanilan kutu sayisini dondurur
}

int main()
{
    int weight[] = {2, 5, 4, 7, 1, 3, 8}; //kutuya yerlesicek nesnelerin agirligi
    int capacity = 10; //her kutunun max kapasitesi
    int n = sizeof(weight) / sizeof(weight[0]); //dizideki nesne sayisi

    int result = firstFit(weight, n, capacity);

    if (result != -1) {
        cout << "First Fit icin gerekli kutu sayisi: "<< result << '\n';
    }
    return 0;
}