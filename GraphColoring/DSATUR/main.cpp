#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;

//doygunluk derecesini hesaplar
int getSaturationDegree(int u, const vector<vector<int>> &adjMatrix, const vector<int> &result, int V){
    unordered_set<int> uniqueColors;
    
    for(int v = 0; v < v; v++){
        //eger u ile v komsuysa ve v dugumu onceden boyanmissa
        if(adjMatrix[u][v] == 1 && result[v] != -1){
            uniqueColors.insert(result[v]);//benzersiz renk kumesine ekle
        }
    }
    return uniqueColors.size();//farkli renk sayisi
}
//bir dugumun derecesini hesaplar(esitlik bozucu icin(tie-breaker))
int getNodeDegree(int u, const vector<vector<int>> &adjMatrix, int V) {
    int degree = 0;
    for (int v = 0; v < V; v++) {
        if (adjMatrix[u][v] == 1) {
            degree++;
        }
    }
    return degree;
}
//boyanmamis dugumler arasinda boyanacak siradaki en uygun dugumu secere
int getHighestSaturationNode(const vector<vector<int>> &adjMatrix, const vector<int> &result, int V){
    int maxSatDegree = -1;
    int maxNodeDegree = -1;
    int selectedNode = -1;
    for(int u = 0; u <V; u++){
        //henuz boyanmamis dugumlere bak
        if (result[u] == -1){
            int satDegree = getSaturationDegree(u, adjMatrix, result, V);
            int nodeDegree = getNodeDegree(u, adjMatrix, V);
            //kural1:en yuksek doygunluk derecesine sahip dugumu sec
            if (satDegree > maxSatDegree){
                maxSatDegree = satDegree;
                maxNodeDegree = nodeDegree;
                selectedNode = u;
            } 
           //kural2:esitlik durumu varsa dugum derecesi yuksek olan dugumu sec
           else if(satDegree == maxSatDegree){
                if (nodeDegree > maxNodeDegree) {
                    maxNodeDegree = nodeDegree;
                    selectedNode = u;
                }
           }
        }
    }
    return selectedNode;
}
//dsatur uygulamasi
void dsaturGraphColoring(const vector<vector<int>> &adjMatrix, int V){
    vector<int> result(V, -1);//result[u]:u düğümüne atanan rengi tutar ve henuz boyanmadigi icin -1
    //birinci adim en yuksek dereceye sahip ilk dugumu bul ve boya(ilk renkle)
    int maxDeg = -1;
    int firstNode = 0;
    for (int u = 0; u < V; u++) {
        int deg = getNodeDegree(u, adjMatrix, V);
        if (deg > maxDeg) {
            maxDeg = deg;
            firstNode = u;
        }
    }
    result[firstNode] = 0;//ilk dugum boyandi

    vector<bool> available(V, true);//komsularin renklerini takip etmek icin(gecici dizi)
    //ikinci adim:kalan V-1 adet dugumu dsatur kuralina gore sirayla boya
    for (int count = 1; count < V; count++){
        int u = getHighestSaturationNode(adjMatrix, result, V);//doygunluk derecesi en yuksek olan boyanmamis dugumu sec
        //u dugumunun boyali komsularinin renklerini yasakla
        for (int v = 0; v < V; v++) {
            if (adjMatrix[u][v] == 1 && result[v] != -1) {
                available[result[v]] = false;
            }
        }
        //kullanilabilir en kucuk numarali rengi bul(greedy)
        int cr;
        for (cr = 0; cr < V; cr++) {
            if (available[cr] == true) {
                break;
            }
        }
        result[u]=cr;//rengi u dugumune atar

        //bir sonraki dugum icin available dizisini tekrar true yap(sifirla)
        for (int v = 0; v < V; v++) {
            if (adjMatrix[u][v] == 1 && result[v] != -1) {
                available[result[v]] = true;
            }
        }

        //yazdirma
        cout << "--- DSATUR GRAPH COLORING SONUCLARI ---\n";
        int maxColor = 0;
        for (int u = 0; u < V; u++) {
            cout << "Dugum " << u << " ---> Renk " << result[u] << "\n";
            if (result[u] > maxColor) {
                maxColor = result[u];
            }
        }
        cout << "Kullanilan Toplam Renk Sayisi: " << (maxColor + 1) << endl;
    }

}
int main() {
    int V = 5;
    vector<vector<int>> adjMatrix = {
        {0, 1, 1, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 1, 0},
        {0, 1, 1, 0, 1},
        {0, 0, 0, 1, 0}
    };
    dsaturGraphColoring(adjMatrix, V);
    return 0;
}