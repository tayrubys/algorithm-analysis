# Ford-Fulkerson Algoritması — Maximum Flow

Bu bölümde bir akış ağında (flow network) kaynak (source) düğümden hedef (sink) düğüme gönderilebilecek maksimum akış miktarını bulan **Ford-Fulkerson** yöntemi açıklanmaktadır. Kodda yol arama işlemi BFS ile yapıldığından bu, algoritmanın **Edmonds-Karp** adı verilen özel bir uygulamasıdır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Bir ağda, kenarların taşıyabileceği kapasiteler göz önünde bulundurularak kaynaktan hedefe gönderilebilecek en yüksek akış miktarını bulmak için geliştirilmiştir. Ulaştırma, ağ trafiği, eşleştirme problemleri gibi birçok alanda karşılaşılan "kapasiteli ağda maksimum akış" problemini çözer.
- **Hangi eksikliği gidermektedir?** Akışı doğrudan artırmaya çalışan basit yaklaşımlar, bir kez yanlış yönde gönderilen akışı geri alamaz ve bu nedenle optimum sonuca ulaşamayabilir. Ford-Fulkerson, **residual graph (kalan kapasite grafiği)** kullanarak daha önce gönderilen akışın gerekirse geri alınmasına (geri kenar üzerinden) izin verir; böylece hatalı bir yoldan gönderilen akış sonradan düzeltilebilir.
- **Önceki yöntemlere göre avantajı nedir?** Ford-Fulkerson, uygun koşullar altında maksimum akışı bulur. Bu kodda yol arama işlemi BFS ile yapıldığı için algoritma Edmonds-Karp olarak uygulanmaktadır ve maksimum akışı bulurken polinomik bir çalışma süresi garantisi sağlar. Residual graph fikri sayesinde, akışı geri alma esnekliği kazandırarak açgözlü (greedy) yaklaşımların takıldığı durumlardan kurtulur. BFS ile birleştirildiğinde (Edmonds-Karp), kapasite değerlerinden bağımsız, garanti edilmiş bir çalışma süresi sunar.
- **Hangi durumda tercih edilir?**
  - Kaynaktan hedefe kesin maksimum akış değerinin bulunması gerektiği durumlarda.
  - Ağ trafiği, boru hattı, ulaşım ağı gibi kapasite kısıtlı sistemlerin analizinde.
  - Eşleştirme (matching), kesme (min-cut) gibi maksimum akışa indirgenebilen problemlerde.
- **Hangi durumda tercih edilmez?**
  - Çok büyük ağlarda (çok sayıda düğüm/kenar) performansın kritik olduğu ve yaklaşık bir sonucun yeterli olduğu durumlarda; bu gibi durumlarda daha hızlı özel algoritmalar (Dinic gibi) tercih edilebilir.
  - Ağın sürekli değiştiği (dinamik) ve her değişiklikte akışın baştan hesaplanmasının maliyetli olduğu durumlarda.

## Algoritmanın Çalışma Mantığı

Algoritma, kaynaktan hedefe **artırıcı yol (augmenting path)** bulunamayana kadar tekrar tekrar çalışır. Her artırıcı yol bulunduğunda, bu yol üzerinden mümkün olan en fazla akış gönderilir.

Her adımda:

1. Residual graph (`rGraph`) üzerinde BFS ile kaynaktan hedefe bir yol aranır.
2. Yol bulunamazsa algoritma durur ve o ana kadar biriken toplam akış sonuç olarak döndürülür.
3. Yol bulunduysa, bu yol üzerindeki kenarların kalan kapasitelerinin **minimumu** (`path_flow`) hesaplanır.
4. Yol üzerindeki her kenarın kapasitesi `path_flow` kadar azaltılır (ileri yön).
5. Yol üzerindeki her kenarın ters yönüne `path_flow` kadar kapasite eklenir (geri yön); bu, gerekirse akışın sonradan geri alınabilmesini sağlar.
6. `path_flow`, toplam akışa (`max_flow`) eklenir.
7. Kaynaktan hedefe artık yol kalmayana kadar 1-6 arası adımlar tekrarlanır.
8. Toplam akış (`max_flow`) döndürülür.

### Örnek

Koddaki `main` fonksiyonunda tanımlı ağ, `0` kaynak ve `5` hedef düğüm olacak şekilde ele alınsın:

```text
0 -> 1 : 8      0 -> 4 : 3
1 -> 2 : 9
2 -> 4 : 7      2 -> 5 : 2
3 -> 5 : 5
4 -> 2 : 7      4 -> 3 : 4
```

BFS ile bulunan artırıcı yollar ve bu yollarda gönderilen akışlar sırasıyla:

```text
Yol 1: 0 -> 1 -> 2 -> 5      gönderilen akış = 2
Yol 2: 0 -> 4 -> 3 -> 5      gönderilen akış = 3
Yol 3: 0 -> 1 -> 2 -> 4 -> 3 -> 5   gönderilen akış = 1
```

Bu yoldan sonra kaynaktan hedefe başka artırıcı yol kalmaz ve algoritma durur.

Toplam maksimum akış: **2 + 3 + 1 = 6**

> Ford-Fulkerson, bir yolu bulduktan sonra o yolu "kalıcı" kabul etmez. Residual graph üzerindeki geri kenarlar sayesinde, ilerideki bir adımda daha önce gönderilen akışın bir kısmı dolaylı olarak geri alınabilir. Bu, algoritmanın kesin maksimum akışı bulmasını garanti eder.

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

    Döndür Ziyaret[Hedef]


Fonksiyon FordFulkerson(Graf, Kaynak, Hedef):

    ResidualGraf = Graf'in bir kopyasi
    Parent = DugumSayisi boyutunda dizi
    MaksimumAkis = 0

    Döngü (BFS(ResidualGraf, Kaynak, Hedef, Parent) True dondugu surece):

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

* **`bool visited[V]`** ve **`memset(visited, 0, sizeof(visited))`** — BFS sırasında hangi düğümlerin ziyaret edildiğini tutar. `memset`, diziyi tek seferde ve hızlıca `false` (0) değerleriyle doldurur.

* **`queue<int> q`** — BFS'nin genişlik öncelikli gezinme sırasını sağlamak için kullanılan STL kuyruk yapısıdır (`push`, `pop`, `front`, `empty`).

* **`int parent[V]`** — BFS sırasında her düğüme hangi düğümden ulaşıldığını saklar. Bulunan yolun hedef düğümden kaynak düğüme doğru geriye izlenmesini sağlar.

* **`INT_MAX`** (`<limits.h>`) — Yol üzerindeki minimum kalan kapasiteyi (`path_flow`) hesaplarken başlangıç değeri olarak kullanılır; böylece ilk karşılaştırmada her zaman daha küçük bir değer bulunması garanti edilir.

* **`min()`** (`<algorithm>`) — Yol üzerindeki kenarlardan en düşük kalan kapasiteyi bulmak için kullanılır; bu değer o yoldan gönderilebilecek maksimum akış miktarıdır.

* **`#define V 6`** — Graf düğüm sayısını sabit olarak tanımlar; komşuluk matrisinin boyutunu belirler.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <limits.h>
#include <string.h>
#include <queue>
#include <algorithm>
```

- `<iostream>`: `cout` kullanılarak sonucun ekrana yazdırılması için kullanılır.
- `<limits.h>`: `path_flow` değişkeninin başlangıç değeri olan `INT_MAX` sabiti için kullanılır.
- `<string.h>`: `visited` dizisini sıfırlamak için kullanılan `memset` fonksiyonu için kullanılır.
- `<queue>`: BFS algoritmasında düğümleri sırayla işlemek için kullanılan `queue` veri yapısı için kullanılır.
- `<algorithm>`: Yol üzerindeki minimum kapasiteyi bulmak için kullanılan `min` fonksiyonu için kullanılır.

---

## Zaman Karmaşıklığı

BFS ile artırıcı yol arayan bu uygulama **Edmonds-Karp** algoritmasıdır ve zaman karmaşıklığı, kapasite değerlerinden bağımsız olarak düğüm (`V`) ve kenar (`E`) sayısına bağlıdır.

Her BFS çağrısı, komşuluk matrisi üzerinde çalıştığından:

```cpp
for (int v = 0; v < V; v++)
```

her düğüm için `V` genişliğinde bir tarama yapar; bu nedenle tek bir BFS çağrısı `O(V²)` zaman alır (matris temsili nedeniyle).

Edmonds-Karp'ın kanıtlanmış özelliğine göre, en fazla `O(V × E)` kez artırıcı yol bulunabilir. Bu nedenle toplam zaman karmaşıklığı:

```text
O(V × E) × O(V²) = O(V³ × E)
```

Komşuluk **listesi** ile daha sıkı bir sınır olan `O(V × E²)` elde edilebilir; bu kodda komşuluk **matrisi** kullanıldığı için gerçek pratik maliyet yukarıdaki gibi matris taramasına bağlıdır.

### Best Case

En iyi durumda, kaynaktan hedefe tek bir artırıcı yol tüm akışı taşır ve BFS sadece bir kez çalışır. Bu durumda karmaşıklık tek bir BFS çağrısının maliyeti olan `O(V²)`'ye yaklaşır.

### Average Case

Ortalama durumda algoritma birden fazla artırıcı yol bulabilir ve her yol için BFS tekrar çalıştırılır. Bu nedenle toplam çalışma süresi, bulunan artırıcı yol sayısına ve grafın yapısına bağlıdır. Edmonds-Karp için standart olarak belirlenmiş bir ortalama durum karmaşıklığı bulunmadığından, temel analiz en kötü durum karmaşıklığı üzerinden yapılır.

### Worst Case

En kötü durumda, `O(V × E)` sınırına yakın sayıda artırıcı yol bulunur ve her biri için `O(V²)` maliyetli bir BFS çalıştırılır. Toplam en kötü durum karmaşıklığı:

```text
O(V³ × E)
```

olur.

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
* `queue<int> q` → en fazla `O(V)`

alan kullanır.

`graph[V][V]` giriş matrisi olduğu için yardımcı alan karmaşıklığına ayrıca dahil edilmez. Ancak `graph` ve `rGraph` birlikte değerlendirildiğinde toplam bellek kullanımı da `O(V²)`'dir.

Baskın terim `V × V` boyutundaki matrisler olduğu için alan karmaşıklığı:

```text
O(V²)
```

olur.
