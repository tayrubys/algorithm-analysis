# Ford-Fulkerson Algoritması — Maximum Flow

Bu bölümde bir akış ağında (flow network) kaynak (source) düğümden hedef (sink) düğüme gönderilebilecek maksimum akış miktarını bulan **Ford-Fulkerson** yöntemi açıklanmaktadır. Kodda yol arama işlemi özyinelemeli (recursive) **DFS** ile yapılır.
## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Bir ağda, kenarların taşıyabileceği kapasiteler göz önünde bulundurularak kaynaktan hedefe gönderilebilecek en yüksek akış miktarını bulmak için geliştirilmiştir. Ulaştırma, ağ trafiği, eşleştirme problemleri gibi birçok alanda karşılaşılan "kapasiteli ağda maksimum akış" problemini çözer.
- **Hangi eksikliği gidermektedir?** Akışı doğrudan artırmaya çalışan basit yaklaşımlar, bir kez yanlış yönde gönderilen akışı geri alamaz ve bu nedenle optimum sonuca ulaşamayabilir. Ford-Fulkerson, **residual graph (kalan kapasite grafiği)** kullanarak daha önce gönderilen akışın gerekirse geri alınmasına (geri kenar üzerinden) izin verir; böylece hatalı bir yoldan gönderilen akış sonradan düzeltilebilir.
- **Önceki yöntemlere göre avantajı nedir?** Ford-Fulkerson, uygun koşullar altında (tam sayılı kapasitelerde) kesin maksimum akışı bulur. Residual graph fikri sayesinde, akışı geri alma esnekliği kazandırarak açgözlü (greedy) yaklaşımların takıldığı durumlardan kurtulur. Kodlanması BFS tabanlı Edmonds-Karp'a göre daha basittir; DFS özyinelemeli bir fonksiyonla doğrudan ifade edilebilir.
- **Hangi durumda tercih edilir?**
  - Kaynaktan hedefe kesin maksimum akış değerinin bulunması gerektiği durumlarda.
  - Ağ trafiği, boru hattı, ulaşım ağı gibi kapasite kısıtlı sistemlerin analizinde.
  - Eşleştirme (matching), kesme (min-cut) gibi maksimum akışa indirgenebilen problemlerde.
  - Kapasite değerlerinin küçük/tam sayı olduğu ve DFS'in az sayıda artırıcı yol bulacağı durumlarda.
- **Hangi durumda tercih edilmez?**
  - DFS ile artırıcı yol arandığından, kapasitelerin büyük olduğu bazı graflarda çok sayıda küçük artırıcı yol bulunabilir; bu da algoritmanın gereksiz yere yavaş çalışmasına yol açar. Bu gibi durumlarda BFS tabanlı Edmonds-Karp veya Dinic gibi algoritmalar tercih edilir.
  - Çok büyük ağlarda performansın kritik olduğu ve garanti edilmiş polinomsal bir çalışma süresine ihtiyaç duyulan durumlarda.
  - Ağın sürekli değiştiği (dinamik) ve her değişiklikte akışın baştan hesaplanmasının maliyetli olduğu durumlarda.

## Algoritmanın Çalışma Mantığı

Algoritma, kaynaktan hedefe **artırıcı yol (augmenting path)** bulunamayana kadar tekrar tekrar çalışır. Her artırıcı yol bulunduğunda, bu yol üzerinden mümkün olan en fazla akış gönderilir. Yol arama işlemi, kaynaktan başlayarak mümkün olduğunca derine inen özyinelemeli bir **DFS** ile yapılır.

Her adımda:

1. `visited` dizisi sıfırlanır ve residual graph (`rGraph`) üzerinde DFS ile kaynaktan hedefe bir yol aranır.
2. DFS, bulunduğu düğümden gidilebilecek ilk uygun (ziyaret edilmemiş ve kapasitesi olan) komşuya geçer; hedefe ulaşana kadar veya çıkmaza girene kadar derinlemesine ilerler.
3. Bir düğümden gidilecek uygun komşu kalmazsa, DFS bir önceki düğüme geri döner (backtrack) ve oradan başka bir komşu dener.
4. Hedefe ulaşılırsa DFS `true` döndürür ve `parent` dizisi, hedeften kaynağa kadar izlenen yolu tutar.
5. Yol bulunamazsa (DFS `false` döndürürse) algoritma durur ve o ana kadar biriken toplam akış sonuç olarak döndürülür.
6. Yol bulunduysa, bu yol üzerindeki kenarların kalan kapasitelerinin **minimumu** (`path_flow`) hesaplanır.
7. Yol üzerindeki her kenarın kapasitesi `path_flow` kadar azaltılır (ileri yön), ters yönüne ise `path_flow` kadar kapasite eklenir (geri yön); bu, gerekirse akışın sonradan geri alınabilmesini sağlar.
8. `path_flow`, toplam akışa (`max_flow`) eklenir ve 1. adıma dönülür.

### Örnek

Koddaki `main` fonksiyonunda tanımlı ağ, `0` kaynak ve `5` hedef düğüm olacak şekilde ele alınsın:

```text
0 -> 1 : 8      0 -> 4 : 3
1 -> 2 : 9
2 -> 4 : 7      2 -> 5 : 2
3 -> 5 : 5
4 -> 2 : 7      4 -> 3 : 4
```

DFS, her düğümde komşuları `0`'dan `V-1`'e doğru sırayla dener ve ilk uygun komşuya dalar. Bu şekilde bulunan artırıcı yollar ve gönderilen akışlar sırasıyla:

```text
Yol 1: 0 -> 1 -> 2 -> 4 -> 3 -> 5      gönderilen akış = 4   (sınırlayan kenar: 4 -> 3)
Yol 2: 0 -> 1 -> 2 -> 5                gönderilen akış = 2   (sınırlayan kenar: 2 -> 5)
```

Bu noktadan sonra DFS, `0 -> 4` kenarını (kalan kapasite `3`) denese bile `4` düğümünden `3`'e giden kapasite tükendiği ve başka bir çıkışı olmadığı için hedefe ulaşamaz; kaynaktan hedefe başka artırıcı yol kalmaz ve algoritma durur.

Toplam maksimum akış: **4 + 2 = 6**

> Ford-Fulkerson, bir yolu bulduktan sonra o yolu "kalıcı" kabul etmez. Residual graph üzerindeki geri kenarlar sayesinde, ilerideki bir adımda daha önce gönderilen akışın bir kısmı dolaylı olarak geri alınabilir. Bu, algoritmanın kesin maksimum akışı bulmasını garanti eder. DFS tabanlı arama, BFS'ten farklı olarak "en kısa" yolu değil, "ilk bulduğu" yolu kullanır; bu nedenle bulunan yolların sırası ve sayısı BFS'e göre değişebilir, fakat toplam maksimum akış aynıdır.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon DFS(ResidualGraf, u, Hedef, Parent, Ziyaret):

    Ziyaret[u] = True

    Eğer u == Hedef ise:
        Döndür True

    Döngü (v = 0'dan DugumSayisi - 1'e kadar):
        Eğer Ziyaret[v] == False ve ResidualGraf[u][v] > 0 ise:
            Parent[v] = u

            Eğer DFS(ResidualGraf, v, Hedef, Parent, Ziyaret) == True ise:
                Döndür True

    Döndür False


Fonksiyon FordFulkerson(Graf, Kaynak, Hedef):

    ResidualGraf = Graf'in bir kopyasi
    Parent = DugumSayisi boyutunda dizi
    Ziyaret = DugumSayisi boyutunda dizi
    MaksimumAkis = 0

    Döngü (Sonsuz döngü):

        Ziyaret = tum dugumler icin False

        Eğer DFS(ResidualGraf, Kaynak, Hedef, Parent, Ziyaret) == False ise:
            Döngüden çık

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

* **`bool dfs(...)` (özyineleme / recursion)** — Kaynaktan hedefe bir yol aramak için kullanılan özyinelemeli fonksiyon. Fonksiyon kendi içinde tekrar kendisini çağırarak (`dfs(rGraph, v, t, parent, visited)`) grafı derinlemesine gezer; çağrı yığını (call stack) gezilen yolun uzunluğu kadar derinleşir.

* **`bool visited[V]`** ve **`memset(visited, 0, sizeof(visited))`** — DFS sırasında hangi düğümlerin ziyaret edildiğini tutar. Her yeni DFS çağrısından (yani her yeni artırıcı yol aramasından) önce `memset` ile tekrar sıfırlanır.

* **`int parent[V]`** — DFS sırasında her düğüme hangi düğümden ulaşıldığını saklar. Bulunan yolun hedef düğümden kaynak düğüme doğru geriye izlenmesini sağlar.

* **`while (true)` + `break`** — DFS artık bir yol bulamayana kadar döngünün devam etmesini sağlar; `dfs(...)` `false` döndürdüğünde `break` ile döngü sonlandırılır.

* **`INT_MAX`** (`<climits>`) — Yol üzerindeki minimum kalan kapasiteyi (`path_flow`) hesaplarken başlangıç değeri olarak kullanılır; böylece ilk karşılaştırmada her zaman daha küçük bir değer bulunması garanti edilir.

* **`min()`** (`<algorithm>`) — Yol üzerindeki kenarlardan en düşük kalan kapasiteyi bulmak için kullanılır; bu değer o yoldan gönderilebilecek maksimum akış miktarıdır.

* **`#define V 6`** — Graf düğüm sayısını sabit olarak tanımlar; komşuluk matrisinin boyutunu belirler.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <climits>
#include <cstring>
#include <algorithm>
```

- `<iostream>`: `cout` kullanılarak sonucun ekrana yazdırılması için kullanılır.
- `<climits>`: `path_flow` değişkeninin başlangıç değeri olan `INT_MAX` sabiti için kullanılır.
- `<cstring>`: `visited` dizisini her DFS çağrısından önce sıfırlamak için kullanılan `memset` fonksiyonu için kullanılır.
- `<algorithm>`: Yol üzerindeki minimum kapasiteyi bulmak için kullanılan `min` fonksiyonu için kullanılır.

Not: Bu sürümde BFS kullanılmadığı için `<queue>` kütüphanesine ihtiyaç yoktur; yol arama, `queue` yerine fonksiyonun kendi çağrı yığınını (call stack) kullanan özyinelemeli `dfs` ile yapılır.

---

## Zaman Karmaşıklığı

Bu uygulama, yol aramanın DFS ile yapıldığı **klasik Ford-Fulkerson** yöntemidir. Edmonds-Karp'ın aksine, bulunan artırıcı yol sayısı yalnızca düğüm/kenar sayısına değil, **kenar kapasitelerinin değerlerine de** bağlıdır; bu nedenle algoritma "pseudo-polinomsal" bir çalışma süresine sahiptir.

Tek bir DFS çağrısı, komşuluk matrisi üzerinde her düğüm için `V` genişliğinde bir tarama yapabileceğinden en kötü durumda `O(V²)` zaman alır.

Tam sayılı kapasitelerde her artırıcı yol, toplam akışı en az `1` birim artırdığından, algoritma en fazla `max_flow` (bulunan maksimum akış değeri) kadar kez DFS çalıştırabilir. Bu nedenle toplam zaman karmaşıklığı:

```text
O(max_flow × V²)
```

veya kenar sayısı cinsinden yaygın olarak:

```text
O(E × max_flow)
```

şeklinde ifade edilir.

### Best Case

En iyi durumda, DFS'in bulduğu ilk yol(lar) akışın tamamını veya büyük bir kısmını taşır ve çok az sayıda artırıcı yolla algoritma sona erer (örnekteki gibi 2 yol). Bu durumda karmaşıklık, birkaç DFS çağrısının toplam maliyeti olan `O(V²)`'ye yakındır.

### Average Case

Ortalama durumda, DFS'in "en kısa" değil "ilk bulduğu" yolu seçmesi nedeniyle, bulunan artırıcı yolların akış miktarı küçük olabilir ve birden fazla DFS çağrısı gerekebilir. Toplam maliyet, artırıcı yol sayısına bağlı olarak `O(V²)` ile `O(E × max_flow)` arasında değişir.

### Worst Case

En kötü durumda, DFS her seferinde yalnızca çok küçük (örneğin `1` birimlik) bir artırıcı yol bulur ve bu durum `max_flow` kez tekrarlanabilir. Her DFS çağrısı `O(V²)` maliyetli olduğundan, toplam en kötü durum karmaşıklığı:

```text
O(E × max_flow)
```

olur. Kapasite değerleri büyüdükçe `max_flow` da büyüyebileceğinden, bu, algoritmanın büyük kapasiteli graflarda yavaşlayabileceği anlamına gelir — Edmonds-Karp'ın `O(V³ × E)` gibi kapasiteden bağımsız bir sınıra sahip olmamasının temel nedeni budur.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(V²)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Algoritmanın en büyük yardımcı bellek kullanımı, residual graph için oluşturulan `V × V` boyutundaki matristen kaynaklanır:

```cpp
int rGraph[V][V];
```

Bu yapı `O(V²)` alan kullanır.

Bunun yanında:

* `visited[V]` → `O(V)`
* `parent[V]` → `O(V)`
* `dfs` fonksiyonunun **özyineleme (recursion) çağrı yığını** → en kötü durumda yol, tüm düğümleri kapsayabileceğinden `O(V)`

alan kullanır.

`graph[V][V]` giriş matrisi olduğu için yardımcı alan karmaşıklığına ayrıca dahil edilmez. Ancak `graph` ve `rGraph` birlikte değerlendirildiğinde toplam bellek kullanımı da `O(V²)`'dir.

Baskın terim `V × V` boyutundaki matrisler olduğu için toplam alan karmaşıklığı:

```text
O(V²)
```

olur.
