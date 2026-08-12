# Branch and Bound Algoritmasi - 0/1 Knapsack Problemi

Bu bölümde, 0/1 Knapsack probleminin, karar ağacındaki umut vaat etmeyen dalları teorik bir üst sınır (upper bound) hesabıyla erken eleyerek çözülmesini sağlayan **Branch and Bound (Dal-Sınır)** yöntemi açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Saf özyinelemeli ve memoization/bottom-up çözümlerin aksine, karar ağacının **bütün** dallarını (veya bütün `(n, capacity)` durumlarını) gezmek yerine, sonucun kesinlikle mevcut en iyi çözümü geçemeyeceği dalları daha keşfedilmeden eleyerek gereksiz hesaplamaların önüne geçmek için geliştirilmiştir.
- **Hangi eksikliği gidermektedir?** Bottom-up DP çözümü, kapasite (`W`) büyük olduğunda `O(N × W)` boyutunda bir tablo gerektirir ve bu, kapasite büyüdükçe pratik olmaktan çıkar. Branch and Bound, tablo boyutuna bağlı kalmadan, yalnızca gerçekten "umut vaat eden" dalları inceleyerek bu sınırlamayı aşmaya çalışır.
- **Önceki yöntemlere göre avantajı nedir?** Her dala girmeden önce, o daldan devam edilirse elde edilebilecek **teorik maksimum değeri** (kesirli/fractional knapsack mantığıyla) hesaplar. Bu üst sınır, o ana kadar bulunan en iyi çözümden düşükse, ilgili dal hiç incelenmeden elenir. Pratikte bu, özellikle eşyaların değer/ağırlık oranına göre sıralanmasıyla birleştiğinde, gezilen dal sayısını saf özyinelemeye kıyasla ciddi ölçüde azaltabilir.
- **Hangi durumda tercih edilir?**
  - Eşya sayısının orta-büyük olduğu, ancak dinamik programlama tablosunun (`O(N × W)`) kapasite nedeniyle çok büyük veya verimsiz kalacağı durumlarda.
  - Eşyaların değer/ağırlık oranlarının belirgin farklılıklar gösterdiği, dolayısıyla üst sınır (bound) hesabının dalları etkili bir şekilde eleyebildiği durumlarda.
- **Hangi durumda tercih edilmez?**
  - Bütün eşyaların değer/ağırlık oranlarının birbirine çok yakın olduğu durumlarda; bu durumda üst sınır hesabı dalları etkili şekilde eleyemez ve algoritma pratikte saf özyinelemeye yakın (üstel) bir performansa döner.
  - Kapasitenin (`W`) küçük ve eşya sayısının az olduğu, dolayısıyla bottom-up DP'nin zaten hızlı ve basit çalıştığı durumlarda; bu gibi durumlarda Branch and Bound'un ek karmaşıklığı gereksizdir.

## Algoritmanın Çalışma Mantığı

Algoritma, karar ağacını **BFS (genişlik öncelikli arama)** mantığıyla, bir kuyruk (`queue<Node>`) kullanarak gezer. Her düğüm (`Node`), o ana kadar verilen kararların bir özetini tutar: hangi seviyede (`level`) olunduğu, o ana kadarki toplam değer (`profit`), toplam ağırlık (`weight`) ve bu daldan devam edilirse ulaşılabilecek teorik üst sınır (`bound`).

1. **Ön hazırlık:** Eşyalar, değer/ağırlık oranına göre büyükten küçüğe sıralanır (`cmp` fonksiyonu). Bu sıralama, üst sınır hesabının (kesirli knapsack mantığıyla) mümkün olduğunca isabetli olmasını sağlar.
2. **Başlangıç düğümü:** `level = -1`, `profit = 0`, `weight = 0` olan bir kök düğüm oluşturulup kuyruğa eklenir.
3. **Düğüm işleme:** Kuyruktan bir düğüm (`u`) alınır. Eğer bu düğüm son seviyedeyse (bütün eşyalar için karar verilmişse) daha fazla dallanma yapılmaz.
4. **1. Dal — Eşyayı al:** Bir sonraki eşyanın ağırlığı ve değeri mevcut toplamlara eklenerek yeni bir düğüm (`v`) oluşturulur. Ağırlık kapasiteyi aşmıyorsa ve elde edilen değer, o ana kadarki en iyi sonuçtan (`maxProfit`) büyükse, `maxProfit` güncellenir.
5. **Üst sınır hesabı (`bound`):** Bu yeni düğümden devam edilirse ulaşılabilecek teorik maksimum değer hesaplanır. Bu hesap, kalan eşyaları sırayla (gerekirse bir eşyanın **kesirli** bir kısmını alarak) cantaya doldurmayı varsayar; bu nedenle gerçek 0/1 çözümünden daha iyimser (üst sınır niteliğinde) bir tahmindir.
6. **Budama (pruning):** Eğer bu üst sınır, o ana kadar bulunan en iyi gerçek çözümden (`maxProfit`) küçük veya eşitse, bu dalın daha iyi bir sonuç üretme ihtimali yoktur ve kuyruğa **eklenmez** (dal budanır). Aksi hâlde düğüm kuyruğa eklenerek daha sonra incelenmek üzere saklanır.
7. **2. Dal — Eşyayı alma:** Ağırlık ve değer değişmeden, yalnızca bir sonraki seviyeye geçilerek aynı üst sınır ve budama mantığı uygulanır.
8. Kuyruk boşalana kadar bu işlem tekrarlanır ve sonunda `maxProfit`, bulunabilen en iyi gerçek (0/1) çözüm olarak döndürülür.

### Örnek

Aşağıdaki girdiler verilsin:

```text
W = 10
arr = {(agirlik: 2, deger: 40), (agirlik: 3.14, deger: 50),
       (agirlik: 1.98, deger: 100), (agirlik: 5, deger: 95),
       (agirlik: 3, deger: 30)}
```

Algoritma önce eşyaları değer/ağırlık oranına göre sıralar; oranı en yüksek olan (`agirlik: 1.98, deger: 100`) öne alınır. Kök düğümden başlanarak her eşya için "al" ve "alma" dalları oluşturulur. Örneğin ilk birkaç yüksek oranlı eşya alınarak kapasiteye yakın, yüksek değerli bir çözüme erken ulaşılır ve `maxProfit` bu değere güncellenir. Daha sonra incelenen bazı dallar için hesaplanan üst sınır bu `maxProfit` değerinin altında kaldığından, bu dallar hiç açılmadan elenir. Bütün kuyruk boşaldığında `maxProfit`, `Maximum possible profit` olarak döndürülür.

> Branch and Bound, kesin (0/1) çözümü garanti eder; `bound` fonksiyonunda kullanılan "kesirli eşya alma" mantığı yalnızca **elemeyi hızlandırmak** için kullanılır, gerçek çözüme kesirli eşya dahil edilmez.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon Bound(u, n, W, Esyalar):
    Eğer u.agirlik >= W ise:
        Döndür 0

    UstSinir = u.deger
    j = u.seviye + 1
    ToplamAgirlik = u.agirlik

    Döngü (j < n VE ToplamAgirlik + Esyalar[j].agirlik <= W):
        ToplamAgirlik += Esyalar[j].agirlik
        UstSinir += Esyalar[j].deger
        j = j + 1

    Eğer j < n ise:
        UstSinir += (W - ToplamAgirlik) * Esyalar[j].deger / Esyalar[j].agirlik

    Döndür UstSinir


Fonksiyon Knapsack(W, Esyalar, n):
    Esyalari deger/agirlik oranina gore buyukten kucuge sirala

    Kuyruk = boş kuyruk
    Baslangic.seviye = -1
    Baslangic.deger = Baslangic.agirlik = 0
    Kuyruk.ekle(Baslangic)

    maxDeger = 0

    Döngü (Kuyruk boş değilken):
        u = Kuyruk.cikar()

        Eğer u.seviye == n - 1 ise:
            Devam et (bu dugumden dallanma)

        v.seviye = u.seviye + 1

        // 1. Dal: Esyayi al
        v.agirlik = u.agirlik + Esyalar[v.seviye].agirlik
        v.deger = u.deger + Esyalar[v.seviye].deger

        Eğer v.agirlik <= W VE v.deger > maxDeger ise:
            maxDeger = v.deger

        v.ustSinir = Bound(v, n, W, Esyalar)
        Eğer v.ustSinir > maxDeger ise:
            Kuyruk.ekle(v)

        // 2. Dal: Esyayi alma
        v.agirlik = u.agirlik
        v.deger = u.deger
        v.ustSinir = Bound(v, n, W, Esyalar)
        Eğer v.ustSinir > maxDeger ise:
            Kuyruk.ekle(v)

    Döndür maxDeger
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`struct Item`** — Bir eşyanın `weight` (ağırlık, `float`) ve `value` (değer, `int`) bilgilerini bir arada tutar.

* **`struct Node`** — Karar ağacındaki bir düğümün durumunu tutar: `level` (hangi eşya seviyesinde olunduğu), `profit` (o ana kadarki toplam değer), `bound` (bu daldan devam edilirse ulaşılabilecek teorik üst sınır) ve `weight` (o ana kadarki toplam ağırlık). Bu yapı, algoritmanın hem mevcut durumu hem de gelecekteki potansiyeli aynı anda takip etmesini sağlar.

* **`bool cmp(Item a, Item b)`** — Eşyaları değer/ağırlık oranına göre büyükten küçüğe sıralamak için `sort` fonksiyonuna verilen karşılaştırma fonksiyonudur. Sıralama, üst sınır hesabının isabetli ve dolayısıyla budamanın etkili olmasını sağlar.

* **`int bound(Node u, int n, int W, Item arr[])`** — Bir düğümden devam edilirse ulaşılabilecek teorik maksimum değeri, kalan eşyaları (gerekirse kesirli olarak) kapasiteye doldurma mantığıyla hesaplar. Bu, **kesirli (fractional) knapsack** çözümünün üst sınır olarak kullanılmasıdır ve gerçek 0/1 çözümünden her zaman büyük veya eşittir.

* **`queue<Node> Q`** — Karar ağacındaki incelenmeyi bekleyen düğümleri tutan bir kuyruktur. Algoritmanın ağacı **BFS (genişlik öncelikli)** biçimde, umut vermeyen dalları budayarak gezmesini sağlar.

* **Budama (`if (v.bound > maxProfit)`)** — Algoritmanın temelini oluşturan mekanizmadır; bir düğümün teorik üst sınırı mevcut en iyi çözümü geçemiyorsa, o düğüm kuyruğa hiç eklenmez ve o dal tamamen elenmiş olur.

---

## Kullanılan Kütüphaneler

```cpp
#include <bits/stdc++.h>
```

Bu başlık, aşağıdaki standart kütüphaneleri tek seferde programa dahil eder:

- `<iostream>`: `cout` ile sonucun ekrana yazdırılması için kullanılır.
- `<queue>`: Karar ağacındaki düğümlerin BFS mantığıyla gezilmesi için `queue<Node>` yapısı kullanılır.
- `<algorithm>`: Eşyaların değer/ağırlık oranına göre sıralanması için `sort` fonksiyonu kullanılır.

`<bits/stdc++.h>` standart bir C++ başlık dosyası olmadığı için bazı derleyiciler tarafından desteklenmeyebilir; taşınabilirlik gerektiren projelerde ilgili başlıkların ayrı ayrı eklenmesi önerilir.

---

## Zaman Karmaşıklığı

Budama mekanizması olmasaydı, algoritma her eşya için iki dala ayrılan tam bir karar ağacını gezerdi; bu da saf özyinelemeli çözümdeki gibi:

```text
O(2^N)
```

karmaşıklığına karşılık gelirdi. Branch and Bound'un amacı, üst sınır hesabıyla bu ağacın büyük bir kısmını **hiç gezmeden** elemektir.

### Best Case — O(N log N)

En iyi durumda (örneğin eşyaların değer/ağırlık oranları arasında belirgin farklar varsa), üst sınır hesabı dalların büyük çoğunluğunu erken eler ve algoritma neredeyse doğrudan en iyi çözüme ulaşır. Bu durumda baskın maliyet, eşyaların sıralanmasıdır:

```text
O(N log N)
```

### Average Case — Değişken (Girdiye Bağlı)

Ortalama durumda, kaç dalın budanacağı doğrudan eşyaların değer/ağırlık oranlarının dağılımına bağlıdır. Pratikte Branch and Bound, saf özyinelemeye kıyasla çoğu girdide çok daha az düğüm inceler; ancak kesin bir ortalama durum formülü verilemez, çünkü bu tamamen girdiye bağlıdır.

### Worst Case — O(2^N)

En kötü durumda (örneğin bütün eşyaların değer/ağırlık oranları birbirine çok yakınsa), üst sınır hesabı dalları etkili bir şekilde eleyemez ve algoritma neredeyse bütün karar ağacını gezmek zorunda kalır. Bu durumda zaman karmaşıklığı, saf özyinelemeli çözümle aynı üstel seviyeye çıkar:

```text
O(2^N)
```

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: En kötü durumda O(2^N)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Algoritma, incelenmeyi bekleyen düğümleri bir kuyrukta (`queue<Node> Q`) tutar. Budama etkili olduğunda kuyrukta aynı anda bulunan düğüm sayısı oldukça azdır; ancak üst sınır hesabının dalları elemekte başarısız olduğu en kötü durumda, kuyruk karar ağacının geniş bir kesitini (üstel sayıda düğümü) aynı anda tutabilir. Bu nedenle en kötü durum alan karmaşıklığı:

```text
O(2^N)
```

olarak kabul edilir.

Pratikte, iyi bir sıralama ve etkili budama sayesinde kuyrukta aynı anda bulunan düğüm sayısı bundan çok daha azdır; bu da Branch and Bound'un en kötü durum garantisi zayıf olsa da, pratikte genellikle DP çözümlerinden daha az bellek kullanabilmesinin nedenidir.

`arr` dizisi girdinin kendisi olduğu için ek alan karmaşıklığına dahil edilmez.