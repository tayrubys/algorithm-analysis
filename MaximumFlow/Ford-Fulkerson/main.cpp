#include <iostream>
#include <climits>
#include <string.h>
#include <queue>
#include <algorithm>
using namespace std;

#define V 6 //grafin dugum sayisi

//bfs kullanarak kaynak ile hedef arasinda yol arar
bool bfs(int rGraph[V][V], int s, int t, int parent[])
{
    bool visited[V];//ziyaret edilen dugumleri tutar
    memset(visited, 0, sizeof(visited));//baslangicta tum dugumler ziyaret edilmemis kabul edilir
    queue<int> q;//bfs icin

    q.push(s);//kaynak dugum kuyruga eklenir
    visited[s] = true;
    parent[s] = -1;//kaynak dugumden bir onceki dugum yoktur
    //kuyruk bos olmadigi surece bfs devam eder
    while (!q.empty())
    {
        //kuyrugun basindaki dugum alinir
        int u = q.front();
        q.pop();
        //u dugumunden diger tum dugumlere bakilir
        for (int v = 0; v < V; v++)
        {   //v dugumu daha once ziyaret edilmemisse ve kapasitesi varsa ziyaret edilir 
            if (visited[v] == false && rGraph[u][v] > 0)
            {
                q.push(v);
                parent[v] = u;//v dugumune u uzerinden ulasildigi kaydedilir
                visited[v] = true;
            }
        }
    }
    //heef dugume ulasildiysa t , ulasilmadiysa f
    return (visited[t] == true);
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

    //bulunan yolun dugumlerini tutar(bfs tarafindan)
    int parent[V];

    int max_flow = 0;//baslangictaki toplam akis

    //kaynaktan hedefe yol oldugu surece devam et
    while (bfs(rGraph, s, t, parent))
    {
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
    cout << "Max Flow: "<< fordFulkerson(graph, 0, 5)<< endl;
    return 0;
}