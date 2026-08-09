# DSATUR Algoritması - Graph Coloring 

Bu bölümde Graf Boyama (Graph Coloring) probleminin, düğümleri **doygunluk derecesine (saturation degree)** göre sırayla ele alarak çözülmesini sağlayan **DSATUR (Degree of Saturation)** yöntemi açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Basit Greedy (sıralı) graf boyama yönteminde düğümlerin sabit bir sırayla (`0`'dan `V-1`'e) ele alınması, bazı graflarda gereğinden fazla renk kullanılmasına yol açar. DSATUR, bir sonraki boyanacak düğümü sabit sırayla değil, o an en çok "kısıtlanmış" (yani komşularında en fazla farklı renk bulunan) düğümü seçerek daha az renk kullanan çözümler üretmek amacıyla geliştirilmiştir.
- **Hangi eksikliği gidermektedir?** Greedy yöntemin düğüm sırasına duyarlı olması eksikliğini giderir; her adımda "en kritik" düğümü önce boyayarak, ilerleyen adımlarda ortaya çıkabilecek renk çakışmalarını önceden azaltır.
- **Önceki yöntemlere göre avantajı nedir?** Aynı Greedy gibi kesin (exact) yöntemlere göre çok hızlı çalışırken, düğüm seçim stratejisi sayesinde pratikte Greedy'den genellikle daha az veya en fazla eşit sayıda renk kullanır. Özellikle yoğun (dense) ve düzensiz yapılı graflarda bu fark belirginleşir.
- **Hangi durumda tercih edilir?**
  - Greedy'ye göre daha kaliteli (daha az renkli) bir çözüme ihtiyaç duyulan, ancak yine de kesin çözüm (backtracking, brute force) yöntemlerinin çok yavaş kalacağı büyük graflarda.
  - Kayıt tahsisi (register allocation), zaman çizelgeleme (scheduling), harita/frekans atama gibi pratik renk sayısının önemli olduğu uygulamalarda.
  - Düğüm sırasının sonucu olumsuz etkileyebileceği düzensiz yapılı graflarda.
- **Hangi durumda tercih edilmez?**
  - Minimum renk sayısının (kromatik sayının) kesin olarak garanti edilmesi gerektiği durumlarda; DSATUR da Greedy gibi bir sezgisel (heuristic) yöntemdir ve optimum sonucu garanti etmez.
  - Çok küçük graflarda; ek doygunluk derecesi hesaplamaları getirdiği karmaşıklığa değmeyebilir.

## Algoritmanın Çalışma Mantığı

Algoritma iki temel adımdan oluşur:

1. **İlk düğümün seçilmesi ve boyanması:** Graftaki en yüksek dereceye (en çok komşuya) sahip düğüm bulunur ve doğrudan `0` numaralı renk atanır.
2. **Kalan düğümlerin doygunluk derecesine göre sırayla boyanması:** Her adımda, henüz boyanmamış düğümler arasından **doygunluk derecesi** (o düğüme komşu olan, farklı renklerle boyanmış düğüm sayısı) en yüksek olan düğüm seçilir.
   - Eşitlik durumunda, düğüm derecesi (toplam komşu sayısı) daha yüksek olan düğüm tercih edilir (tie-breaker).
   - Seçilen düğümün boyanmış komşularının renkleri "kullanılamaz" olarak işaretlenir.
   - Kullanılabilir en küçük numaralı renk bulunur ve düğüme atanır.
   - Bir sonraki adım için işaretler sıfırlanır.

Her adım için:

1. Boyanmamış düğümler arasında doygunluk derecesi hesaplanır (`getSaturationDegree`).
2. Eşitlik varsa düğüm derecesi karşılaştırılır (`getNodeDegree`).
3. En uygun düğüm seçilir (`getHighestSaturationNode`).
4. Seçilen düğümün boyanmış komşularının renkleri kullanılamaz işaretlenir.
5. `0`'dan başlanarak kullanılabilir ilk (en küçük) renk bulunur ve düğüme atanır.
6. İşaretler bir sonraki düğüm için sıfırlanır.
7. Tüm düğümler boyandıktan sonra kullanılan toplam renk sayısı (en büyük renk numarası + 1) elde edilir.

### Örnek

Kodda verilen 5 düğümlü komşuluk matrisine göre graf şu şekildedir:

```text
0 - 1, 0 - 2
1 - 0, 1 - 2, 1 - 3
2 - 0, 2 - 1, 2 - 3
3 - 1, 3 - 2, 3 - 4
4 - 3
```

Düğüm dereceleri: `deg(0)=2, deg(1)=3, deg(2)=3, deg(3)=3, deg(4)=1`

Düğümlerin boyanması şu şekilde gerçekleşir:

```text
Baslangic: en yuksek dereceli dugum secilir -> Dugum 1 ---> Renk 0

Adim 1: Doygunlugu en yuksek dugumler (0,2,3) esit (=1);
        dugum derecesi en yuksek olan Dugum 2 secilir ---> Renk 1
Adim 2: Doygunlugu en yuksek dugumler (0,3) esit (=2);
        dugum derecesi daha yuksek olan Dugum 3 secilir ---> Renk 2
Adim 3: Doygunlugu en yuksek olan Dugum 0 secilir ---> Renk 2
Adim 4: Kalan tek dugum olan Dugum 4 secilir ---> Renk 0
```

Sonuç:

```text
Dugum 0 ---> Renk 2
Dugum 1 ---> Renk 0
Dugum 2 ---> Renk 1
Dugum 3 ---> Renk 2
Dugum 4 ---> Renk 0
```

Algoritma toplam **3** renk kullanır.

> DSATUR algoritması, Greedy'nin aksine düğümleri sabit bir sırayla değil, o anki boyama durumuna göre dinamik olarak seçer. Bu sayede komşularında daha fazla farklı renk bulunan (dolayısıyla daha "kısıtlı" olan) düğümler önce boyanarak sonraki adımlarda oluşabilecek renk çakışmaları azaltılmaya çalışılır.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon DoygunlukDerecesi(u, KomsulukMatrisi, Sonuc, V):
    BenzersizRenkler = bos kume
    Döngü (v = 0'dan V - 1'e kadar):
        Eğer KomsulukMatrisi[u][v] == 1 ve Sonuc[v] != -1 ise:
            BenzersizRenkler.ekle(Sonuc[v])
    Döndür BenzersizRenkler.boyut

Fonksiyon DugumDerecesi(u, KomsulukMatrisi, V):
    derece = 0
    Döngü (v = 0'dan V - 1'e kadar):
        Eğer KomsulukMatrisi[u][v] == 1 ise:
            derece = derece + 1
    Döndür derece

Fonksiyon EnYuksekDoygunlukluDugum(KomsulukMatrisi, Sonuc, V):
    maxDoygunluk = -1, maxDerece = -1, secilenDugum = -1
    Döngü (u = 0'dan V - 1'e kadar):
        Eğer Sonuc[u] == -1 ise:
            doygunluk = DoygunlukDerecesi(u, ...)
            derece = DugumDerecesi(u, ...)
            Eğer doygunluk > maxDoygunluk ise:
                maxDoygunluk = doygunluk, maxDerece = derece, secilenDugum = u
            Değilse Eğer doygunluk == maxDoygunluk ve derece > maxDerece ise:
                maxDerece = derece, secilenDugum = u
    Döndür secilenDugum

Fonksiyon DsaturGraphColoring(KomsulukMatrisi, V):
    Sonuc = V boyutunda dizi, tumu -1

    // En yuksek dereceli dugumu bul ve ilk renkle boya
    ilkDugum = en yuksek DugumDerecesi degerine sahip dugum
    Sonuc[ilkDugum] = 0

    Kullanilabilir = V boyutunda dizi, tumu true

    Döngü (sayac = 1'den V - 1'e kadar):
        u = EnYuksekDoygunlukluDugum(KomsulukMatrisi, Sonuc, V)

        Döngü (v = 0'dan V - 1'e kadar):
            Eğer KomsulukMatrisi[u][v] == 1 ve Sonuc[v] != -1 ise:
                Kullanilabilir[Sonuc[v]] = false

        Döngü (renk = 0'dan V - 1'e kadar):
            Eğer Kullanilabilir[renk] == true ise:
                Döngüden çık

        Sonuc[u] = renk

        Döngü (v = 0'dan V - 1'e kadar):
            Eğer KomsulukMatrisi[u][v] == 1 ve Sonuc[v] != -1 ise:
                Kullanilabilir[Sonuc[v]] = true

    Döndür max(Sonuc) + 1
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`vector<vector<int>> adjMatrix`** — Grafın komşuluk matrisini saklar. `adjMatrix[u][v] == 1` ise `u` ve `v` düğümleri arasında kenar vardır. `const` referans olarak fonksiyonlara geçirilir, böylece içerikleri değiştirilemez ve gereksiz kopyalama önlenir.

* **`vector<int> result(V, -1)`** — Her düğüme atanan rengi tutar. Başlangıçta `-1` ile doldurulur; bu, düğümün henüz boyanmadığını ifade eder.

* **`unordered_set<int> uniqueColors`** — `getSaturationDegree` fonksiyonu içinde, bir düğümün boyanmış komşularının **farklı** renklerini tutmak için kullanılır. Küme yapısı sayesinde aynı renk birden fazla kez sayılmaz.

* **`vector<bool> available(V, true)`** — Bir düğüm boyanırken hangi renklerin komşular tarafından kullanıldığını, dolayısıyla hangi renklerin o düğüm için uygun olduğunu tutar.

* **`break`** — Kullanılabilir en küçük renk bulunduğunda renk arama döngüsünü sonlandırır.

* **İç içe `for` döngüleri** — Doygunluk derecesi hesaplama, düğüm derecesi hesaplama, komşuları işaretleme ve işaretleri sıfırlama işlemlerinin tamamı komşuluk matrisi üzerinde döngülerle yürütülür.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
```

- `<iostream>`: `cout` kullanılarak ekrana çıktı yazdırmak için kullanılır.
- `<vector>`: Komşuluk matrisini, renk atamalarını ve kullanılabilirlik bilgisini saklamak için kullanılır.
- `<unordered_set>`: Bir düğümün komşularındaki farklı renkleri (doygunluk derecesini) hesaplamak için kullanılır.
- `<algorithm>`: Standart algoritma yardımcı fonksiyonları için dahil edilmiştir.

---

## Zaman Karmaşıklığı

Kalan `V - 1` düğümü boyayan dış döngü toplam `V - 1` kez çalışır. Her adımda:

- **`EnYuksekDoygunlukluDugum`** fonksiyonu boyanmamış her düğüm için `getSaturationDegree` (`O(V)`) ve `getNodeDegree` (`O(V)`) fonksiyonlarını çağırır; bu da her çağrıda `O(V)` sürer ve en kötü durumda `V` düğüm için toplam `O(V²)` işlem gerektirir.
- Seçilen düğümün komşularını işaretleme/sıfırlama işlemleri `O(V)` sürer.
- Kullanılabilir en küçük rengi bulma işlemi en kötü durumda `O(V)` sürer.

Bu nedenle her dış döngü adımı en kötü durumda `O(V²)` sürer ve toplam zaman karmaşıklığı:

```text
O(V) × O(V²) = O(V³)
```

olur. Bu, Greedy algoritmanın `O(V²)` karmaşıklığına göre daha yüksektir; DSATUR'un her adımda "en uygun" düğümü yeniden hesaplaması ek maliyet getirir.

### Best Case — O(V³)

Komşuluk matrisi temsili kullanıldığı için, düğümlerin gerçek komşu sayısından bağımsız olarak her adımda tüm düğümler için doygunluk ve derece hesabı yapılır. `break` yalnızca renk arama adımını kısaltır; bu nedenle en iyi durumda dahi karmaşıklık `O(V³)` olarak kalır.

### Average Case — O(V³)

Ortalama durumda da her adımda tüm boyanmamış düğümler için doygunluk ve derece hesaplaması yapıldığından ortalama durum karmaşıklığı `O(V³)` olarak kabul edilir.

### Worst Case — O(V³)

En kötü durumda (örneğin tam bağlı bir graf) her adımda hem doygunluk/derece hesaplamaları hem de komşu işaretleme/sıfırlama işlemleri tam `V` boyutunda çalışır. Toplam işlem sayısı `(V - 1) × O(V²)` ile orantılıdır ve bu da `O(V³)` verir.

> Not: Komşuluk matrisi (adjacency matrix) kullanıldığı için karmaşıklık grafın kenar sayısından (E) bağımsızdır. Komşuluk listesi ve öncelik kuyruğu (priority queue) gibi yapılarla optimize edilmiş bir DSATUR implementasyonunda bu karmaşıklık `O((V + E) log V)` seviyesine düşürülebilir.

---

## Alan Karmaşıklığı

**Ek alan karmaşıklığı: O(V)**

Algoritmanın çalışması sırasında kullanılan ek veri yapıları:

* `vector<int> result(V, -1)` → `O(V)`
* `vector<bool> available(V, true)` → `O(V)`
* `unordered_set<int> uniqueColors` (fonksiyon çağrısı başına, en fazla `V` eleman) → `O(V)`
* `u`, `v`, `cr`, `maxSatDegree`, `maxNodeDegree`, `selectedNode` gibi değişkenler → `O(1)`

Bu nedenle grafın kendisini (`adjMatrix`) giriş verisi olarak kabul ettiğimizde, algoritmanın **ek alan karmaşıklığı `O(V)`**'dir.

Ancak `adjMatrix` de dahil olmak üzere programın kullandığı **toplam bellek** hesaplanırsa, `V × V` boyutundaki komşuluk matrisi nedeniyle toplam alan:

```text
O(V²)
```

olur.