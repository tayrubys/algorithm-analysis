# 2-Approximation Algoritmasi - Vertex Cover Problemi 

Bu bölümde, Vertex Cover probleminin, grafın kenarlarını sırayla dolaşıp henüz kapsanmamış her kenarın iki ucunu da örtüye ekleyerek çözülmesini sağlayan **açgözlü (greedy) 2-yaklaşıklı (2-approximation)** algoritma açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Vertex Cover probleminin NP-Hard olması nedeniyle, büyük graflarda kesin (minimum) çözümü bulmanın pratik olarak imkansız hâle gelmesinin önüne geçmek; kesin sonuçtan ödün vererek, gerçek minimum köşe sayısının **en fazla iki katı** büyüklüğünde bir örtüyü çok kısa sürede bulmak için geliştirilmiştir.
- **Hangi eksikliği gidermektedir?** Bütün köşe alt kümelerini deneyerek minimum vertex cover'ı bulmaya çalışmanın getirdiği üstel zaman maliyetini giderir. Her kenarı örtmek için o kenarın **her iki** ucunu da alarak, minimum çözümü aramak yerine hızlı ve garantili bir yaklaşık çözüm üretir.
- **Önceki yöntemlere (kesin çözümlere) göre avantajı nedir?** Çalışma mantığı ve kodlanması oldukça basittir; yalnızca grafın kenarlarını tek geçişte dolaşır. Kesin çözüm yöntemlerinin aksine, çözüm kalitesi hakkında **matematiksel bir garanti** sunar: bulunan örtü, gerçek minimum örtünün en fazla iki katı büyüklüğündedir.
- **Hangi durumda tercih edilir?**
  - Köşe ve kenar sayısının fazla olduğu, kesin çözümün pratik olarak hesaplanamayacağı durumlarda.
  - Hızlı bir sonuca ihtiyaç duyulduğu ve "gerçek minimumun en fazla iki katı" garantisinin yeterli kabul edildiği durumlarda.
- **Hangi durumda tercih edilmez?**
  - Minimum vertex cover'ın kesin olarak bulunması gereken durumlarda. Bu algoritma, bir kenarın iki ucunu da örtüye eklediği için gereğinden fazla köşe seçebilir; örneğin bir köşenin tek başına bütün kenarları örtebileceği durumlarda bile, algoritma gereksiz ikinci bir köşeyi de örtüye dahil edebilir.

## Algoritmanın Çalışma Mantığı

Algoritma, bütün köşeleri başlangıçta "seçilmemiş" (`visited[i] = false`) olarak işaretler. Ardından köşeleri sırayla dolaşır:

1. Her köşe (`u`) için, eğer bu köşe henüz örtüye dahil edilmediyse, komşuluk listesi taranır.
2. Komşuluk listesinde henüz örtüye dahil edilmemiş **ilk** komşu (`v`) bulunur; bu, henüz örtülmemiş bir `(u, v)` kenarının seçilmesi anlamına gelir.
3. Bu kenarın **her iki ucu da** (`u` ve `v`) örtüye dahil edilir ve `visited` olarak işaretlenir.
4. Bir kenar bulunduğunda döngüden çıkılır; çünkü bu köşe (`u`) için artık bir kenar seçilmiş ve örtüye eklenmiştir.
5. Bütün köşeler bu şekilde tek tek işlendikten sonra, `visited` olarak işaretlenmiş köşeler, bulunan vertex cover kümesini oluşturur.

Bu yaklaşımın temel fikri şudur: bir kenar seçildiğinde, o kenarın iki ucundan **en az biri** gerçek minimum örtüde de bulunmak zorundadır (aksi hâlde o kenar örtülemez). Algoritma ihtiyatlı davranarak kenarın iki ucunu da örtüye ekler; bu da bulunan çözümün gerçek minimumdan **en fazla iki kat** büyük olmasını garanti eder.

### Örnek

Aşağıdaki graf verilsin (7 köşe, kenarlar):

```text
0-1, 0-2, 1-3, 3-4, 4-5, 5-6
```

Algoritma köşe `0`'dan başlar; henüz seçilmemiştir. Komşuluk listesinde ilk komşu olan `1` de seçilmemiştir; bu nedenle `(0, 1)` kenarı seçilir ve hem `0` hem `1` örtüye eklenir. Sıradaki köşe `1` zaten örtüde olduğu için atlanır. Köşe `2`'ye gelindiğinde henüz seçilmemiştir; komşusu `0` zaten örtüde olsa da algoritma `0`'ı tekrar kontrol etmez, çünkü `0`'ın komşuluk listesinde `2`'ye rastlamadan önce döngüden çıkmıştı — bu adımda köşe `2` için komşuluk listesi taranır ve `0` bulunur; ancak `0` zaten `visited` olduğundan bu kenar atlanır ve köşe `2` örtüye eklenmez. Algoritma bu şekilde devam ederek `3-4` ve `5-6` kenarlarını da örtüye dahil eder ve sonuçta örtü kümesi `{0, 1, 3, 4, 5, 6}` olarak bulunur.

> Bu algoritma **minimum** vertex cover'ı garanti etmez; yalnızca gerçek minimumun en fazla iki katı büyüklüğünde bir örtü bulunacağını garanti eder.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon VertexCover(Graf, V):

    // Başlangıçta bütün köşeleri ziyaret edilmemiş olarak işaretle
    Ziyaret_Edildi = V boyutunda, tamamı false olan dizi

    Döngü (u = 0'dan V - 1'e kadar):

        Eğer Ziyaret_Edildi[u] == false ise:

            // u'nun komşularını dolaş, henüz seçilmemiş ilk komşuyu bul
            Döngü (v, u'nun komşuluk listesindeki her köşe için):

                Eğer Ziyaret_Edildi[v] == false ise:

                    // (u, v) kenarını sec, her iki ucu da örtüye ekle
                    Ziyaret_Edildi[u] = true
                    Ziyaret_Edildi[v] = true

                    // Bu köşe için bir kenar bulundu, döngüden çık
                    Döngüden çık

    // Ziyaret edilmiş (örtüye dahil edilmiş) köşeleri döndür/yazdır
    Döndür Ziyaret_Edildi
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`class Graph`** — Grafı bir sınıf olarak modeller; köşe sayısını (`V`) ve komşuluk listelerini (`adj`) bir arada tutar. `addEdge` ve `printVertexCover` üye fonksiyonları aracılığıyla graf üzerinde işlem yapılmasını sağlar.

* **`list<int> *adj`** — Grafı **komşuluk listesi (adjacency list)** biçiminde saklayan, `V` elemanlı bir bağlı liste dizisine işaret eden bir pointer'dır. `adj[i]`, `i` köşesine komşu olan bütün köşelerin listesini tutar. Komşuluk listesi, komşuluk matrisine (`adjacency matrix`) kıyasla, özellikle seyrek (sparse) graflarda daha az bellek kullanır.

* **`Graph::Graph(int V)` (Constructor)** — Sınıf oluşturulurken köşe sayısını ayarlar ve `adj` dizisini `new list<int>[V]` ile dinamik olarak oluşturur.

* **`addEdge(int v, int w)`** — Graf **yönsüz (undirected)** olduğu için, `v`'yi `w`'nin komşuluk listesine, `w`'yi de `v`'nin komşuluk listesine ekleyerek kenarı iki yönlü temsil eder.

* **`bool visited[V]`** — Her köşenin vertex cover kümesine dahil edilip edilmediğini tutan bir dizidir. Algoritmanın temel karar mekanizmasını oluşturur: bir köşe zaten `visited` ise, o köşeyle ilgili kenarlar tekrar işlenmez.

* **`list<int>::iterator`** — Bir köşenin komşuluk listesindeki elemanları (komşularını) sırayla dolaşmak için kullanılan yineleyicidir (iterator).

* **`break`** — Bir köşe (`u`) için henüz örtülmemiş ilk kenar bulunduğunda, o köşenin kalan komşularının kontrol edilmesine gerek kalmadığı için döngüyü sonlandırır.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <list>
```

- `<iostream>`: `cout` kullanılarak vertex cover kümesindeki köşelerin ekrana yazdırılması için kullanılır.
- `<list>`: Grafın komşuluk listesi biçiminde temsil edilmesi için `list<int>` yapısı kullanılır.

---

## Zaman Karmaşıklığı

Algoritma, dış döngüde bütün köşeleri (`V` adet) dolaşır:

```cpp
for (int u = 0; u < V; u++)
```

Her köşe için, komşuluk listesindeki komşular en kötü durumda tamamen taranabilir. Bütün köşelerin komşuluk listelerinin toplam uzunluğu, yönsüz bir grafta kenar sayısının iki katına (`2E`) eşittir. Bu nedenle toplam zaman karmaşıklığı:

```text
O(V + E)
```

olur. Burada `V` köşe sayısını, `E` ise kenar sayısını ifade eder.

### Best / Average / Worst Case — O(V + E)

Algoritma her köşeyi ve her kenarı en fazla sabit sayıda kez ziyaret ettiğinden (her kenar ya seçilir ve iki ucu işaretlenir, ya da bir ucu zaten işaretlenmiş olduğu için atlanır), en iyi, ortalama ve en kötü durum karmaşıklığı arasında önemli bir fark yoktur. Algoritma her koşulda:

```text
O(V + E)
```

karmaşıklığıyla çalışır; bu da onu büyük graflar için oldukça pratik kılar.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(V + E)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Grafın komşuluk listesi (`adj`), toplamda `V` liste başlığı ve yönsüz bir grafta her kenar için iki kayıt (`2E`) içerir; bu nedenle grafın kendisi:

```text
O(V + E)
```

bellek kullanır.

Köşelerin ziyaret durumunu tutan `visited` dizisi, `V` elemanlı olduğu için `O(V)` ek bellek kullanır.

`u`, `v` ve yineleyici (`iterator`) gibi diğer değişkenler sabit miktarda bellek kullandığı için `O(1)` alan tüketir.

Bu nedenle algoritmanın toplam alan karmaşıklığı, grafın kendisini saklamak için gereken bellekle birlikte:

```text
O(V + E)
```

olur.