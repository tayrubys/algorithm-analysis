# First Fit Algoritması — Bin Packing

Bu bölümde Bin Packing probleminin, nesneleri sığabilecekleri ilk kutuya yerleştirerek çözülmesini sağlayan **First Fit** yöntemi açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı
 
- **Bu algoritma neden geliştirilmiştir?** Bin Packing probleminde bütün olası yerleştirmeleri deneyerek kesin sonucu bulma zorunluluğunun yarattığı performans kaybını engellemek, bunun yerine nesneleri kutulara hızlı ve kolay bir şekilde yerleştirerek kabul edilebilir bir çözüm üretmek için geliştirilmiştir.
- **Hangi eksikliği gidermektedir?** Bin Packing probleminde bütün olası yerleştirmeleri kontrol etmenin nesne sayısı arttıkça yarattığı büyük zaman kaybını giderir; bütün olasılıkları denemek yerine her nesneyi sığdığı ilk kutuya yerleştirerek daha kısa sürede sonuç üretir.
- **Önceki yöntemlere göre avantajı nedir?** En büyük avantajı basitliğidir. Çalışma mantığı ve kodlanması oldukça kolaydır, karmaşık veri yapılarına ihtiyaç duymaz ve kesin çözüm yöntemlerine göre çok daha hızlı çalışır.
- **Hangi durumda tercih edilir?**
  - Nesne sayısının fazla olduğu durumlarda.
  - Hızlı bir sonuca ihtiyaç duyulduğu durumlarda.
  - Minimum kutu sayısının kesin olarak bulunmasının zorunlu olmadığı durumlarda.
  - Nesneler sisteme sırayla geldiği için çevrim içi (online) sistemlerde.
- **Hangi durumda tercih edilmez?**
  - Minimum kutu sayısının kesin olarak bulunması gereken durumlarda (Örn: `weight = [6, 6, 6, 6, 6]`, kapasite `= 10`). First Fit nesnelerin geliş sırasından etkilendiği için bu durumda gereğinden fazla kutu kullanır ve algoritma optimum çözümden uzaklaşır.


## Algoritmanın Çalışma Mantığı

Nesneler verildikleri sıraya göre tek tek ele alınır. Her nesne için daha önce açılmış kutular ilk kutudan başlanarak kontrol edilir.

Her nesne için:

1. Nesnenin ağırlığı alınır.
2. Açılmış kutular ilk kutudan başlanarak kontrol edilir.
3. Nesnenin sığabileceği ilk kutu bulunur.
4. Nesne bu kutuya yerleştirilir.
5. Kutunun kalan kapasitesi nesnenin ağırlığı kadar azaltılır.
6. Nesne mevcut kutuların hiçbirine sığmazsa yeni bir kutu açılır.
7. Bütün nesneler yerleştirildikten sonra kullanılan kutu sayısı döndürülür.

### Örnek

Kutu kapasitesi `10` ve nesneler aşağıdaki gibi olsun:

```text
2, 5, 4, 7, 1, 3, 8
```

Nesnelerin yerleştirilmesi şu şekilde gerçekleşir:

```text
Kutu 1: 2 + 5 + 1 = 8
Kutu 2: 4 + 3 = 7
Kutu 3: 7
Kutu 4: 8
```

Algoritma toplam **4** kutu kullanır.

> First Fit algoritması bir nesne için en uygun kutuyu aramaz. Nesnenin sığdığı ilk kutuyu kullanır. Bu nedenle nesnelerin veriliş sırası sonucu etkileyebilir.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon FirstFit(Agirliklar, NesneSayisi, KutuKapasitesi):

    // Kullanılan kutu sayısını başlangıçta sıfır yap
    KullanilanKutuSayisi = 0

    // Kutuların kalan kapasitelerini saklayacak bir dizi oluştur
    KalanKapasiteler = NesneSayisi boyutunda dizi

    // Nesneleri sırayla yerleştir
    Döngü (i = 0'dan NesneSayisi - 1'e kadar):

        // Nesne ağırlığının geçerli olup olmadığını kontrol et
        Eğer Agirliklar[i] <= 0 veya Agirliklar[i] > KutuKapasitesi ise:
            Hata döndür

        // Nesnenin sığabileceği ilk kutuyu ara
        Döngü (j = 0'dan KullanilanKutuSayisi - 1'e kadar):

            Eğer KalanKapasiteler[j] >= Agirliklar[i] ise:

                // Nesneyi kutuya yerleştir
                KalanKapasiteler[j] =
                    KalanKapasiteler[j] - Agirliklar[i]

                // İlk uygun kutu bulunduğu için aramayı durdur
                Döngüden çık

        // Nesne mevcut kutuların hiçbirine sığmadıysa
        Eğer j == KullanilanKutuSayisi ise:

            // Yeni kutu aç ve kalan kapasitesini hesapla
            KalanKapasiteler[KullanilanKutuSayisi] =
                KutuKapasitesi - Agirliklar[i]

            KullanilanKutuSayisi =
                KullanilanKutuSayisi + 1

    // Kullanılan toplam kutu sayısını döndür
    Döndür KullanilanKutuSayisi
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

### `const int weight[]`

Kutulara yerleştirilecek nesnelerin ağırlıklarını saklamak için kullanılır.

```cpp
weight[i]
```

Bu ifade, `i` indeksindeki nesnenin ağırlığını gösterir.

`const` ifadesi, ağırlık dizisinin fonksiyon içerisinde değiştirilmesini engeller.

### `vector<int>`

Kutuların kalan kapasitelerini saklamak için kullanılır.

```cpp
vector<int> binRemaining(n);
```

Vektörün her elemanı bir kutunun kalan kapasitesini gösterir.

```text
binRemaining[0] → Birinci kutunun kalan kapasitesi
binRemaining[1] → İkinci kutunun kalan kapasitesi
```

En kötü durumda her nesne için ayrı bir kutu açılabileceği için vektörün boyutu `N` olarak oluşturulur.

### `break`

Nesne uygun bir kutuya yerleştirildiğinde kutuları kontrol eden döngüyü sonlandırmak için kullanılır.

```cpp
break;
```

First Fit algoritması nesneyi sığdığı ilk kutuya yerleştirdiği için uygun kutu bulunduktan sonra diğer kutular kontrol edilmez.

### `sizeof`

Ana programda ağırlık dizisinin eleman sayısını hesaplamak için kullanılır.

```cpp
int n = sizeof(weight) / sizeof(weight[0]);
```

`sizeof(weight)` dizinin toplam boyutunu, `sizeof(weight[0])` ise dizinin bir elemanının boyutunu verir. Bu değerler bölünerek nesne sayısı bulunur.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <vector>
```

- `<iostream>`: `cout` kullanılarak ekrana çıktı yazdırmak için kullanılır.
- `<vector>`: Kutuların kalan kapasitelerini saklamak için kullanılır.

Kodda bu kütüphaneler yerine:

```cpp
#include <bits/stdc++.h>
```

kullanılmıştır. Bu başlık birçok standart C++ kütüphanesini programa ekler. Ancak standart bir C++ başlık dosyası olmadığı için bazı derleyiciler tarafından desteklenmeyebilir.

---

## Zaman Karmaşıklığı

Nesneleri sırayla gezen dış döngü:

```cpp
for (int i = 0; i < n; i++)
```

toplam `N` kez çalışır.

Her nesne için daha önce açılmış kutuları kontrol eden iç döngü:

```cpp
for (j = 0; j < binCount; j++)
```

en kötü durumda `N` kutuya kadar kontrol yapabilir.

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

En iyi durumda ilk nesne yeni bir kutu açar ve diğer nesnelerin tamamı ilk kontrol edilen kutuya sığar.

İç döngü her nesne için yalnızca bir kez çalışır ve `break` ile sonlanır. Bu nedenle en iyi durum zaman karmaşıklığı:

```text
O(N)
```

olur.

### Average Case — O(N²)

Ortalama durumda nesnenin sığabileceği kutuyu bulmak için birden fazla kutunun kontrol edilmesi gerekebilir.

Kontrol edilen kutu sayısı nesnelerin ağırlıklarına, kutu kapasitesine ve nesnelerin sırasına bağlıdır. Temel First Fit uygulamasının ortalama durum karmaşıklığı genel olarak:

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

olur. Bu toplam yaklaşık olarak `N²` ile orantılı olduğu için en kötü durum zaman karmaşıklığı:

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

`binCount`, `i` ve `j` gibi diğer değişkenler sabit miktarda bellek kullandığı için `O(1)` alan tüketir.

Nesnelerin ağırlıklarını tutan `weight` dizisi girdinin kendisi olduğu için ek alan karmaşıklığına dahil edilmez.

Bu nedenle algoritmanın ek alan karmaşıklığı:

```text
O(N)
```

olur. 