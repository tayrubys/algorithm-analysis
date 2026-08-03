#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> search(const string &pat, const string &txt){
    
    int m = pat.length(); //pattern uzunlugu
    int n = txt.length(); //text uzunlugu

    vector<int> result; //sonuclarin tutulacagi vector(bulunan indeksler burda tutulacak)
    //pattern kaydırma
    for(int i=0; i<=n-m; i++){
        
        int j;
        for(j=0; j<m; j++){
            //pattern ve textteki karakterler eşleşmezse donguden cik
            if(txt[j+i] != pat[j]){ 
                break; 
            }
        }
        //pattern eslesirse, bulunan indexi result vectorune ekle
        if(j==m){
            result.push_back(i);
        }
    }
    return result; 
}
int main()
{
    string txt = "aabaacaadaabaaba";
    string pat = "aaba";

    vector<int> res = search(pat, txt);

    for (auto it : res)
    {
        cout << it << " ";
    }

    return 0;
}