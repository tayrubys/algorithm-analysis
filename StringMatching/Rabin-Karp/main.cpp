#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<int> search(const string &pat, const string &txt){
    int d = 256;//ascii
    int q = 101; //hash hesaplamasinda kullanilan asal sayi(tasmayi onlemek icin)

    int m = pat.length(); //pattern uzunlugu
    int n = txt.length(); //text uzunlugu

    int p = 0; //patternin hash degeri
    int t = 0; //textin hash degeri
    int h = 1; //en soldaki karakterin katsayisi

    vector<int> result; //bulanan indekserlerin tutulacagi vector

    //h degerini hesapla
    for(int i = 0; i < m - 1; i++){
        h = (h * d) % q;
    }

    //hash degerlerini hesapla
    for(int i = 0; i < m; i++){
        p = (d * p + pat[i]) % q;
        t = (d * t + txt[i]) % q;
    }

    //pattern i text uzerinde birer karakter kaydirma
    for (int i = 0; i <= n - m; i++)
    {
        //hash degerleri ayniysa karakterleri tek tek kontrol et
        if (p == t)
        {
            bool match = true;

            for (int j = 0; j < m; j++)
            {
                if (txt[i + j] != pat[j])
                {
                    match = false;
                    break;
                }
            }

            //tam eslesme varsa baslangic indeksini kaydet
            if (match)
                result.push_back(i);
        }

        //bir sonraki pencerenin hash degerini hesapla
        if (i < n - m)
        {
            t = (d * (t - txt[i] * h) + txt[i + m]) % q;

            //hash negatif çıkarsa pozitife çevir
            if (t < 0)
                t += q;
        }
    }
     return result;
}
int main()
{
    string txt = "kocaeli universitesi bilisim sistemleri muhendisligi bolumu";
    string pat = "b";

    vector<int> res = search(pat, txt);

    //sonuclari yazdir
    if (res.empty())
    {
        cout << "pattern bulunamadi" << endl;
    }
    else
    {
        cout << "pattern in bulundugu indeksler: ";
        for (auto it : res)
        {
            cout << it << " ";
        }
        cout << endl;
    }

    return 0;
}