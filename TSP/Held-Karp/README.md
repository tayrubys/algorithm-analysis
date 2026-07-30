# Held–Karp Algoritması — TSP

Bu bölümde Gezgin Satıcı Problemi’ni çözmek için kullanılan klasik bir dinamik programlama algoritması olan Held-Karp açıklanmaktadır.

TSP, NP-zor bir problem olmasına rağmen Held–Karp algoritması üstel zamanda kesin çözüm üretir. Bu yöntem, küçük ve orta büyüklükteki girdiler için brute-force yöntemine göre oldukça daha verimlidir.

### Algoritmanın Geliştirilme Amacı

Held–Karp algoritması, Brute Force yönteminde aynı alt problemlerin tekrar tekrar hesaplanmasını önlemek amacıyla geliştirilmiştir.

Brute Force algoritması bütün şehir sıralamalarını ayrı ayrı oluştururken Held–Karp, daha önce hesaplanan sonuçları bir dinamik programlama tablosunda saklar. Aynı şehir grubu ve aynı son şehir için yalnızca en düşük maliyetli sonuç tutulur.

* **Hangi eksikliği gidermektedir?** Brute Force algoritmasında aynı şehir gruplarına ait maliyetler farklı rotalarda tekrar hesaplanır. Held–Karp, bu sonuçları saklayarak gereksiz tekrarları azaltır.
* **Brute Force yöntemine göre avantajı nedir?** Brute Force algoritmasının `O(N!)` zaman karmaşıklığına karşılık Held–Karp algoritması `O(N² × 2ⁿ)` zamanda çalışır.
* **Hangi durumda tercih edilir?** Şehir sayısının çok büyük olmadığı ve kesin sonucun bulunmasının gerektiği problemlerde tercih edilir.
* **Hangi durumda tercih edilmez?** Şehir sayısı arttıkça `2ⁿ` farklı alt küme oluştuğu için hem çalışma süresi hem de bellek kullanımı hızlı bir şekilde artar. Bu nedenle çok büyük veri setlerinde tercih edilmez.

### Algoritmanın Çalışma Mantığı

Held–Karp algoritması, TSP problemini küçük alt problemlere ayırarak dinamik programlama ile çözer.

Algoritma şu bilgiyi hesaplar:

```text
Belirli şehirleri ziyaret edip son olarak j şehrinde
bulunmanın minimum maliyeti nedir?
```

Bu değerler aşağıdaki tabloda saklanır:

```text
dp[mask][j]
```

Burada:

- `mask`, ziyaret edilen şehirleri,
- `j`, son bulunulan şehri,
- `dp[mask][j]`, bu duruma ulaşmanın minimum maliyetini gösterir.

### Bitmask Kullanımı

Ziyaret edilen şehirler ikili sayılarla gösterilir:

```text
0001 → Yalnızca 0 ziyaret edildi
0011 → 0 ve 1 ziyaret edildi
0101 → 0 ve 2 ziyaret edildi
1111 → Bütün şehirler ziyaret edildi
```

`1` ziyaret edilmiş, `0` ise ziyaret edilmemiş şehir anlamına gelir.

Toplam alt küme sayısı:

```cpp
int N = 1 << n;
```

şeklinde hesaplanır. `n = 4` için toplam `16` alt küme bulunur.

### Başlangıç Durumu

Başlangıç şehri `0` olduğu için:

```cpp
dp[1][0] = 0;
```

olarak belirlenir. Buradaki `1`, yalnızca `0` numaralı şehrin ziyaret edildiğini gösterir.

### Dinamik Programlama Geçişi

Bir `j` şehrinde biten minimum maliyet, `j` şehrinden önce bulunulabilecek bütün `k` şehirleri kontrol edilerek hesaplanır:

```text
dp[mask][j] =
min(dp[prevMask][k] + dist[k][j])
```

Burada `prevMask`, `j` şehri çıkarılmadan önceki şehir kümesidir:

```cpp
int prevMask = mask ^ (1 << j);
```

Aynı şehirler ziyaret edilip aynı şehirde biten birden fazla rota varsa yalnızca en düşük maliyetli olan sonuç saklanır.

### Turun Tamamlanması

Bütün şehirler ziyaret edildikten sonra son şehirden başlangıç şehrine dönüş maliyeti eklenir:

```text
dp[tüm şehirler][son şehir] + dist[son şehir][0]
```

En küçük toplam maliyete sahip sonuç seçilir.

### Rotanın Oluşturulması

En kısa rotayı bulmak için `parent[mask][j]` tablosu kullanılır. Bu tablo, `j` şehrinden önce hangi şehirde bulunulduğunu saklar.

Rota son şehirden başlangıç şehrine doğru geriye takip edilir, ardından ters çevrilir ve başlangıç şehri sona eklenir.

Örneğin:

```text
0 → 2 → 3 → 1 → 0
```
### Algoritmanın Çalışma Adımları

1. Şehir sayısı ve oluşturulabilecek alt küme sayısı hesaplanır.
2. `dp` tablosunun bütün değerleri başlangıçta sonsuz yapılır.
3. Yalnızca başlangıç şehrinin ziyaret edildiği durumun maliyeti `0` yapılır.
4. Başlangıç şehrini içeren bütün şehir alt kümeleri incelenir.
5. Her alt küme için son olarak bulunulabilecek şehirler kontrol edilir.
6. Son şehirden önce bulunulabilecek bütün şehirler karşılaştırılır.
7. Daha düşük maliyet bulunduğunda `dp` ve `parent` tabloları güncellenir.
8. Bütün şehirler ziyaret edildikten sonra başlangıç şehrine dönüş maliyeti eklenir.
9. En düşük maliyetli son şehir belirlenir.
10. `parent` tablosu geriye doğru takip edilerek en kısa rota oluşturulur.

### Sözde Kod (Pseudo-Code)

```text
Fonksiyon HeldKarp(MesafeMatrisi):

    n = şehir sayısı
    AltKumeSayisi = 2^n
    DP tablosunun bütün değerlerini SONSUZ yap
    Parent tablosunun bütün değerlerini -1 yap

    DP[sadece başlangıç şehri][başlangıç şehri] = 0

    Her Mask değeri için:

        Eğer başlangıç şehri Mask içinde değilse:
            Sonraki Mask değerine geç

        Her SonSehir için:

            Eğer SonSehir Mask içinde değilse:
                Sonraki şehre geç

            OncekiMask = Mask içinden SonSehir'i çıkar

            Her OncekiSehir için:

                Eğer OncekiSehir, OncekiMask içinde bulunuyorsa:

                    YeniMaliyet =
                        DP[OncekiMask][OncekiSehir]
                        + Mesafe[OncekiSehir][SonSehir]

                    Eğer YeniMaliyet < DP[Mask][SonSehir] ise:

                        DP[Mask][SonSehir] = YeniMaliyet
                        Parent[Mask][SonSehir] = OncekiSehir

    TumSehirlerMaskesi = 2^n - 1
    EnIyiMaliyet = SONSUZ
    SonSehir = -1

    Başlangıç şehri dışındaki her şehir için:

        ToplamMaliyet =
            DP[TumSehirlerMaskesi][Sehir]
            + Mesafe[Sehir][BaşlangıçSehri]

        Eğer ToplamMaliyet < EnIyiMaliyet ise:

            EnIyiMaliyet = ToplamMaliyet
            SonSehir = Sehir

    Parent tablosunu kullanarak rotayı geriye doğru oluştur
    Rotayı ters çevir
    Başlangıç şehrini rotanın sonuna ekle

    EnIyiMaliyet ve Rotayı döndür
```

### Kullanılan Veri Yapıları ve C++ Özellikleri

### `vector<vector<long long>>`

Maliyet matrisini ve dinamik programlama tablosunu saklamak için kullanılır.

Maliyet matrisi:

```cpp
dist[i][j]
```

`i` numaralı şehirden `j` numaralı şehre gitmenin maliyetini gösterir.

DP tablosu:

```cpp
dp[mask][j]
```

`mask` içerisindeki şehirleri ziyaret edip `j` şehrinde bitmenin minimum maliyetini tutar.

### `vector<vector<int>>`

`parent` tablosunu saklamak için kullanılır.

```cpp
parent[mask][j]
```

`j` şehrinden önce hangi şehirde bulunulduğunu gösterir.

### `vector<int>`

Bulunan en kısa rotadaki şehirleri saklamak için kullanılır.

Örneğin:

```cpp
tour = {0, 2, 3, 1, 0};
```

### `pair<long long, vector<int>>`

Fonksiyondan hem minimum maliyeti hem de en kısa rotayı döndürmek için kullanılır.

```cpp
result.first
```

Minimum tur maliyetini tutar.

```cpp
result.second
```

Bulunan turu tutar.

### Bit İşlemleri

#### `1 << n`

`2ⁿ` değerini hesaplamak ve toplam alt küme sayısını bulmak için kullanılır.

```cpp
int N = 1 << n;
```

#### `mask & (1 << j)`

`j` numaralı şehrin alt kümede bulunup bulunmadığını kontrol eder.

```cpp
if (mask & (1 << j))
```

Sonuç sıfırdan farklıysa `j` şehri alt kümede bulunmaktadır.

#### `mask ^ (1 << j)`

`j` numaralı şehri alt kümeden çıkarmak için kullanılır.

```cpp
int prevMask = mask ^ (1 << j);
```

### `reverse`

Rota `parent` tablosu kullanılarak sondan başa doğru oluşturulduğu için sırayı düzeltmek amacıyla kullanılır.

```cpp
reverse(tour.begin(), tour.end());
```

### Structured Binding

Fonksiyonun döndürdüğü maliyet ve rota değerlerini ayrı değişkenlere almak için kullanılır.

```cpp
auto [cost, tour] = heldKarp(dist);
```

Bu özellik C++17 ve daha yeni sürümlerde kullanılabilir.

---

### Kullanılan Kütüphaneler

Kodda:

```cpp
#include <bits/stdc++.h>
```

kullanılmıştır.

Bu başlık, GCC derleyicisinde standart C++ kütüphanelerinin büyük bölümünü tek seferde ekler.

Kodda kullanılan temel kütüphaneler ayrı ayrı yazılmak istenirse:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
```

şeklinde eklenebilir.

* `<iostream>`: Sonuçları ekrana yazdırmak için kullanılır.
* `<vector>`: Maliyet matrisi, DP tablosu, parent tablosu ve rotayı saklamak için kullanılır.
* `<algorithm>`: `reverse` fonksiyonunu kullanmak için eklenir.
* `<climits>`: `LLONG_MAX` değerini kullanmak için eklenir.
* `<utility>`: `pair` veri yapısını kullanmak için eklenir.

> `bits/stdc++.h` standart bir C++ başlığı değildir. GCC tabanlı derleyicilerde çalışır. Daha taşınabilir bir kod için gerekli kütüphanelerin ayrı ayrı eklenmesi önerilir.

### Zaman Karmaşıklığı

Held–Karp algoritmasında `n` şehir için:

```text
2ⁿ
```

farklı şehir alt kümesi bulunur.

Her alt küme için:

* Son şehir olabilecek yaklaşık `n` şehir,
* Önceki şehir olabilecek yaklaşık `n` şehir

kontrol edilir.

Bu nedenle toplam zaman karmaşıklığı:

```text
O(2ⁿ × n × n)
```

olur.

Sadeleştirilmiş hali:

```text
O(n² × 2ⁿ)
```

şeklindedir.

Bu kod, maliyet değerlerinden bağımsız olarak bütün geçerli alt kümeleri ve şehir seçeneklerini kontrol eder. Bu nedenle en iyi, ortalama ve en kötü durumda asimptotik zaman karmaşıklığı aynıdır.

```text
Best Case    : O(n² × 2ⁿ)
Average Case : O(n² × 2ⁿ)
Worst Case   : O(n² × 2ⁿ)
```

Brute Force algoritmasının zaman karmaşıklığı:

```text
O(n!)
```

iken Held–Karp algoritmasının zaman karmaşıklığı:

```text
O(n² × 2ⁿ)
```

şeklindedir.

Held–Karp hâlâ üstel zamanda çalışır ancak şehir sayısı arttığında Brute Force yönteminden daha verimlidir.

### Alan Karmaşıklığı

`dp` tablosunda:

```text
2ⁿ × n
```

adet değer saklanır.

Bu nedenle DP tablosunun alan karmaşıklığı:

```text
O(n × 2ⁿ)
```

şeklindedir.

Aynı boyutta bir `parent` tablosu da kullanılır:

```text
O(n × 2ⁿ)
```

İki tablonun toplamı sabit katsayıları değiştirse de genel alan karmaşıklığı değişmez:

```text
O(n × 2ⁿ)
```

Bulunan rotayı saklayan `tour` vektörü ise en fazla `n + 1` eleman içerir ve:

```text
O(n)
```

alan kullanır.

Genel yardımcı alan karmaşıklığı:

```text
O(n × 2ⁿ)
```

şeklindedir.
