#include <bits/stdc++.h>
using namespace std;

struct Item
{
    float weight;
    int value;  
};
//karar agacindaki bir dugumun bilgilerini tutan yapi
struct Node
{
    //level:karar agacinda hangi seviyede oldugumuzu gosterir profit:bu dugume kadar secilen esyalarin toplam degeri
    // bound:ust sinir weight:bu dugume kadar secilen esyalarin toplam agirligi
    int level, profit, bound;
    float weight;
};


//esyalari deger/agirlik oranina gore buyukten kucuge siralamak icin kullanilan karsilastirma fonksiyonu
bool cmp(Item a, Item b)
{
    double r1 = (double)a.value / a.weight;
    double r2 = (double)b.value / b.weight;
    //deger/agirlik orani daha buyuk olan once gelsin
    return r1 > r2;
}


//u dugumunden devam edildiginde elde edilebilecek max teorik değeri (upper bound) hesaplar
int bound(Node u, int n, int W, Item arr[])
{
    //eger mevcut agirlik kapasiteyi doldurduysa bu daldan devam edilemez
    if (u.weight >= W)
        return 0;

    //baslangicta bound degerini mevcut gercek deger olarak kabul ediyoruz
    int profit_bound = u.profit;
    int j = u.level + 1;//bir sonraki esyadan baslayacagiz
    int totweight = u.weight;//su ana kadarki toplam agirlik

    //cantaya tamamen sigan esyalari sırayla ekle
    while ((j < n) && (totweight + arr[j].weight <= W))
    {
        totweight += arr[j].weight;//esyanin agirligini toplam agirliga ekle
        profit_bound += arr[j].value;//esyanin degerini toplam degere ekle
        j++;//bir sonraki esyaya gec
    }


    //eger hala degerlendirilecek esya kaldıysa ve bu esya cantaya tamamen sigmiyorsa,
    //sadece bir kismini aliyormusuz gibi hesap yapiyoruz
    //bu islem gercek 0/1 cozumunde yapilmaz
    //sadece upper bound hesaplamak icin kullanilir
    if (j < n)
    {
        //kalan kapasite kadar esyanin degerini ekle
        profit_bound += (W - totweight) * arr[j].value /arr[j].weight;
    }

    //hesaplanan ust siniri geri dondur
    return profit_bound;
}


//kapasitesi W olan cantadan elde edilebilecek maksimum gerçek degeri bulur
int knapsack(int W, Item arr[], int n)
{
    //esyalari deger/agirlik oranina gore sirala
    sort(arr, arr + n, cmp);

    queue<Node> Q;//karar agacindaki dugumler tutmak icin kuyruk olustur

    //u:su anda incelenen dugum
    //v:olusturacagimıiz cocuk dugum
    Node u, v;


    //baslangic dugumun olustur
    // level = -1:henuz hicbir esyaya karar verilmedigini gosterir
    //profit = 0:henuz hicbir esyanin degeri alinmadi
    // weight = 0:henuz hicbir esyanin agirligi alinmadi
    u.level = -1;
    u.profit = u.weight = 0;

    Q.push(u);//baslangic dugumunu kuyruga ekle


    //su ana kadar bulunan en iyi gercek cozum
    int maxProfit = 0;


    //kuyruk bos olmadigi surece karar agacini gez
    while (!Q.empty())
    {
        u = Q.front();//kuyrugun basindaki dugumu al
        Q.pop();//aldigimiz dugumu kuyruktan cikar


        //eger baslangic dugumundeysek ilk esyanin seviyesine gec
        if (u.level == -1)
            v.level = 0;

        //eger son seviyeye geldiysek artik yeni cocuk olusturmaya gerek yok
        if (u.level == n - 1)
            continue;

        v.level = u.level + 1;//bir sonraki esyanin seviyesine gec


        // 1. BRANCH: Eşyayı AL
        //mevcut esyanin agirligini toplam agirliga ekle
        v.weight = u.weight + arr[v.level].weight;

        //mevcut esyanin degerini toplam degere ekle
        v.profit = u.profit + arr[v.level].value;

        //eger esyanin toplam agirligi kapasiteyi asmiyorsa ve elde edilen deger mevcut en iyi degerden buyukse maxProfit güncelle
        if (v.weight <= W && v.profit > maxProfit)
            maxProfit = v.profit;

        //bu daldan devam edersek ulasabilecegimiz teorik max degeri hesapla
        v.bound = bound(v, n, W, arr);

        if (v.bound > maxProfit)
            Q.push(v);


        // 2. BRANCH: Esyayi ALMA
        //esyayi almadigimiz icin ağirlik degismez
        v.weight = u.weight;

        //esyayi almadigimiz icin deger de degismez
        v.profit = u.profit;

        //esyayi almama durumundaki dalin teorik max degerini hesapla
        v.bound = bound(v, n, W, arr);

        //bu dalin daha iyi bir cozum bulma ihtimali varsa kuyruga ekle
        if (v.bound > maxProfit)
            Q.push(v);
    }

    return maxProfit;//buldugumuz en iyi gercek cozumu dondur
}

int main()
{
    int W = 10;//cantanin max kapasitesi
    //esyalar:{agirlik,deger}
    Item arr[] = {
        {2, 40},
        {3.14, 50},
        {1.98, 100},
        {5, 95},
        {3, 30}
    };
    int n = sizeof(arr) / sizeof(arr[0]);//toplam esya sayisini hesapla
    cout << "Maximum possible profit = "<< knapsack(W, arr, n);
    return 0;
}