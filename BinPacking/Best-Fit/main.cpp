#include <iostream>
#include <vector>
using namespace std;

//best fit ile gerekli kutu sayisini hesaplar
int bestFit(const int weights[], int n, int capacity){
    if(capacity<=0 || n<=0){
        cout << "kutu kapasitesi ve nesne sayisi pozitif olmalidir.\n";
        return -1;
    }

    int binCount = 0;//kullanilan kutu sayisi

    vector<int> binRemaining(n); //kutularda kalan kapasiteyi tutar
    //nesneleri verilen sirayla yerleştirir
    for(int i=0;i<n;i++){
        if(weights[i]<=0){
            cout <<"nesne agirligi pozitif olmalidir.\n";
            return -1;
        }
        //nesnenin kutu kapasitesinden buyukse kutuya sigmaz
        if(weights[i]>capacity){
            cout <<weights[i] << "agirligindaki nesne," << capacity << "kapasiteli kutuya sigmaz.\n";
            return -1;
        }

        //nesnenin yerlesecegi en uygun kutunun indexsini tutar
        int bestBinIndex = -1;//-1 olmama sebebi henuz uygun kutu bulunamadigini gosterir

        //yerlestirme sonrasinda kutuda kalacak en az boslugu tutar
        int minRemaining = capacity + 1;//baslangicta kapasiteden buyuk bir deger verilir
        
        //daha once acilmis kutulari kontrol eder
        for(int j = 0; j < binCount; j++){
            //nesne mevcut kutuya sigiyorsa
            if(binRemaining[j]>=weights[i]){

                int remainingAfterPlacement = binRemaining[j] - weights[i];//nesneyi koyduktan sonra kalan bos alan
                //onceki kutulardan daha az bosluk birakıyorsa bu kutuyu en uygun kutu olarak kaydeder
                if (remainingAfterPlacement < minRemaining) {
                    minRemaining = remainingAfterPlacement;
                    bestBinIndex = j;
                }
            }
        }
        //nesnenin sigacagi kutu bulunmadiysa yenisi acilir
        if (bestBinIndex == -1) {
            binRemaining[binCount] = capacity - weights[i];//yeni kutunun kalan kapasitesi hesaplar
            binCount++;
        }
        else{
            binRemaining[bestBinIndex] -= weights[i];
        }
    }
    return binCount;
}

int main(){
    int weights[] = {2, 5, 4, 7, 1, 3, 8};
    int capacity = 10;//her kutunun max kapasitesi
    int n = sizeof(weights) / sizeof(weights[0]);//dizideki toplam nesne sayisi
    int result = bestFit(weights, n, capacity);
    if (result != -1) {
        cout << "Best Fit icin gerekli kutu sayisi: "<< result << '\n';
    }
    return 0;
}