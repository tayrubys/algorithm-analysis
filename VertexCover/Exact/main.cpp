#include <iostream>
#include <vector>
using namespace std;

//grafin kenarlarini tutar
vector<pair<int, int>> edges;

//en iyi cozumu tutacak degiskenler
vector<bool> bestCover;
int bestCount;

//secilen vertexlerin gecerli bir vertex cover olup olmadigini kontrol eder
bool isVertexCover(vector<bool>& selected, int V)
{
    for (auto edge : edges)
    {
        int u = edge.first;
        int v = edge.second;

        //kenarin iki ucu da secilmemisse
        //bu kenar kapatilmamistir
        if (!selected[u] && !selected[v])
            return false;
    }

    return true;
}

//exact backtracking
void backtrack(int index,int V, vector<bool>& selected, int selectedCount){
    //butun vertexleri degerlendirdik
    if (index == V)
    {
        //gecerli bir vertex cover mi
        if (isVertexCover(selected, V))
        {
            //daha kucuk bir cozum bulduysak guncelle
            if (selectedCount < bestCount)
            {
                bestCount = selectedCount;
                bestCover = selected;
            }
        }

        return;
    }

    //birinci durum: vertex i sec
    selected[index] = true;

    if (selectedCount + 1 < bestCount)
    {
        backtrack(index + 1, V, selected,selectedCount + 1);
    }

    //ikinci durum:vertex i secme
    selected[index] = false;

    if (selectedCount < bestCount)
    {
        backtrack(index + 1,V,selected,selectedCount);
    }
}

int main()
{
    int V = 5;

    edges.push_back({0, 1});
    edges.push_back({0, 2});
    edges.push_back({1, 3});
    edges.push_back({2, 3});
    edges.push_back({3, 4});

    vector<bool> selected(V, false);

    bestCount = V + 1;
    bestCover.resize(V, false);

    backtrack(0, V, selected, 0);

    //sonucu yazdir
    cout << "Minimum Vertex Cover: ";

    for (int i = 0; i < V; i++)
    {
        if (bestCover[i])
            cout << i << " ";
    }
    cout << endl;
    cout << "Vertex Cover boyutu: "<< bestCount << endl;
    return 0;
}