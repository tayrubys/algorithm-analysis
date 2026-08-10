#include <iostream>
#include <climits>
#include <cstring>
#include <algorithm>
using namespace std;

#define V 6 //grafin dugum sayisi

//dfs kullanarak kaynak ile hedef arasinda yol arar
bool dfs(int rGraph[V][V], int u, int t, int parent[], bool visited[])
{
    visited[u] = true;//mevcut dugumu ziyaret edildi olarak isaretler

    //hedefe ulastiysa yol bulundu, true döndür
    if (u == t)
        return true;

    //u dugumunden gidilebilecek diger tum dugumlere bak
    for (int v = 0; v < V; v++)
    {
        //v dugumu ziyaret edilmemisse ve aradaki kenarda bos kapasite varsa
        if (!visited[v] && rGraph[u][v] > 0)
        {
            parent[v] = u; //yolu kaydet
            //v dugum uzerinden hedefe giden bir yol var mi diye daha derine in
            if (dfs(rGraph, v, t, parent, visited))
            {
                return true;
            }
        }
    }
    //hicbir yoldan hedefe ulasilamadiysa false dondur
    return false;
}

//ford-fulkerson algoritmasini uygular
int fordFulkerson(int graph[V][V], int s, int t)
{
    int u, v;

    //residual graph olusturulur
    //baslabgicta rGraph orijinal grafin kapsasite degerlerinin aynisidir
    int rGraph[V][V];

    for (u = 0; u < V; u++)
    {
        for (v = 0; v < V; v++)
        {
            rGraph[u][v] = graph[u][v];
        }
    }

    //bulunan yolun dugumlerini tutar (dfs tarafindan doldurulacak)
    int parent[V];

    int max_flow = 0;//baslangictaki toplam akis
    bool visited[V];//ziyaret dizisi

    //kaynaktan hedefe yol oldugu surece devam et
    while (true)
    {
        //dfsher calistiginda ziyaret edilenler listesini sifirlamamiz gerekir
        memset(visited, 0, sizeof(visited));
        
        //eger dfs bir yol bulamazsa islemi bitir
        if (!dfs(rGraph, s, t, parent, visited))
        {
            break;
        }

        //bulunan yol uzerindeki min kapasiteyi bulmak icin buyuk deger atanir
        int path_flow = INT_MAX;

        for (v = t; v != s; v = parent[v])
        {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);//yol uzerinde en kucuk kapasite bulunur
        }

        //residual graph'i güncelle(bulunan yoldaki akis gonderildikten sonra)
        for (v = t; v != s; v = parent[v])
        {
            u = parent[v];

            //ileri yöndeki kapasiteyi azaltilir
            rGraph[u][v] -= path_flow;
            //geri yondeki kapasiteyi artilir(daha sonra gonderilen akisin geri alinmasina izin verilir)
            rGraph[v][u] += path_flow;
        }

        //bulunan akis toplam akisa eklenir
        max_flow += path_flow;
    }

    return max_flow;
}

int main()
{
    int graph[V][V] =
    {
        {0, 8, 0, 0, 3, 0},
        {0, 0, 9, 0, 0, 0},
        {0, 0, 0, 0, 7, 2},
        {0, 0, 0, 0, 0, 5},
        {0, 0, 7, 4, 0, 0},
        {0, 0, 0, 0, 0, 0}
    };
    cout << "Max Flow: " << fordFulkerson(graph, 0, 5) << endl;
    return 0;
}