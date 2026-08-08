# Greedy Graph Coloring Algoritması

Bu bölümde Graf Boyama (Graph Coloring) probleminin, düğümleri sırayla ele alıp her düğüme kullanılabilecek en küçük numaralı rengi atayarak çözülmesini sağlayan **Greedy (Açgözlü) Graph Coloring** yöntemi açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Graf boyama probleminde komşu düğümlerin farklı renklere sahip olmasını sağlayacak minimum renk sayısını (kromatik sayı) kesin olarak bulmanın NP-Zor olması nedeniyle ortaya çıkan performans kaybını engellemek, bunun yerine düğümleri sırayla boyayarak kabul edilebilir bir çözüm üretmek için geliştirilmiştir.
- **Hangi eksikliği gidermektedir?** Minimum renk sayısını kesin olarak bulmak için bütün boyama kombinasyonlarını denemenin düğüm sayısı arttıkça yarattığı büyük zaman kaybını giderir; bütün olasılıkları denemek yerine her düğümü komşularının kullanmadığı en küçük renge atayarak daha kısa sürede sonuç üretir.
- **Önceki yöntemlere göre avantajı nedir?** En büyük avantajı basitliğidir. Çalışma mantığı ve kodlanması oldukça kolaydır, karmaşık veri yapılarına ihtiyaç duymaz ve kesin çözüm yöntemlerine (backtracking, brute force) göre çok daha hızlı çalışır.
- **Hangi durumda tercih edilir?**
  - Düğüm sayısının fazla olduğu büyük graflarda.
  - Hızlı ve kabul edilebilir bir çözüme ihtiyaç duyulduğu durumlarda.
  - Kromatik sayının kesin olarak bulunmasının zorunlu olmadığı durumlarda.
  - Zamanlama, kayıt tahsisi (register allocation), frekans atama gibi yaklaşık çözümün yeterli olduğu uygulamalarda.
- **Hangi durumda tercih edilmez?**
  - Minimum renk sayısının kesin olarak bulunması gereken durumlarda. Greedy algoritma düğümlerin ele alınış sırasından etkilendiği için bazı graflarda gereğinden fazla renk kullanır ve algoritma optimum çözümden (kromatik sayıdan) uzaklaşabilir.

## Algoritmanın Çalışma Mantığı

Düğümler `0`'dan `V-1`'e kadar sırayla ele alınır. İlk düğüme doğrudan `0` numaralı renk atanır. Sonraki her düğüm için, o düğüme komşu olan ve daha önce boyanmış düğümlerin renkleri tespit edilir; bu renkler o düğüm için "kullanılamaz" olarak işaretlenir ve kullanılabilir en küçük numaralı renk seçilir.

Her düğüm için:

1. Düğümün bütün komşuları taranır.
2. Komşu boyanmışsa, o komşunun rengi "kullanılamaz" olarak işaretlenir.
3. `0`'dan başlanarak kullanılabilir ilk (en küçük) renk bulunur.
4. Bu renk düğüme atanır.
5. Bir sonraki düğümün doğru değerlendirilebilmesi için işaretlenen renkler tekrar "kullanılabilir" durumuna alınır.
6. Bütün düğümler boyandıktan sonra kullanılan toplam renk sayısı (en büyük renk numarası + 1) döndürülür.

### Örnek

Kodda verilen 5 düğümlü komşuluk matrisine göre graf şu şekildedir:

```text
0 - 1, 0 - 2
1 - 0, 1 - 2, 1 - 3
2 - 0, 2 - 1, 2 - 3
3 - 1, 3 - 2, 3 - 4
4 - 3
```

Düğümlerin boyanması şu şekilde gerçekleşir:

```text
Dugum 0 ---> Renk 0
Dugum 1 ---> Renk 1   (0 nolu komsusu Renk 0 kullaniyor)
Dugum 2 ---> Renk 2   (0 ve 1 nolu komsulari Renk 0 ve Renk 1 kullaniyor)
Dugum 3 ---> Renk 0   (1 ve 2 nolu komsulari Renk 1 ve Renk 2 kullaniyor)
Dugum 4 ---> Renk 1   (3 nolu komsusu Renk 0 kullaniyor)
```

Algoritma toplam **3** renk kullanır.

> Greedy Graph Coloring algoritması bir düğüm için en uygun rengi aramaz; komşularının kullanmadığı ilk (en küçük numaralı) rengi kullanır. Bu nedenle düğümlerin ele alınış sırası sonucu etkileyebilir.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon GreedyGraphColoring(KomsulukMatrisi, V):

    // Her dugume atanan rengi tutar, baslangicta hicbiri boyanmamistir
    Sonuc = V boyutunda dizi, tumu -1

    // Ilk dugumu ilk renge (0) boya
    Sonuc[0] = 0

    // Renklerin kullanilabilir olup olmadigini takip eden dizi
    Kullanilabilir = V boyutunda dizi, tumu true

    // Kalan V-1 dugumu sirayla boya
    Döngü (u = 1'den V - 1'e kadar):

        // u dugumunun komsularini tara
        Döngü (v = 0'dan V - 1'e kadar):
            Eğer KomsulukMatrisi[u][v] == 1 ve Sonuc[v] != -1 ise:
                Kullanilabilir[Sonuc[v]] = false

        // Kullanilabilir en kucuk numarali rengi bul
        Döngü (renk = 0'dan V - 1'e kadar):
            Eğer Kullanilabilir[renk] == true ise:
                Döngüden çık

        // Bulunan rengi u dugumune ata
        Sonuc[u] = renk

        // Kullanilabilir dizisini bir sonraki dugum icin sifirla
        Döngü (v = 0'dan V - 1'e kadar):
            Eğer KomsulukMatrisi[u][v] == 1 ve Sonuc[v] != -1 ise:
                Kullanilabilir[Sonuc[v]] = true

    // Kullanilan toplam renk sayisini dondur (en buyuk renk numarasi + 1)
    Döndür max(Sonuc) + 1
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`vector<vector<int>> adjMatrix`** — Grafın komşuluk matrisini saklar. `adjMatrix[u][v] == 1` ise `u` ve `v` düğümleri arasında kenar vardır. `const` referans olarak fonksiyona geçirilir, böylece fonksiyon içinde değiştirilemez ve gereksiz kopyalama önlenir.

* **`vector<int> result(V, -1)`** — Her düğüme atanan rengi tutar. Başlangıçta `-1` ile doldurulur; bu, düğümün henüz boyanmadığını ifade eder.

* **`vector<bool> available(V, true)`** — Bir düğüm boyanırken hangi renklerin komşular tarafından kullanıldığını, dolayısıyla hangi renklerin o düğüm için uygun olduğunu tutar.

* **`break`** — Kullanılabilir en küçük renk bulunduğunda renk arama döngüsünü sonlandırır. Greedy algoritma ilk uygun (en küçük) rengi kullandığı için bulunduktan sonra diğer renkler kontrol edilmez.

* **İç içe `for` döngüleri** — Her düğüm için komşuları işaretleme ve işaretleri sıfırlama işlemleri, komşuluk matrisi üzerinde çift döngü ile yürütülür.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <vector>
```

- `<iostream>`: `cout` kullanılarak ekrana çıktı yazdırmak için kullanılır.
- `<vector>`: Komşuluk matrisini, renk atamalarını ve kullanılabilirlik bilgisini saklamak için kullanılır.

---

## Zaman Karmaşıklığı

Düğümleri sırayla gezen dış döngü:

```cpp
for (int u = 1; u < V; u++)
```

toplam `V - 1` kez çalışır.

Her düğüm için komşuları tarayan ve daha sonra işaretleri sıfırlayan iki iç döngü:

```cpp
for (int v = 0; v < V; v++)
```

komşuluk matrisi kullanıldığı için her seferinde `V` düğümün tamamını kontrol eder (gerçek komşu olup olmadığına bakılmaksızın). Buna ek olarak kullanılabilir rengi bulan döngü de en kötü durumda `V` renge kadar kontrol yapabilir.

Bu nedenle her dış döngü adımı `O(V)` sürer ve toplam zaman karmaşıklığı:

```text
O(V × V) = O(V²)
```

olur.

### Best Case — O(V²)

Komşuluk matrisi temsili kullanıldığı için, düğümün gerçekte hiç komşusu olmasa bile matrisin tamamı (`V` eleman) taranmak zorundadır; `break` yalnızca renk arama adımını kısaltır, komşu tarama adımını kısaltmaz. Bu nedenle en iyi durumda dahi karmaşıklık:

```text
O(V²)
```

olarak kalır.

### Average Case — O(V²)

Ortalama durumda her düğüm için hem komşuların taranması hem de kullanılabilir rengin bulunması `V` ile orantılı işlem gerektirir. Bu nedenle ortalama durum karmaşıklığı da:

```text
O(V²)
```

şeklinde kabul edilir.

### Worst Case — O(V²)

En kötü durumda (örneğin tam bağlı bir graf — her düğüm diğer bütün düğümlere komşuysa) her düğüm için hem komşu tarama hem işaret sıfırlama hem de renk arama adımlarının tamamı `V`'ye kadar çalışır. Toplam kontrol sayısı:

```text
(V - 1) × V
```

ile orantılıdır ve bu da:

```text
O(V²)
```

verir.

> Not: Komşuluk matrisi (adjacency matrix) kullanıldığı için bu algoritmanın karmaşıklığı grafın kenar sayısından (E) bağımsız olarak her zaman `O(V²)`'dir. Komşuluk listesi (adjacency list) kullanılsaydı karmaşıklık `O(V + E)` seviyesine düşürülebilirdi.

---

## Alan Karmaşıklığı

**Ek alan karmaşıklığı: O(V)**

Algoritmanın çalışması sırasında kullanılan ek veri yapıları:

* `vector<int> result(V, -1)` → `O(V)`
* `vector<bool> available(V, true)` → `O(V)`
* `u`, `v`, `cr`, `maxColor` gibi değişkenler → `O(1)`

Bu nedenle grafın kendisini (`adjMatrix`) giriş verisi olarak kabul ettiğimizde, algoritmanın **ek alan karmaşıklığı `O(V)`**'dir.

Ancak `adjMatrix` de dahil olmak üzere programın kullandığı **toplam bellek** hesaplanırsa, `V × V` boyutundaki komşuluk matrisi nedeniyle toplam alan:

```text
O(V²)
```

olur.
