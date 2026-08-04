#include <iostream>
#include <string>
#include <vector>
using namespace std;

void constructLps(string &pat, vector<int> &lps) {
    int len = 0;//onceki suffix ve prefix uzunlugu
    lps[0] = 0; // lps[0] her zaman 0

    int i=1;
    while(i < pat.length()) {
        //karakterler eslesirse ortak prefix-suffix uzunlugunu artir ve lps dizisine yaz
        if(pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } 
        //uyumsuzluk varsa, lps dizisini kullanarak bir onceki lps degerine geri don
        else {
            if(len != 0) {
                //gereksiz tekrarları onlemek icin len degerini bir onceki lps degerine guncelle
                len = lps[len-1];
            } else {
                //eslesen bir prefix yoksa lps[i]yi 0 yap
                lps[i] = 0;
                i++;
            }
        }
    }
}

vector<int> search(string &pat, string &txt) {
    int n = txt.length();
    int m = pat.length();
    //pattern icin bos lps dizisi olusturur
    vector<int> lps(m);
    vector<int> res;

    constructLps(pat, lps);

    int i = 0; // txt icin index
    int j = 0; // pat icin index
    while(i < n) {
        if(txt[i]==pat[j]){
            i++;
            j++;
            //tum pattern eslestiyse baslangic indeksini kaydet
            if(j == m){
                res.push_back(i-j);
                j = lps[j-1];//gereksiz karsilastirmaları onlemek icin j'yi uygun lps degeriyle guncelle
            }
        }
        //uyumsuzluk varsa
        else{
           //gereksiz karsilastirmalari onlemek için onceki indeksin lps degerini kullan
              if(j != 0){
                 j = lps[j-1];
                } else {
                 i++;
                }
        }
    }
    return res;
}

int main() {
    string txt = "aabaacaadaabaaba";
    string pat = "aaba";

    vector<int> res = search(pat, txt);
    for (int i = 0; i < res.size(); i++)
        cout << res[i] << " ";

    return 0;
}