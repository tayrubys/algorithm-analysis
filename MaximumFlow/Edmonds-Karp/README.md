# Edmonds-Karp Algoritması — Maximum Flow

Bu bölümde bir akış ağında (flow network) kaynak (source) düğümden hedef (sink) düğüme gönderilebilecek maksimum akış miktarını bulan **Edmonds-Karp** yöntemi açıklanmaktadır. Kodda yol arama işlemi kuyruk (queue) kullanan **BFS (Genişlik Öncelikli Arama)** ile yapılır.

## Algoritmanın Geliştirilme Amacı

* **Bu algoritma neden geliştirilmiştir?** Bir ağda, kenarların taşıyabileceği kapasiteler göz önünde bulundurularak kaynaktan hedefe gönderilebilecek en yüksek akış miktarını bulmak için geliştirilmiştir. Ulaştırma, ağ trafiği, eşleştirme problemleri gibi birçok alanda karşılaşılan "kapasiteli ağda maksimum akış" problemini çözer.


* **Hangi eksikliği gidermektedir?** Akışı doğrudan artırmaya çalışan basit yaklaşımlar, bir kez yanlış yönde gönderilen akışı geri alamaz ve bu nedenle optimum sonuca ulaşamayabilir. Ford-Fulkerson metodunun temeli olan **residual graph (kalan kapasite grafiği)** kullanılarak daha önce gönderilen akışın gerekirse geri alınmasına (geri kenar üzerinden) izin verilir; böylece hatalı bir yoldan gönderilen akış sonradan düzeltilebilir. Edmonds-Karp ise bu mantığı BFS ile birleştirerek, DFS'nin yüksek kapasiteli graflarda takılabileceği uzun döngü eksikliğini giderir.


* **Önceki yöntemlere göre avantajı nedir?** Saf Ford-Fulkerson'a (DFS tabanlı) göre en büyük avantajı, çalışma süresinin grafın kenar kapasitelerine değil, doğrudan düğüm ve kenar sayısına bağlı (polinomsal) olmasıdır. Her zaman en kısa (en az kenardan oluşan) artırıcı yolu bulduğu için sonsuz döngü riskini veya devasa kapasitelerde sistemin yavaşlamasını önler.
* **Hangi durumda tercih edilir?**
* Kaynaktan hedefe kesin maksimum akış değerinin bulunması gerektiği durumlarda.


* Ağ trafiği, boru hattı, ulaşım ağı gibi kapasite kısıtlı sistemlerin analizinde.


* Eşleştirme (matching), kesme (min-cut) gibi maksimum akışa indirgenebilen problemlerde.


* Kenar kapasitelerinin çok büyük değerler alabildiği ve performans garantisi (polinomsal zaman) aranan durumlarda.


* **Hangi durumda tercih edilmez?**
* Çok büyük ağlarda performansın kritik olduğu durumlarda; bu gibi durumlarda (Dinic gibi) daha hızlı özel algoritmalar tercih edilebilir.


* Ağın sürekli değiştiği (dinamik) ve her değişiklikte akışın baştan hesaplanmasının maliyetli olduğu durumlarda.





## Algoritmanın Çalışma Mantığı

Algoritma, kaynaktan hedefe **artırıcı yol (augmenting path)** bulunamayana kadar tekrar tekrar çalışır. Her artırıcı yol bulunduğunda, bu yol üzerinden mümkün olan en fazla akış gönderilir. Yol arama işlemi, ağın katmanlarını seviye seviye tarayan **BFS (Genişlik Öncelikli Arama)** ile yapılır.

Her adımda:

1. `visited` dizisi sıfırlanır ve residual graph (`rGraph`) üzerinde BFS ile kaynaktan hedefe ulaşan *en az kenarlı* yol aranır.
2. BFS, ulaştığı düğümleri kuyruğa (`queue`) ekler ve ziyaret edilmemiş komşuları kapasiteleri doğrultusunda seviye seviye tarar.
3. Hedefe ulaşıldığında BFS anında `true` döndürür ve `parent` dizisi, hedeften kaynağa kadar izlenen yolu tutar.


4. Yol bulunamazsa algoritma durur ve o ana kadar biriken toplam akış sonuç olarak döndürülür.


5. Yol bulunduysa, bu yol üzerindeki kenarların kalan kapasitelerinin **minimumu** (`path_flow`) hesaplanır.


6. Yol üzerindeki her kenarın kapasitesi `path_flow` kadar azaltılır (ileri yön), ters yönüne ise `path_flow` kadar kapasite eklenir (geri yön); bu, gerekirse akışın sonradan geri alınabilmesini sağlar.


7. `path_flow`, toplam akışa (`max_flow`) eklenir ve 1. adıma dönülür.



### Örnek

Koddaki `main` fonksiyonunda tanımlı ağ, `0` kaynak ve `5` hedef düğüm olacak şekilde ele alınsın:

```text
0 -> 1 : 8      0 -> 4 : 3
1 -> 2 : 9
2 -> 4 : 7      2 -> 5 : 2
3 -> 5 : 5
4 -> 2 : 7      4 -> 3 : 4

```

BFS her zaman en kısa yolları bulmaya öncelik verir. Sırasıyla şu yollar bulunur ve akışlar gönderilir:

```text
Yol 1: 0 -> 1 -> 2 -> 5                gönderilen akış = 2   (sınırlayan kenar: 2 -> 5)
Yol 2: 0 -> 4 -> 3 -> 5                gönderilen akış = 3   (sınırlayan kenar: 0 -> 4)
Yol 3: 0 -> 1 -> 2 -> 4 -> 3 -> 5      gönderilen akış = 1   (sınırlayan kenar: 4 -> 3)

```

Bu noktadan sonra BFS, kaynaktan hedefe ulaşan boş kapasiteli başka bir yol bulamaz ve algoritma durur.

Toplam maksimum akış: **2 + 3 + 1 = 6**

> Edmonds-Karp, residual graph üzerindeki geri kenarlar sayesinde ilerideki bir adımda daha önce gönderilen akışın bir kısmının dolaylı olarak geri alınabilmesini sağlar. BFS kullanıldığı için daima "en kısa" yol tercih edilir, bu da algoritmanın kesin maksimum akışı kapasitelerden bağımsız bir sürede bulmasını garanti eder.
> 
> 

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon BFS(ResidualGraf, Kaynak, Hedef, Parent):

    Ziyaret = tum dugumler icin False
    Kuyruk = bos kuyruk

    Kuyruk.ekle(Kaynak)
    Ziyaret[Kaynak] = True
    Parent[Kaynak] = -1

    Döngü (Kuyruk bos degilken):
        u = Kuyruk.cikar()

        Döngü (v = 0'dan DugumSayisi - 1'e kadar):
            Eğer Ziyaret[v] == False ve ResidualGraf[u][v] > 0 ise:
                Kuyruk.ekle(v)
                Parent[v] = u
                Ziyaret[v] = True
                
                Eğer v == Hedef ise:
                    Döndür True

    Döndür False


Fonksiyon EdmondsKarp(Graf, Kaynak, Hedef):

    ResidualGraf = Graf'in bir kopyasi
    Parent = DugumSayisi boyutunda dizi
    MaksimumAkis = 0

    Döngü (BFS(ResidualGraf, Kaynak, Hedef, Parent) == True olduğu sürece):

        // Yol uzerindeki en kucuk kalan kapasiteyi bul
        YolAkisi = sonsuz

        Döngü (v = Hedef'ten Kaynak'a Parent uzerinden geri giderek):
            u = Parent[v]
            YolAkisi = min(YolAkisi, ResidualGraf[u][v])

        // Residual grafi guncelle
        Döngü (v = Hedef'ten Kaynak'a Parent uzerinden geri giderek):
            u = Parent[v]
            ResidualGraf[u][v] = ResidualGraf[u][v] - YolAkisi   // ileri yon azalir
            ResidualGraf[v][u] = ResidualGraf[v][u] + YolAkisi   // geri yon artar

        MaksimumAkis = MaksimumAkis + YolAkisi

    Döndür MaksimumAkis

```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`int graph[V][V]`** — Ağın kapasitelerini tutan komşuluk matrisi (adjacency matrix). `graph[u][v]`, `u` düğümünden `v` düğümüne olan kenarın kapasitesidir; kenar yoksa değer `0`'dır.


* **`int rGraph[V][V]`** — Residual graph. Başlangıçta `graph` ile aynıdır, fakat algoritma ilerledikçe ileri yön kapasiteleri azalır, geri yön kapasiteleri (`rGraph[v][u]`) artar. Bu geri kenarlar, gönderilen akışın gerekirse "geri alınmasına" imkân tanır.


* **`queue<int> q`** — BFS'nin genişlik öncelikli gezinme sırasını sağlamak için kullanılan STL kuyruk yapısıdır.
* **`bool visited[V]`** ve **`memset(visited, 0, sizeof(visited))`** — BFS sırasında hangi düğümlerin ziyaret edildiğini tutar. Her yeni artırıcı yol aramasından önce tekrar sıfırlanır.


* **`int parent[V]`** — BFS sırasında her düğüme hangi düğümden ulaşıldığını saklar. Bulunan yolun hedef düğümden kaynak düğüme doğru geriye izlenmesini sağlar.


* **`INT_MAX`** (`<climits>`) — Yol üzerindeki minimum kalan kapasiteyi (`path_flow`) hesaplarken başlangıç değeri olarak kullanılır; böylece ilk karşılaştırmada her zaman daha küçük bir değer bulunması garanti edilir.


* **`min()`** (`<algorithm>`) — Yol üzerindeki kenarlardan en düşük kalan kapasiteyi bulmak için kullanılır; bu değer o yoldan gönderilebilecek maksimum akış miktarıdır.


* **`#define V 6`** — Graf düğüm sayısını sabit olarak tanımlar; komşuluk matrisinin boyutunu belirler.



---

## Kullanılan Kütüphaneler

* `<iostream>`: `cout` kullanılarak sonucun ekrana yazdırılması için kullanılır.


* `<climits>`: `path_flow` değişkeninin başlangıç değeri olan `INT_MAX` sabiti için kullanılır.


* `<cstring>` / `<string.h>`: `visited` dizisini her BFS çağrısından önce sıfırlamak için kullanılan `memset` fonksiyonu için kullanılır.
* `<queue>`: BFS algoritmasında düğümleri sırayla işlemek için kullanılan veri yapısı kütüphanesidir.
* `<algorithm>`: Yol üzerindeki minimum kapasiteyi bulmak için kullanılan `min` fonksiyonu için kullanılır.



---

## Zaman Karmaşıklığı

Edmonds-Karp algoritmasının getirdiği asıl yenilik, algoritma hızının kapasitelere bağlı olmaktan çıkmasıdır. Bulunan artırıcı yol sayısı en fazla `O(V × E)` olabilir.

Matris implementasyonunda koddaki tek bir BFS çağrısı, her düğüm için `V` genişliğinde tarama yapabileceğinden `O(V²)` zaman alır.

Bu nedenle bu spesifik komşuluk matrisi uygulamasının toplam zaman karmaşıklığı:

```text
O(V × E) × O(V²) = O(V³ × E)

```

*(Not: Eğer komşuluk matrisi yerine komşuluk listesi kullanılsaydı, standart karmaşıklık `O(V × E²)` seviyesine düşerdi.)*

### Best Case

En iyi durumda, BFS tarafından doğrudan bulunan tek veya çok az sayıda yol maksimum akışı sağlar. Bu durumda karmaşıklık `O(V²)`'ye yakınsar.

### Average Case

Bulunan yolların sayısına bağlı olarak karmaşıklık `O(V²)` ile `O(V³ × E)` arasında değişir. BFS en kısa yolları seçtiği için pratikte DFS versiyonuna kıyasla ağdaki darboğazları çok daha hızlı çözer.

### Worst Case

Matris temsili sebebiyle her BFS iterasyonunun `O(V²)` sürdüğü ve teorik maksimum sınır olan `O(V × E)` iterasyonun gerektiği senaryoda karmaşıklık `O(V³ × E)` olur.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(V²)**

Algoritmanın en büyük yardımcı bellek kullanımı, residual graph için oluşturulan `V × V` boyutundaki matristen kaynaklanır:

```cpp
int rGraph[V][V];

```

Bu yapı `O(V²)` alan kullanır.

Bunun yanında:

* `visited[V]` → `O(V)`

* `parent[V]` → `O(V)`

* `queue<int> q` → `O(V)`

`graph[V][V]` giriş matrisi olduğu için yardımcı alan karmaşıklığına ayrıca dahil edilmez. Ancak `graph` ve `rGraph` birlikte değerlendirildiğinde toplam bellek kullanımı da `O(V²)`'dir.

Baskın terim `V × V` boyutundaki matrisler olduğu için alan karmaşıklığı:

```text
O(V²)

```

olur.