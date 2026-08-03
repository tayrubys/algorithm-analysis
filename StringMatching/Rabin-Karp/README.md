# Rabin-Karp Algoritması — String Matching

Bu bölümde String Matching (Dizi Eşleştirme) probleminin, örüntüyü (pattern) ve metin üzerindeki her pencereyi doğrudan karakter karakter karşılaştırmak yerine **hash değerleri** üzerinden karşılaştırarak çözülmesini sağlayan **Rabin-Karp** yöntemi açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Naive algoritmanın her konumda örüntünün tamamını karakter karakter karşılaştırma zorunluluğunun yarattığı performans kaybını azaltmak için geliştirilmiştir. Karşılaştırma işlemi önce hash değerleri üzerinden yapılır, karakter karşılaştırması yalnızca hash değerleri eşleştiğinde gerçekleştirilir.
- **Hangi eksikliği gidermektedir?** Naive algoritmada her pencerede örüntünün baştan sona karşılaştırılması gerekliliğini giderir. Rabin-Karp'ta bir sonraki pencerenin hash değeri, önceki pencerenin hash değerinden **kayan (rolling) hash** yöntemiyle `O(1)` sürede hesaplanır; bu sayede her pencere için hash yeniden baştan hesaplanmaz.
- **Önceki yöntemlere göre avantajı nedir?** En büyük avantajı, özellikle **birden fazla örüntünün aynı anda arandığı durumlarda**, hash karşılaştırmasının karakter karşılaştırmasından daha ucuz olmasıdır. Ayrıca rolling hash sayesinde pencereden pencereye geçiş sabit zamanda yapılır.
- **Hangi durumda tercih edilir?**
  - Birden fazla örüntünün aynı metin içinde aynı anda arandığı durumlarda (plagiarism/intihal tespiti gibi).
  - Hash çakışmalarının (collision) az olduğu, iyi seçilmiş `d` (taban) ve `q` (asal sayı) değerleriyle ortalama performansın önemli olduğu durumlarda.
- **Hangi durumda tercih edilmez?**
  - `q` asal sayısının kötü seçildiği veya alfabenin/metnin hash çakışmalarına çok müsait olduğu durumlarda. Bu durumda `p == t` koşulu sık sık sağlanır ancak karakterler eşleşmez (**spurious hit / yanlış alarm**), bu da algoritmayı Naive algoritma kadar yavaşlatabilir.
  - Çok büyük `d * q` çarpımlarının integer taşmasına (overflow) yol açabileceği, taşma kontrolüne dikkat edilmesi gereken ortamlarda.

## Algoritmanın Çalışma Mantığı

Metin ve örüntü doğrudan karşılaştırılmaz; önce her ikisinin de sayısal bir hash değeri hesaplanır. Metin üzerinde `m` (örüntü uzunluğu) genişliğinde bir pencere kaydırılır ve bu pencerenin hash değeri örüntünün hash değeriyle karşılaştırılır.

Her pencere için:

1. Örüntünün (`pat`) hash değeri (`p`) ve metnin ilk penceresinin hash değeri (`t`) hesaplanır.
2. Pencerenin hash değeri (`t`) örüntünün hash değeriyle (`p`) karşılaştırılır.
3. Hash değerleri eşitse (`p == t`), gerçekten eşleşme olup olmadığını doğrulamak için karakterler tek tek karşılaştırılır (hash çakışması ihtimaline karşı).
4. Karakterler de eşleşiyorsa, pencerenin başlangıç indeksi sonuç listesine eklenir.
5. Bir sonraki pencereye geçilirken hash değeri baştan hesaplanmaz; **rolling hash** formülüyle mevcut hash değerinden bir sonraki pencerenin hash değeri türetilir: soldaki karakter çıkarılır, sağdaki yeni karakter eklenir.
6. Metnin sonuna kadar bu işlem tekrarlanır.
7. Bütün pencereler denendikten sonra bulunan eşleşmelerin başlangıç indekslerinin listesi döndürülür.

### Örnek

Metin ve örüntü aşağıdaki gibi olsun:

```text
Metin (txt)   : "kocaeli universitesi bilisim sistemleri muhendisligi bolumu"
Örüntü (pat)  : "b"
```

Karşılaştırmaların gerçekleşmesi şu şekildedir:

```text
d = 256 (ASCII taban değeri), q = 101 (asal modül)
m = 1 (örüntü uzunluğu), n = 61 (metin uzunluğu)

Örüntünün hash değeri (p) hesaplanır.
Metnin ilk penceresinin ("k") hash değeri (t) hesaplanır.

Pencere kaydırıldıkça her karakter için hash karşılaştırılır;
hash eşleştiğinde tek karakterlik gerçek karşılaştırma yapılır.

İndeks 21 → "b" → hash eşleşir, karakter eşleşir → Eşleşme bulundu!
İndeks 53 → "b" → hash eşleşir, karakter eşleşir → Eşleşme bulundu!
```

Algoritma metinde toplam **2 eşleşme** bulur: `21 53`.

> Rabin-Karp bir konumda hash değerleri eşleşse bile bunun gerçek bir eşleşme olduğundan emin olmak için mutlaka karakter karşılaştırması yapar. Aksi hâlde hash çakışması (spurious hit) yanlış sonuca yol açabilir.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon search(pat, txt):

    d = 256   // ASCII alfabe boyutu
    q = 101   // Taşmayı azaltmak için kullanılan asal sayı

    m = pat uzunluğu   // pattern uzunluğu
    n = txt uzunluğu   // text uzunluğu

    p = 0   // pattern'in hash değeri
    t = 0   // text penceresinin hash değeri
    h = 1   // en soldaki karakterin katsayısı (d^(m-1) mod q)

    // Bulunan eşleşme indekslerini saklayacak bir vektör oluştur
    result = boş vektör

    // h değerini hesapla: h = d^(m-1) mod q
    Döngü (i = 0'dan m - 2'ye kadar):
        h = (h * d) mod q

    // pattern'in ve ilk pencerenin hash değerlerini hesapla
    Döngü (i = 0'dan m - 1'e kadar):
        p = (d * p + pat[i]) mod q
        t = (d * t + txt[i]) mod q

    // pattern'i text üzerinde birer karakter kaydır
    Döngü (i = 0'dan (n - m)'e kadar):

        // Hash değerleri aynıysa karakterleri tek tek kontrol et
        Eğer p == t ise:

            match = doğru

            Döngü (j = 0'dan m - 1'e kadar):
                Eğer txt[i + j] != pat[j] ise:
                    match = yanlış
                    Döngüden çık

            // Tam eşleşme varsa başlangıç indeksini kaydet
            Eğer match == doğru ise:
                result.ekle(i)

        // Bir sonraki pencerenin hash değerini hesapla (rolling hash)
        Eğer i < n - m ise:
            t = (d * (t - txt[i] * h) + txt[i + m]) mod q

            // Hash negatif çıkarsa pozitife çevir
            Eğer t < 0 ise:
                t = t + q

    // Bulunan tüm eşleşme indekslerini döndür
    Döndür result
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`const string &pat`, `const string &txt`** — Sırasıyla aranan örüntüyü ve içinde arama yapılacak ana metni referans olarak saklar. `const &` kullanımı, gereksiz kopyalama yapılmasını önler ve fonksiyon içinde değiştirilmelerini engeller.

* **`int d = 256`** — Kullanılan alfabenin boyutunu temsil eder (ASCII karakter kümesi). Hash hesaplamasında taban (base) olarak kullanılır.

* **`int q = 101`** — Hash hesaplamasında kullanılan bir asal sayıdır. Hash değerlerinin çok büyümesini (taşmasını) önlemek amacıyla `mod` işleminde kullanılır.

* **`int m`, `int n`** — `m`, `pat` dizisinin uzunluğunu; `n`, `txt` dizisinin uzunluğunu tutar: `int m = pat.length(); int n = txt.length();`

* **`int p`, `int t`** — `p`, örüntünün hash değerini; `t`, metindeki mevcut pencerenin hash değerini tutar. Karşılaştırma bu iki değer üzerinden yapılır.

* **`int h`** — Pencerenin en soldaki (en eski) karakterinin hash formülündeki katsayısını (`d^(m-1) mod q`) tutar. Rolling hash hesaplamasında bu karakteri hash değerinden çıkarmak için kullanılır.

* **`vector<int> result`** — Bulunan eşleşmelerin başlangıç indekslerini tutar. `push_back` ile dinamik olarak büyür.

* **`bool match`** — İç döngüde karakterlerin gerçekten birebir eşleşip eşleşmediğini takip eden bayrak (flag) değişkendir. Hash eşleşmesinin gerçek bir eşleşme olup olmadığını doğrulamak için kullanılır.

* **`break`** — `txt[i + j] != pat[j]` koşulu sağlandığında (bir uyumsuzluk tespit edildiğinde) iç karşılaştırma döngüsünü sonlandırır; örüntünün kalan karakterleri gereksiz yere kontrol edilmez.

* **`.length()`** — Metin ve örüntünün uzunluklarını bulmak için kullanılır.

* **Rolling Hash formülü** — `t = (d * (t - txt[i] * h) + txt[i + m]) % q` satırı, bir önceki pencerenin hash değerinden bir sonraki pencerenin hash değerini `O(1)` sürede türetir: soldaki karakterin (`txt[i]`) katkısı çıkarılır, sağa eklenen yeni karakterin (`txt[i + m]`) katkısı eklenir. `%` işleminin negatif sonuç verebilmesi ihtimaline karşı `if (t < 0) t += q;` satırıyla değer pozitife çevrilir.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <string>
#include <vector>
```

- `<iostream>`: `cout` kullanılarak ekrana çıktı yazdırmak için kullanılır.
- `<string>`: Metin ve örüntü verilerini `string` türünde saklamak için kullanılır.
- `<vector>`: Bulunan eşleşme indekslerini saklamak için kullanılır.

---

## Zaman Karmaşıklığı

Pattern ve ilk pencerenin hash değerlerini hesaplayan döngüler `O(M)` sürede çalışır.

Metin üzerinde pencereyi kaydıran ana döngü:

```cpp
for (int i = 0; i <= n - m; i++)
```

toplam `N - M + 1` kez çalışır. Her adımda:

- Hash karşılaştırması (`p == t`) `O(1)` sürede yapılır.
- Rolling hash ile bir sonraki pencerenin hash değeri `O(1)` sürede hesaplanır.
- Hash değerleri eşleştiğinde (`p == t`), doğrulama amaçlı karakter karşılaştırması `O(M)` sürede yapılır.

Bu nedenle toplam zaman karmaşıklığı, hash çakışmalarının (spurious hit) sıklığına bağlıdır.

### Best Case — O(N + M)

En iyi durumda hash değerleri yalnızca gerçek eşleşme konumlarında eşit çıkar (hash çakışması yaşanmaz). Bu durumda her pencerede sadece `O(1)` hash karşılaştırması yapılır ve karakter karşılaştırması yalnızca gerçek eşleşmelerde gerçekleşir. Bu nedenle en iyi durum zaman karmaşıklığı:

```text
O(N + M)
```

olur (`N`: metin uzunluğu, `M`: örüntü uzunluğu).

### Average Case — O(N + M)

`q` asal sayısı iyi seçildiğinde hash çakışmaları oldukça nadir görülür. Bu nedenle ortalama durumda her pencerede yalnızca `O(1)` hash karşılaştırması yapılır ve tam karakter karşılaştırması nadiren gerekir. Temel Rabin-Karp uygulamasının ortalama durum karmaşıklığı genel olarak:

```text
O(N + M)
```

şeklinde kabul edilir.

### Worst Case — O(N × M)

En kötü durumda hash çakışmaları (spurious hit) çok sık yaşanır; yani `p == t` koşulu çoğu pencerede sağlanır ancak karakterler gerçekte eşleşmez. Bu durumda hemen hemen her pencerede `O(M)` sürecek bir karakter karşılaştırması yapılır.

Örneğin `q` küçük seçildiğinde veya metin/örüntü tek tip karakterlerden oluştuğunda (`"aaaa...a"` gibi) hash değerleri sürekli çakışabilir. Bu durumda toplam karmaşıklık:

```text
O(N × M)
```

olur; yani en kötü durumda Rabin-Karp, Naive algoritma ile aynı performansa geriler.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(1)** *(eşleşme listesi hariç)*

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Algoritmada hash hesaplamaları için yalnızca sabit sayıda değişken (`d`, `q`, `m`, `n`, `p`, `t`, `h`, `i`, `j`, `match`) kullanılmıştır. Bu değişkenler `O(1)` alan tüketir.

```cpp
int d, q, m, n, p, t, h;
```

Metin (`txt`) ve örüntü (`pat`) girdinin kendisi olduğu için ek alan karmaşıklığına dahil edilmez.

Bulunan eşleşme indekslerini saklamak için kullanılan `result` vektörü, en kötü durumda (örüntü metnin her konumunda eşleşirse) `O(N)` boyutuna ulaşabilir. Bu nedenle eşleşme listesi dahil edildiğinde alan karmaşıklığı `O(N)` olarak da ifade edilebilir; ancak algoritmanın kendi hash hesaplama mantığı için gereken ek alan:

```text
O(1)
```

olur.