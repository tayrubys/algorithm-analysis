#include <iostream>
#include <vector>
using namespace std;

//greedy graph boyama fonksiyonu
void greedyGraphColoring(const vector<vector<int>>& adjMatrix, int V){
    //result[u]:u dugumune atanan rengi tutar
    vector<int> result(V,-1);//henuz boyanmadigi icin -1

    result[0] = 0; //ilk dugumu 0 ilk renge(0)boyar
    //komsularin kullandigi renkleri takip etmek  icin
    vector<bool> available(V,true);//available[i] = true ise renk i kullanilabilir
    //geriye kalan v-1 dugum sirayla boyar
    for(int u = 1; u < V; u++){
        //u dugumunun komsularini tarar
        for(int v = 0; v <V; v++){
            //eger u ile v arasinda kenar varsa ve v daha once boyanmamissa
            if(adjMatrix[u][v] && result[v] != -1){
                available[result[v]] = false; //v dugumunun rengini kullanilamaz yap
            }
        }
        int cr;//u dugumu icin kullanilabilir ilk rengi bul(en kucuk numarali)
        for(cr = 0; cr < V; cr++){
            if(available[cr]){
                break;
            }
        }
        result[u]=cr;//bulunan u rengini u dugumune ata
        //available dizisini sonraki dugum icin sifirla
        for(int v = 0; v < V; v++){
            if(adjMatrix[u][v] && result[v] != -1){
                available[result[v]] = true;
            }
        }   
    }
    //sonucları yazdirma
    cout << "--- GREEDY GRAPH COLORING SONUCLARI ---\n";
    int maxColor = 0;
    for (int u = 0; u < V; u++) {

        cout << "Dugum " << u<< " ---> Renk " << result[u] << "\n";

        if (result[u] > maxColor) {
            maxColor = result[u];
        }
    }

    cout << "Kullanilan Toplam Renk Sayisi: " << (maxColor + 1) << endl;
}
int main(){
    int V = 5;
   //5 dugumlu komsuluk matrisi
    vector<vector<int>> adjMatrix = {
        {0, 1, 1, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 1, 0},
        {0, 1, 1, 0, 1},
        {0, 0, 0, 1, 0}
    };

    greedyGraphColoring(adjMatrix, V);

    return 0;

}