# Best Fit Algoritması — Bin Packing

Bu bölümde Bin Packing probleminin, nesneleri yerleştirme sonrasında en az boşluk bırakacak kutuya yerleştirerek çözülmesini sağlayan **Best Fit** yöntemi açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

* **Bu algoritma neden geliştirilmiştir?** Bin Packing probleminde kutuların içinde kalan boş alanları daha verimli kullanmak ve her nesneyi mevcut kutular arasından kendisine en uygun olan kutuya yerleştirerek kabul edilebilir bir çözüm üretmek için geliştirilmiştir.
* **Hangi eksikliği gidermektedir?** First Fit algoritması bir nesneyi sığdığı ilk kutuya yerleştirir ve diğer kutuların daha uygun olup olmadığını kontrol etmez. Best Fit ise bütün açık kutuları kontrol ederek yerleştirme sonrasında en az boşluk bırakacak kutuyu seçer.
* **Önceki yöntemlere göre avantajı nedir?** First Fit algoritmasına göre kutuların kalan kapasitelerini daha dikkatli değerlendirir. Nesneyi sığdığı ilk kutuya yerleştirmek yerine, nesne yerleştirildikten sonra en az boşluk kalacak kutuyu seçer. Bu nedenle bazı durumlarda First Fit'ten daha az kutu kullanabilir.
* **Hangi durumda tercih edilir?**

  * Kutuların içindeki boş alanların mümkün olduğunca azaltılmasının istendiği durumlarda.
  * Minimum kutu sayısının kesin olarak bulunmasının zorunlu olmadığı durumlarda.
  * Hızlı ve yaklaşık bir çözüme ihtiyaç duyulduğu durumlarda.
  * Nesnelerin sisteme sırayla geldiği çevrim içi (online) sistemlerde.
* **Hangi durumda tercih edilmez?**

  * Minimum kutu sayısının kesin olarak bulunması gereken durumlarda.
  * Nesne sayısının çok fazla olduğu ve her nesne için bütün açık kutuların kontrol edilmesinin zaman kaybı oluşturduğu durumlarda.
  * Best Fit de nesnelerin geliş sırasından etkilendiği için bazı durumlarda optimum çözümden uzaklaşabilir. Örneğin `weight = [4, 4, 4, 6, 6, 6]` ve kapasite `10` olduğunda Best Fit `4` kutu kullanabilir. Ancak daha uygun bir yerleştirme ile `3` kutu yeterlidir.

## Algoritmanın Çalışma Mantığı

Nesneler verildikleri sıraya göre tek tek ele alınır. Her nesne için daha önce açılmış olan bütün kutular kontrol edilir.

Her nesne için:

1. Nesnenin ağırlığı alınır.
2. Daha önce açılmış bütün kutular kontrol edilir.
3. Nesnenin sığabileceği kutular belirlenir.
4. Nesne yerleştirildikten sonra her kutuda kalacak boş alan hesaplanır.
5. Yerleştirme sonrasında en az boşluk bırakacak kutu seçilir.
6. Nesne seçilen kutuya yerleştirilir.
7. Kutunun kalan kapasitesi nesnenin ağırlığı kadar azaltılır.
8. Nesne mevcut kutuların hiçbirine sığmazsa yeni bir kutu açılır.
9. Bütün nesneler yerleştirildikten sonra kullanılan kutu sayısı döndürülür.

### Örnek

Kutu kapasitesi `10` ve nesneler aşağıdaki gibi olsun:

```text
2, 5, 4, 7, 1, 3, 8
```

Nesnelerin yerleştirilmesi şu şekilde gerçekleşir:

```text
Kutu 1: 2 + 5 + 1 = 8
Kutu 2: 4
Kutu 3: 7 + 3 = 10
Kutu 4: 8
```

Algoritma toplam **4** kutu kullanır.

`3` ağırlığındaki nesne yerleştirilirken açık kutuların kalan kapasiteleri şöyledir:

```text
Kutu 1 kalan kapasite: 2
Kutu 2 kalan kapasite: 6
Kutu 3 kalan kapasite: 3
```

`3` ağırlığındaki nesne ikinci ve üçüncü kutuya sığabilir.

İkinci kutuya yerleştirilirse:

```text
6 - 3 = 3
```

birim boşluk kalır.

Üçüncü kutuya yerleştirilirse:

```text
3 - 3 = 0
```

birim boşluk kalır.

Best Fit, yerleştirme sonrasında daha az boşluk bıraktığı için üçüncü kutuyu seçer.

> Best Fit algoritması nesnenin sığdığı ilk kutuyu kullanmaz. Açılmış bütün kutuları kontrol eder ve nesne yerleştirildikten sonra en az boşluk kalacak kutuyu seçer.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon BestFit(Agirliklar, NesneSayisi, KutuKapasitesi):

    // Kullanılan kutu sayısını başlangıçta sıfır yap
    KullanilanKutuSayisi = 0

    // Kutuların kalan kapasitelerini saklayacak bir dizi oluştur
    KalanKapasiteler = NesneSayisi boyutunda dizi

    // Nesneleri sırayla yerleştir
    Döngü (i = 0'dan NesneSayisi - 1'e kadar):

        // Nesne ağırlığının geçerli olup olmadığını kontrol et
        Eğer Agirliklar[i] <= 0 veya Agirliklar[i] > KutuKapasitesi ise:
            Hata döndür

        // Henüz uygun kutu bulunmadığını belirt
        EnIyiKutuIndeksi = -1

        // Yerleştirme sonrasında kalabilecek en küçük boşluğu tut
        MinimumKalanKapasite = KutuKapasitesi + 1

        // Daha önce açılmış bütün kutuları kontrol et
        Döngü (j = 0'dan KullanilanKutuSayisi - 1'e kadar):

            // Nesne mevcut kutuya sığıyorsa
            Eğer KalanKapasiteler[j] >= Agirliklar[i] ise:

                // Yerleştirme sonrasında kutuda kalacak boşluğu hesapla
                YerlestirmeSonrasiKalan =
                    KalanKapasiteler[j] - Agirliklar[i]

                // Daha az boşluk bırakan bir kutu bulunduysa güncelle
                Eğer YerlestirmeSonrasiKalan <
                    MinimumKalanKapasite ise:

                    MinimumKalanKapasite =
                        YerlestirmeSonrasiKalan

                    EnIyiKutuIndeksi = j

        // Nesnenin sığabileceği hiçbir kutu bulunamadıysa
        Eğer EnIyiKutuIndeksi == -1 ise:

            // Yeni kutu aç ve kalan kapasitesini hesapla
            KalanKapasiteler[KullanilanKutuSayisi] =
                KutuKapasitesi - Agirliklar[i]

            KullanilanKutuSayisi =
                KullanilanKutuSayisi + 1

        // Uygun bir kutu bulunduysa
        Aksi halde:

            // Nesneyi en az boşluk bırakacak kutuya yerleştir
            KalanKapasiteler[EnIyiKutuIndeksi] =
                KalanKapasiteler[EnIyiKutuIndeksi]
                - Agirliklar[i]

    // Kullanılan toplam kutu sayısını döndür
    Döndür KullanilanKutuSayisi
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`const int weights[]`** — Nesnelerin ağırlıklarını saklar. `weights[i]`, `i` indeksindeki nesnenin ağırlığıdır. `const`, dizinin fonksiyon içinde değiştirilmesini engeller.

* **`vector<int> binRemaining(n)`** — Her kutunun kalan kapasitesini tutar (`binRemaining[0]` → 1. kutu, `binRemaining[1]` → 2. kutu vb.). En kötü durumda her nesne ayrı bir kutu açabileceğinden boyutu `N` olarak alınır.

* **`bestBinIndex`** — En uygun kutunun indeksini tutar. Başlangıçta `-1` (henüz uygun kutu yok); uygun kutu bulununca o kutunun indeksiyle güncellenir (`bestBinIndex = j`).

* **`minimumRemaining`** — Yerleştirme sonrasında kalabilecek en küçük boşluğu tutar. Başlangıç değeri `capacity + 1` verilir, böylece bulunan ilk uygun kutu otomatik olarak en iyi aday olur. `remainingAfterPlacement < minimumRemaining` koşuluyla daha iyi bir kutu bulunup bulunmadığı kontrol edilir.

* **`remainingAfterPlacement`** — Nesne belirli bir kutuya yerleştirilirse kutuda kalacak boşluğu hesaplar (`binRemaining[j] - weights[i]`). Bu, nesneyi kutuya hemen yerleştirmez; yalnızca uygunluğu ölçer.

* **`sizeof`** — Ana programda nesne sayısını bulmak için kullanılır: `int n = sizeof(weights) / sizeof(weights[0]);`

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <vector>
```

* `<iostream>`: `cout` kullanılarak ekrana çıktı yazdırmak için kullanılır.
* `<vector>`: Kutuların kalan kapasitelerini saklamak için kullanılır.

Kodda bu kütüphaneler yerine:

```cpp
#include <bits/stdc++.h>
```

kullanılabilir. Bu başlık birçok standart C++ kütüphanesini programa ekler. Ancak standart bir C++ başlık dosyası olmadığı için bazı derleyiciler tarafından desteklenmeyebilir.

---

## Zaman Karmaşıklığı

Nesneleri sırayla gezen dış döngü:

```cpp
for (int i = 0; i < n; i++)
```

toplam `N` kez çalışır.

Her nesne için daha önce açılmış bütün kutuları kontrol eden iç döngü:

```cpp
for (int j = 0; j < binCount; j++)
```

en kötü durumda `N` kutuya kadar kontrol yapabilir.

Best Fit, uygun bir kutu bulduğunda işlemi durdurmaz. En az boşluk bırakacak kutuyu bulmak için açık olan bütün kutuları kontrol eder.

Bu nedenle toplam zaman karmaşıklığı:

```text
O(N × N)
```

olur. Bu ifade:

```text
O(N²)
```

şeklinde gösterilir.

### Best Case — O(N)

En iyi durumda bütün nesneler aynı kutuya sığabilir ve algoritma boyunca yalnızca bir kutu açık kalabilir.

Örneğin kutu kapasitesi `100` ve nesneler:

```text
1, 2, 3, 4, 5
```

olsun.

İlk nesne yeni bir kutu açar. Diğer nesneler için yalnızca bu kutu kontrol edilir.

Bu durumda dış döngü `N` kez, iç döngü ise her nesne için bir kez çalışır.

Bu nedenle en iyi durum zaman karmaşıklığı:

```text
O(N)
```

olur.

### Average Case — O(N²)

Ortalama durumda birden fazla kutu açılır. Best Fit algoritması, her nesne için en uygun kutuyu bulmak amacıyla açık olan bütün kutuları kontrol eder.

Kontrol edilen kutu sayısı nesnelerin ağırlıklarına, kutu kapasitesine ve nesnelerin sırasına bağlıdır. Temel Best Fit uygulamasının ortalama durum karmaşıklığı genel olarak:

```text
O(N²)
```

şeklinde kabul edilir.

### Worst Case — O(N²)

En kötü durumda her nesne daha önce açılmış bütün kutularla karşılaştırılır ve hiçbir kutuya sığmadığı için yeni bir kutu açılır.

Örneğin kutu kapasitesi `10` ve nesneler:

```text
6, 6, 6, 6, 6
```

olsun.

Kontrol sayıları:

```text
1. nesne → 0 kutu
2. nesne → 1 kutu
3. nesne → 2 kutu
4. nesne → 3 kutu
5. nesne → 4 kutu
```

şeklinde artar.

Toplam kontrol sayısı:

```text
0 + 1 + 2 + ... + (N - 1)
```

olur.

Bu toplamın matematiksel karşılığı:

```text
N × (N - 1) / 2
```

şeklindedir.

Bu ifade yaklaşık olarak `N²` ile orantılı olduğu için en kötü durum zaman karmaşıklığı:

```text
O(N²)
```

olur.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(N)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Algoritmada her kutunun kalan kapasitesini saklamak için `N` elemanlı tek boyutlu bir vektör kullanılmıştır.

```cpp
vector<int> binRemaining(n);
```

En kötü durumda her nesne ayrı bir kutuya yerleştirilebilir. Bu nedenle en fazla `N` kutunun kalan kapasitesinin saklanması gerekir.

`binCount`, `i`, `j`, `bestBinIndex`, `minimumRemaining` ve `remainingAfterPlacement` gibi diğer değişkenler sabit miktarda bellek kullandığı için `O(1)` alan tüketir.

Nesnelerin ağırlıklarını tutan `weights` dizisi girdinin kendisi olduğu için ek alan karmaşıklığına dahil edilmez.

Bu nedenle algoritmanın ek alan karmaşıklığı:

```text
O(N)
```

olur.