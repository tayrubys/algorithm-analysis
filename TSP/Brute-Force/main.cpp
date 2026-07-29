#include <iostream>
#include <vector>
#include <algorithm> 
#include <climits>

using namespace std;

int bruteForce(const vector<vector<int>>& graph, int startNode){
    int v = graph.size(); //toplam şehir sayısı
    vector<int> nodes;

    //başlangıç düğümü dışındaki diğer tüm düğümleri listeye ekleme
    for(int i=0;i<v;i++){
        if(i!=startNode){
            nodes.push_back(i);
        }
    }
    
    int minPath = INT_MAX; //min maliyeti en yüksek değer veriliyor

    do{
        int currentPathWeight = 0;
        int currentNode = startNode;
        //oluşan permütasyonun maliyetini hesapla
        for(int i=0;i<nodes.size();i++){
            currentPathWeight +=graph[currentNode][nodes[i]];
            currentNode = nodes[i];
        }
        currentPathWeight += graph[currentNode][startNode];//baslangıç noktasına geri donme maliyetını ekle
        //mevcut maliyet min maliyetten daha kucukse min maliyeti guncelle 
        if(currentPathWeight<minPath){
            minPath=currentPathWeight;
        }
    }while(next_permutation(nodes.begin(), nodes.end()));
    return minPath;
}

int main(){
    vector<vector<int>> graph = {{0, 10, 15, 20}, 
                                {10, 0, 35, 25}, 
                                {15, 35, 0, 30}, 
                                {20, 25, 30, 0}};
    
    int startNode = 0;
    int minCost = bruteForce(graph, startNode);
    cout << "TSP Brute Force en kisa rota maliyeti:"<< minCost << endl;
    return 0;
}