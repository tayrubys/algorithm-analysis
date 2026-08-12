#include<iostream>
#include <list>
using namespace std;

//bu sinif komsuluk listesi kullanarak yonsuz bir graf temsil eder
class Graph
{
    int V;//vertex sayisi
    list<int> *adj;//komsuluk listelerini tutan dizi icin pointer

public:
    Graph(int V);//constructor
    void addEdge(int v, int w);//grafa kenar ekleyen fonksiyon
    void printVertexCover();//vertex coverı yazdiran fonksiyon
};

//graph sinifinin constructorı
Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}

//grafa bir kenar ekleyen fonksiyon
void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);//w dugumunu v nin komsuluk listesine ekle
    adj[w].push_back(v);//graf yonsuz oldugu icin v yi de w nin listesine ekle
}

//vertex cover ı yazdiran fonksiyon
void Graph::printVertexCover()
{
    //baslangicta butun koseleri ziyaret edilmemis olarak isaretle
    bool visited[V];

    for (int i=0; i<V; i++)
        visited[i] = false;

    list<int>::iterator i;

    //butun kenarlari tek tek incele
    for (int u=0; u<V; u++)
    {
        //eger u daha once secilmemisse
        if (visited[u] == false)
        {
            //u nun butun komsularin dolas, henuz secilmemis ilk komsuyu bul
            //boylece kalan kenarlardan bir (u, v) kenari secmis oluruz
            for (i = adj[u].begin(); i != adj[u].end(); ++i)
            {
                int v = *i;

                //eger v de daha once secilmemisse
                if (visited[v] == false)
                {
                    //u ve v koselerini vertex covera dahil et
                    //bu iki koseyi secilmis olarak isaretle(boylece bu koselere bagli diger kenarlar goz ardi edilir)
                    visited[v] = true;
                    visited[u]  = true;

                    //ilk uygun kenarı buldugumuz icin donguden cik
                    break;
                }
            }
        }
    }

    //vertex cover kumesindeki koseleri yazdir
    for (int i=0; i<V; i++)
        if (visited[i])
            cout << i << " ";
}

int main()
{
    Graph g(7);//7 koseli graf olustur
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 6);

    g.printVertexCover();

    return 0;
}