#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

//hedef dugumu, akisi, kapasiteyi ve ters kenarin indeksini tutan yapi
struct Edge {
    int v;    //hedef dugum
    int flow; //kenardan o an gecen akis miktari
    int C;    //kenarin maksimum kapasitesi
    int rev;  //ters kenarin komsuluk listesindeki indeksi (hızlı geri alma islemi icin)
};

class Graph {
    int V; //dugum sayisi
    vector<int> level; //seviye grafi (dugumlerin kaynaga olan uzakligi)
    vector<vector<Edge>> adj; //komsuluk listesi (adjacency list)

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
        level.resize(V);
    }

    //grafa yonlu kenar ekler(ve otomatik olarak ters kenari sifir kapasiteyle ekler)
    void addEdge(int u, int v, int C) {
        //ileri yondeki kenar(baslangicta akis 0, kapasite C)
        Edge a{v, 0, C, (int)adj[v].size()};
        
        //geri alma(residual) islemleri icin sanal ters kenar(baslangicta akis 0, kapasite 0)
        Edge b{u, 0, 0, (int)adj[u].size()};

        adj[u].push_back(a);
        adj[v].push_back(b); //ters kenari hedef dugumun listesine ekle
    }

    bool BFS(int s, int t);
    int sendFlow(int u, int flow, int t, vector<int>& start);
    int DinicMaxflow(int s, int t);
};

//kaynaktan(S) hedefe(T) yol olup olmadigini bulur ve seviye grafini olusturur
bool Graph::BFS(int s, int t) {
    fill(level.begin(), level.end(), -1);//tum seviyeleri -1(ziyaret edilmedi)olarak isaretler
    level[s] = 0;//kaynagin seviyesi 0
    queue<int> q;
    q.push(s);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto& e : adj[u]) {
            //eger komsu dugume daha once gidilmediyse ve kenarda bos kapasite varsa
            if (level[e.v] < 0 && e.flow < e.C) {
                level[e.v] = level[u] + 1;//komsunun seviyesini bir artırır
                q.push(e.v);
            }
        }
    }
    //hedefe ulasilabildiyse t, ulasilamadiysa f doner
    return level[t] >= 0;
}

//dfs kullanarak seviye grafi uzerinden bloklama akisi(blocking flow) gonderir
int Graph::sendFlow(int u, int flow, int t, vector<int>& start) {
    if (u == t)//hedefe ulastiysak buldugumuz darbogaz akisini dondurur
        return flow;

    //start dizisi, "cikmaz sokak" olan yollari tekrar tekrar denememek icin kullanilir
    for (; start[u] < adj[u].size(); start[u]++) {
        Edge& e = adj[u][start[u]];

        //sadece bir ust seviyedeki dugume(ileriye)git ve bos kapasite varsa gecer
        if (level[e.v] == level[u] + 1 && e.flow < e.C) {
            //darbogazi bul(mevcut akis ile kenarin kalan kapasitesini karsilastirir)
            int curr_flow = min(flow, e.C - e.flow);
            
            //daha derine in(dfs)
            int temp_flow = sendFlow(e.v, curr_flow, t, start);

            //eger hedefe akis ulastiysa kapasiteleri gunceller
            if (temp_flow > 0) {
                e.flow += temp_flow;//ileri kenara akis ekler
                adj[e.v][e.rev].flow -= temp_flow;//ters kenardan akis cikarir(geri alma hakki)
                return temp_flow;
            }
        }
    }
    return 0;//bu yoldan hedefe ulasilamadi
}

//maksimum akisi dondurur
int Graph::DinicMaxflow(int s, int t) {
    if (s == t) return -1;

    int total = 0;//toplam akis

    //bfs ile hedefe ulasilabildigi(yeni seviye grafi olustugu)surece devam eder
    while (BFS(s, t) == true) {
        
        //dfs te hangi kenarda kaldigimizi tutan dizi
        vector<int> start(V, 0);
        //dfs ile bu seviye grafindaki tum olasi akislari bulur(bloklama akisi)
        //dfs hedefe ulasamayana(0 donene) kadar calisir
        while (int flow = sendFlow(s, INT_MAX, t, start)) {
            total += flow;
        }
    }
    return total;
}

int main() {
    Graph g(6);
    g.addEdge(0, 1, 16);
    g.addEdge(0, 2, 13);
    g.addEdge(1, 2, 10);
    g.addEdge(1, 3, 12);
    g.addEdge(2, 1, 4);
    g.addEdge(2, 4, 14);
    g.addEdge(3, 2, 9);
    g.addEdge(3, 5, 20);
    g.addEdge(4, 3, 7);
    g.addEdge(4, 5, 4);
    cout << "Maximum Flow: " << g.DinicMaxflow(0, 5) << endl;    
    return 0;
}