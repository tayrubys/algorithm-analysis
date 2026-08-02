# First Fit Decreasing (FFD) Algoritması — Bin Packing

Bu bölümde Bin Packing probleminin, nesnelerin önce büyükten küçüğe sıralanıp ardından sığabilecekleri ilk kutuya yerleştirilerek çözülmesini sağlayan **First Fit Decreasing (Azalan İlk Uygun)** yöntemi açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

* **Bu algoritma neden geliştirilmiştir?** First Fit ve Best Fit algoritmaları nesnelerin geliş sırasına bağlıdır. Eğer çok büyük bir nesne sona kalırsa ve mevcut kutularda sadece küçük boşluklar varsa, bu büyük nesne için gereksiz yere yeni bir kutu açılır. FFD, "önce büyük nesneleri yerleştir, küçükler kalan aralara zaten sıkışır" mantığıyla bu israfı önlemek için geliştirilmiştir.
* **Hangi eksikliği gidermektedir?** Girdi sırasının yarattığı rastgeleliği ortadan kaldırarak, basit sezgisel (heuristic) algoritmalar arasında kutu israfını (fragmantasyon) en aza indirir.
* **Önceki yöntemlere göre avantajı nedir?** First Fit ve Best Fit algoritmalarına kıyasla genellikle en az kutuyu harcayan (optimuma en yakın) sonucu üretir. Nesneleri önden sıraladığı için devasa boyutlu eşyaların sona kalıp fazladan kutu açtırması riskini sıfıra indirir.
* **Hangi durumda tercih edilir?**
  * Nesnelerin tamamının boyutlarının baştan bilindiği çevrim dışı (offline) durumlarda.
  * Kutu sayısının ve firenin minimumda tutulmasının çok kritik olduğu endüstriyel senaryolarda (Örn: standart kargo yüklemesi, kumaş kesimi).
* **Hangi durumda tercih edilmez?**
  * Nesnelerin sisteme sırayla geldiği ve tüm listenin baştan bilinmediği çevrim içi (online) sistemlerde kullanılamaz (Sıralama yapılamayacağı için).
  * Çok büyük veri setlerinde sıralama işleminin (Sorting) yaratacağı zaman kaybının tolere edilemeyeceği durumlarda.

## Algoritmanın Çalışma Mantığı

Algoritma temelde iki adımdan oluşur: Önce sıralama yapılır, ardından First Fit mantığı birebir uygulanır.

1. Bütün nesnelerin ağırlıkları alınır.
2. Nesneler büyükten küçüğe (azalan sırada) sıralanır.
3. Sıralanmış listedeki en büyük nesne alınır.
4. Açılmış kutular ilk kutudan başlanarak kontrol edilir.
5. Nesnenin sığabileceği ilk kutu bulunur ve yerleştirilir.
6. Eğer mevcut kutuların hiçbirine sığmazsa yeni bir kutu açılır.
7. Bütün nesneler yerleştirildikten sonra kullanılan kutu sayısı döndürülür.

### Örnek

Kutu kapasitesi `10` ve nesneler aşağıdaki gibi olsun (Önceki algoritmalarla aynı dizi):

```text
2, 5, 4, 7, 1, 3, 8
```

FFD algoritması önce bu diziyi büyükten küçüğe sıralar:

```text
8, 7, 5, 4, 3, 2, 1
```

Sıralanmış nesnelerin yerleştirilmesi şu şekilde gerçekleşir:

```text
Kutu 1: 8 + 2 = 10
Kutu 2: 7 + 3 = 10
Kutu 3: 5 + 4 + 1 = 10
```

Algoritma toplam **3** kutu kullanır.

> **Not:** Aynı dizi için First Fit ve Best Fit algoritmaları 4 kutu kullanmıştı. FFD'nin büyük nesneleri öne alması sayesinde hiçbir kutuda boşluk kalmadı ve optimum sonuca ulaşıldı.

---

## Sözde Kod (Pseudo-Code)

Bu algoritmada kod tekrarını önlemek için DRY (Don't Repeat Yourself) prensibi uygulanmış ve yerleştirme işlemi için halihazırda yazılmış olan `FirstFit` fonksiyonu çağrılmıştır.

```text
Fonksiyon FirstFitDecreasing(Agirliklar, NesneSayisi, KutuKapasitesi):

    // Geçersiz kapasite veya nesne sayısını kontrol et
    Eğer KutuKapasitesi <= 0 veya NesneSayisi <= 0 ise:
        Hata döndür

    // Orijinal veriyi bozmamak için ağırlıkları yeni bir listeye kopyala
    SiraliAgirliklar = Agirliklar dizisinin kopyası

    // Listeyi büyükten küçüğe (azalan sırada) sırala
    Sirala(SiraliAgirliklar, AZALAN_SIRA)

    // Sıralanmış listeyi doğrudan First Fit fonksiyonuna gönder ve sonucu al
    KullanilanKutu = FirstFit(SiraliAgirliklar, NesneSayisi, KutuKapasitesi)

    // Toplam kutu sayısını döndür
    Döndür KullanilanKutu
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`vector<int> sortedWeight`** — `const` olarak gelen orijinal ağırlık dizisini değiştirmemek (bozmamak) için, dizinin kopyalandığı dinamik vektördür. Bu, güvenli (savunmacı) programlama yaklaşımıdır.
* **`rbegin()` ve `rend()` (Reverse Iterator)** — `std::sort` fonksiyonu varsayılan olarak küçükten büyüğe sıralar. Diziyi büyükten küçüğe sıralamak için vektörün sonundan başına doğru ilerleyen ters iteratörler (reverse iterators) kullanılmıştır.
* **`data()`** — Vektörün içindeki belleğin başlangıç adresini bir C-stili işaretçi (pointer) olarak döndürür. `firstFit` fonksiyonu parametre olarak klasik bir dizi beklediği için, `sortedWeight.data()` kullanılarak veri uyumluluğu çok şık bir şekilde sağlanmıştır.
* **DRY (Don't Repeat Yourself)** — FFD için First Fit döngüleri baştan yazılmamış, sadece veri sıralanıp mevcut `firstFit` fonksiyonuna paslanarak modüler bir mimari kurulmuştur.

---

## Zaman Karmaşıklığı

Algoritma iki temel aşamadan oluşur: Sıralama ve Yerleştirme.

1. **Sıralama (Sorting):** `std::sort` fonksiyonu (Genellikle Introsort algoritmasını kullanır) $\mathcal{O}(N \log N)$ zaman alır.
2. **Yerleştirme (First Fit):** Sıralanmış dizi First Fit fonksiyonuna gönderildiğinde, iç içe iki döngü sebebiyle en kötü durumda $\mathcal{O}(N^2)$ zaman harcanır.

Toplam zaman karmaşıklığı: $\mathcal{O}(N \log N) + \mathcal{O}(N^2)$ şeklindedir.
Büyük O gösteriminde baskın (en yavaş büyüyen) terim baz alındığı için genel zaman karmaşıklığı:

```text
O(N²)
```

olarak kabul edilir.

### Best Case — O(N log N)

En iyi durumda, sıralama işlemi $\mathcal{O}(N \log N)$ zaman alır. Sıralama bittikten sonra tüm nesneler ilk kontrol edilen kutuya sığıyorsa (veya her kutuya yerleşimde hiç geriye dönülmüyorsa) First Fit kısmı $\mathcal{O}(N)$ sürer. Ancak sıralamanın maliyeti $\mathcal{O}(N)$'den büyük olduğu için en iyi durum zaman karmaşıklığı sıralamanın hızına takılır: $\mathcal{O}(N \log N)$ olur.

### Average Case ve Worst Case — O(N²)

Ortalama ve en kötü durumlarda, nesnelerin sıralanmasından sonraki süreçte iç içe döngüler tetiklenir. Her bir nesnenin uygun yerini bulmak için açık olan bütün kutuların kontrol edilmesi gerekebilir. Bu kontrol süreci dominant hale geldiği için ortalama ve en kötü durum karmaşıklığı:

```text
O(N²)
```

olarak kabul edilir.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(N)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Algoritmada iki önemli ek bellek kullanımı vardır:

1. Orijinal dizinin sırasını bozmamak için oluşturulan `vector<int> sortedWeight` kopyası $\mathcal{O}(N)$ kadar yer kaplar.
2. `firstFit` fonksiyonuna gidildiğinde kutuların kalan kapasitelerini saklamak için açılan `binRemaining` vektörü de en kötü durumda $\mathcal{O}(N)$ kadar yer kaplar.

Matematiksel olarak $\mathcal{O}(N) + \mathcal{O}(N) = \mathcal{O}(2N)$ olsa da, Büyük O notasyonunda sabit katsayılar dikkate alınmaz. Bu nedenle algoritmanın toplam ek alan karmaşıklığı:

```text
O(N)
```

olur.