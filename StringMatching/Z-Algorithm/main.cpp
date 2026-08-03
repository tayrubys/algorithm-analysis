#include <iostream>
#include <vector>
using namespace std;

// z dizisini hesaplayan fonksiyon
vector<int> zFunction(string &s){
    int n = s.length();
    vector<int> z(n);

    int l = 0, r = 0;// z dizisinin sol ve sag sinirlari
    for (int i = 1; i < n; i++)
    {
        //eger i mevcut z-box içindeyse
        if (i <= r)
        {
            int k = i - l;//i nin kutu icindeki indeksi

            //daha once hesaplanan degeri tekrar kullan
            z[i] = min(r - i + 1, z[k]);//kutunun disina tasmamak icin min kullanilir
        }

        //gerekirse eslesmeyi z-box disina dogru genislet
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
        {
            z[i]++;
        }

        //eger daha buyukk bir z-box olustuysa l ve r yi güncelle
        if (i + z[i] - 1 > r)
        {
            l = i;
            r = i + z[i] - 1;
        }
    }

    return z;
}
//patternin text icindeki tum eslesmelerini bulan fonksiyon
vector<int> search(string &text, string &pattern)
{
    //birlesitrme
    string s = pattern + '$' + text;

    //olusturulan string icin z dizisini hesapla
    vector<int> z = zFunction(s);

    vector<int> pos;

    int m = pattern.size();

    //z dizisini kontrol et
    for (int i = m + 1; i < z.size(); i++)
    {
        //eger z degeri pattern uzunluğuna esitse eslesme vardır
        if (z[i] == m)
        {
            //text icerisindeki gercek baslangıc indeksini kaydet
            pos.push_back(i - m - 1);
        }
    }

    return pos;
}
int main() {
    string text = "aabxaabxaa";
    string pattern = "aab";

    vector<int> matches = search(text, pattern);

    for (int pos : matches)
        cout << pos << " ";

    return 0;
}