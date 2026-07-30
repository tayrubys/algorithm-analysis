#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
//inf kullanılmayan yol(matriste kullanilacak bunun sebebi 0 dan 0 a tekrar gitmek yasak oldugu icin)
const int INF = INT_MAX;

struct Node{
    vector<vector<int>> matrix; //indirgenmis maliyet matrisi
    int cost; //dugumun alt sınır maliyeti
    int currentNode; //su anda bulunulan sehir
    int level; //kac tane gecis yapıldıgı
    vector<int> path; //su ana kadar olusan rota
    Node(int cityCount = 0) {
        matrix.assign(cityCount, vector<int>(cityCount, INF));
        cost = 0;
        currentNode = 0;
        level = 0;
    }
};
//matirsteki satır ve sutunlari indirger ve bulunan tplam indirgeme miktari alt sınıra eklenir
int reduceMatrix(vector<vector<int>>& matrix) {
    int cityCount = matrix.size();
    int reductionCost = 0;

    //her satırdaki en kucuk degeri bulup satırdan cikarma
    for (int i = 0; i < cityCount; i++) {
        int rowMin = INF;

        for (int j = 0; j < cityCount; j++) {
            rowMin = min(rowMin, matrix[i][j]);
        }

        //satir tamamen inf(0) degilse ve en kucuk deger sifirdan buyukse indirgeme yap
        if (rowMin != INF && rowMin > 0) {
            reductionCost += rowMin;

            for (int j = 0; j < cityCount; j++) {
                if (matrix[i][j] != INF) {
                    matrix[i][j] -= rowMin;
                }
            }
        }
    }

    //her sutundaki en kucuk degeri bulup sutundan çıkarma
    for (int j = 0; j < cityCount; j++) {
        int columnMin = INF;

        for (int i = 0; i < cityCount; i++) {
            columnMin = min(columnMin, matrix[i][j]);
        }

        //sutun tamamen inf degilse ve en kucuk deger sifirdan buyukse indirgeme yap
        if (columnMin != INF && columnMin > 0) {
            reductionCost += columnMin;

            for (int i = 0; i < cityCount; i++) {
                if (matrix[i][j] != INF) {
                    matrix[i][j] -= columnMin;
                }
            }
        }
    }

    return reductionCost;
}
//mevcut dugumden yeni bir sehre gidildiğinde çocuk dugum olusturur
Node createChild(const Node& parent, int from, int to) {
    int cityCount = parent.matrix.size();
    Node child(cityCount);

    child.matrix = parent.matrix; //parent dugumden matrisi kopyalar

    //from sehrinden tekrar cikis yapilmasini engeller
    for (int j = 0; j < cityCount; j++) {
        child.matrix[from][j] = INF;
    }

    //to sehrine tekrar girilmesini engeller
    for (int i = 0; i < cityCount; i++) {
        child.matrix[i][to] = INF;
    }

    int edgeCost = parent.matrix[from][to]; //secilen kenarin indirgenmis matristeki maliyeti

    //yeni düğümün maliyetine parent dugumun maliyetini ve secilen kenarı ekler
    child.cost = parent.cost + edgeCost;

    child.cost += reduceMatrix(child.matrix); //yeni matrisi tekrar indirgeme

    //yeni dugum bilgilerini günceller
    child.currentNode = to;
    child.level = parent.level + 1;

    //ana rotayi kopyalayip yeni sehri sona ekle
    child.path = parent.path;
    child.path.push_back(to);

    return child;
}

//priority_queue normalde en buyuk degeri başa alır. bu karşılaştırma sayesinde maliyeti en kucuk dugum once cikar
struct CompareNode {
    bool operator()(const Node& a, const Node& b) const {
        return a.cost > b.cost;
    }
};

//branch and bound couzmu
pair<int, vector<int>> branchAndBound(
    const vector<vector<int>>& graph,
    int startNode
) {
    int cityCount = graph.size();

    //root dugum oluşturma
    Node root(cityCount);
    root.matrix = graph;
    root.currentNode = startNode;
    root.level = 0;
    root.path.push_back(startNode);

    //root dugumun ilk alt sinir maliyetini hesaplama
    root.cost = reduceMatrix(root.matrix);

    //alt siniri en kucuk dugumu once incelemek için oncelik kuyrugu
    priority_queue<Node, vector<Node>, CompareNode> pq;
    pq.push(root);

    int bestCost = INF;   //simdiye kadar bulunan en iyi tam rota maliyeti
    vector<int> bestPath; //simdiye kadar bulunan en iyi rota

    //incelenecek dugum kaldığı sürece devam et
    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        //bu dal mevcut en iyi sonuctan daha kötüyse buda
        if (current.cost >= bestCost) {
            continue;
        }

        //baslangic sehri disindaki tüm sehirler ziyaret edildiyse
        if (current.level == cityCount - 1) {
            //son sehirden baslangic sehrine dönüş maliyeti
            int returnCost =
                current.matrix[current.currentNode][startNode];

            //donus yolu yoksa bu rotayı kullanma
            if (returnCost == INF) {
                continue;
            }

            int totalCost = current.cost + returnCost;

            //daha iyi bir tam rota bulunduysa güncelle
            if (totalCost < bestCost) {
                bestCost = totalCost;
                bestPath = current.path;
                bestPath.push_back(startNode);
            }

            continue;
        }

        //mevcut sehirden gidilebilecek tüm ziyaret edilmemis sehirlere dallanir
        for (int nextNode = 0; nextNode < cityCount; nextNode++) {

            //bu yol kullanilamiyorsa gec
            if (current.matrix[current.currentNode][nextNode] == INF) {
                continue;
            }

            bool visited = false;

            //sehir daha once rotaya eklenmis mi kontrol eder
            for (int city : current.path) {
                if (city == nextNode) {
                    visited = true;
                    break;
                }
            }

            //daha once ziyaret edildiyse tekrar gitme
            if (visited) {
                continue;
            }

            //yeni shre gidilen child dugumu oluşturur
            Node child = createChild(
                current,
                current.currentNode,
                nextNode
            );

            if (child.cost < bestCost) {
                pq.push(child);
            }
        }
    }

    return {bestCost, bestPath};
}

int main() {
    //inf degerleri bir sehrin kendisine gitmesini engeller
    vector<vector<int>> graph = {
        {INF, 10, 15, 20},
        {10, INF, 35, 25},
        {15, 35, INF, 30},
        {20, 25, 30, INF}
    };

    int startNode = 0;

    pair<int, vector<int>> result =
        branchAndBound(graph, startNode);

    cout << "TSP Branch and Bound en kisa rota maliyeti: "
         << result.first << endl;

    cout << "En kisa rota: ";

    for (int city : result.second) {
        cout << city << " ";
    }

    cout << endl;

    return 0;
}