#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int nearestNeighbor(const vector<vector<int>>& graph, int startNode){
    int v = graph.size();
    vector<bool> visited(v,false); // ziyaret durumunu tutar

    int currentPathWeight = 0; //toplam maliyeti tutar
    int currentNode = startNode; 

    visited[currentNode] = true;//başlangıç node ı ziyaret edildi

    //baslangic node ı haric, geri kalan nodelara bakmak için(-1 olma sebebi baslangıc sehrının ziyaret edilmiş)
    for(int i=0;i< v - 1 ;i++){
        int nextNode =-1; //gidilen node un numarasını tutar ve henuz node secilmedigi icin -1
        int minEdge = INT_MAX;

        //bulundugumuz düğümden diğer ziyaret edilmemis dugumlerin mesafesine bakma
        for(int j=0;j<v;j++){
            if(!visited[j] && graph[currentNode][j] < minEdge){
                minEdge = graph[currentNode][j];
                nextNode = j;
            }
        }
        //en kısa mesafedeki bulunan dugume git
        currentPathWeight +=minEdge;
        currentNode = nextNode;
        visited[currentNode] = true;
    } 
    currentPathWeight += graph[currentNode][startNode];
    return currentPathWeight;
}

int main(){
     vector<vector<int>> graph = {{0, 10, 15, 20}, 
                                {10, 0, 35, 25}, 
                                {15, 35, 0, 30}, 
                                {20, 25, 30, 0}};
    
    int startNode = 0;
    int cost = nearestNeighbor(graph, startNode);
    
    cout << "TSP Nearest Neighbor rota maliyeti: " << cost << endl;

    return 0;
}